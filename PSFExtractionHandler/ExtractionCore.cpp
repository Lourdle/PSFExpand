#include "pch.h"
#include "framework.h"
#include "PSFExtHandlerFrame.h"

using namespace std;

static
bool memcmp(size_t size,
	PBYTE pBuf1,
	PBYTE pBuf2)
{
	for (size_t i = 0; i != size; ++i)
		if (pBuf1[i] != pBuf2[i])
			return false;

	return true;
}

static
bool
VerifyData(
	PBYTE pData,
	DWORD cbData,
	ALG_ID algHashType,
	PBYTE Hash,
	bool& Good)
{
	HCRYPTHASH hCryptHash = 0;
	unique_ptr<BYTE[]>pHashData;
	DWORD Length = 0;
	DWORD tmp = sizeof(Length);

	HCRYPTPROV hCryptProv = 0;
	BOOL ret = CryptAcquireContextW(&hCryptProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
	if (!ret)
		goto EndCalc;

	ret = CryptCreateHash(hCryptProv, algHashType, 0, 0, &hCryptHash);
	if (!ret)
		goto EndCalc;

	ret = CryptHashData(hCryptHash, pData, cbData, 0);
	if (!ret)
		goto EndCalc;

	ret = CryptGetHashParam(hCryptHash, HP_HASHSIZE, reinterpret_cast <PBYTE>(&Length), &tmp, 0);
	if (!ret)
		goto EndCalc;

	pHashData.reset(new BYTE[Length]());

	ret = CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData.get(), &Length, 0);
	if (!ret)
		goto EndCalc;

EndCalc:
	tmp = GetLastError();
	if (hCryptHash)
		CryptDestroyHash(hCryptHash);
	if (hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	SetLastError(tmp);

	if (!ret)
		return false;

	Good = memcmp(Length, pHashData.get(), Hash);
	return true;
}


template<typename T,typename D>
class MyUniquePtr
{
public:
	MyUniquePtr(T* ptr, D* deleter) :p(ptr), d(deleter) {}
	MyUniquePtr(const MyUniquePtr&) = delete;
	MyUniquePtr(MyUniquePtr&&) = delete;

	T* get()const
	{
		return p;
	}
	void Reset(T* ptr, D* deleter)
	{
		d(p);

		p = ptr;
		d = deleter;
	}

	~MyUniquePtr()
	{
		d(p);
	}
private:
	T* p;
	D* d;
};

bool Extract(
	HANDLE hPSF,
	HANDLE hFile,
	const FileInfo::Source* src,
	const FILETIME* time,
	WORD flags,
	DWORD& Err
)
{
	BOOL Ret;
	MyUniquePtr<BYTE, void(void*)> Data(new BYTE[src->length], operator delete[]);
	DWORD fsize = src->length;

	SetFilePointer(hPSF, src->offset, nullptr, 0);
	if (ReadFile(hPSF, Data.get(), src->length, nullptr, nullptr))
	{
		if (flags & PSFEXTHANDLER_EXTRACT_FLAG_VERIFY)
		{
			if (src->Hash.alg == INVALID_FLAG)
			{
				Err = ERROR_NOT_SUPPORTED;
				goto BeginWrite;
			}

			bool good;
			if (!VerifyData(Data.get(),
				src->length,
				src->Hash.alg,
				src->Hash.value.get(),
				good))
			{
				if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS))
				{
					Err = GetLastError();
					return false;
				}

				Err = GetLastError();
				goto BeginWrite;
			}

			if (!good)
				Err = ERROR_INVALID_DATA;
			if (!good && !(flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA))
				return false;
		}

	BeginWrite:
		if (src->type != INVALID_FLAG - 1
			&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT))
		{
			DELTA_INPUT ddi;
			ddi.Editable = FALSE;
			ddi.lpcStart = Data.get();
			ddi.uSize = src->length;

			DELTA_OUTPUT DO;
			Ret = ApplyDeltaB(src->type, {}, ddi, &DO);
			if (!Ret)
			{
				Err = GetLastError();
				return false;
			}

			Data.Reset(reinterpret_cast<BYTE*>(DO.lpStart),
				[](void* p)
				{DeltaFree(p); }
			);

			fsize = static_cast<DWORD>(DO.uSize);
		}

		Ret = WriteFile(hFile, Data.get(), fsize, nullptr, nullptr);
		if (!Ret)
			Err = GetLastError();
		else if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_DO_NOT_SET_FILE_TIME))
		{
			SetFileTime(hFile, time, time, time);
			return true;
		}
	}
	else
		Err = GetLastError();

	return false;
}
