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

inline
static bool VerifyData(
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

inline
static bool AutoCreateDirectory(wstring& file)
{
	for (auto& i : file)
		if (i == '/')
			i = '\\';

	if (file.find('\\') == wstring::npos)
		return true;

	size_t StartingPos = file.find(L"\\\\") == 0 ? 2 : 0;
	if (StartingPos == 2)
	{
		StartingPos = file.find('\\', 2);
		StartingPos = file.find('\\', 4);
		if (StartingPos == wstring::npos)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}
	}
	for (auto pos = file.find('\\', StartingPos); pos != wstring::npos; pos = file.find('\\', pos + 1))
	{
		if (pos == 0 || file[pos - 1] == ':')
			continue;

		file[pos] = 0;
		if (!CreateDirectoryW(file.c_str(), nullptr))
			if (GetLastError() != ERROR_ALREADY_EXISTS)
				return false;

		file[pos] = '\\';
	}
	return true;
}

HANDLE AutoCreateFile(PCWSTR name, PCWSTR out, DWORD flags)
{
	HANDLE hFile;
	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE)
		hFile = const_cast<PWSTR>(out);
	else
	{
		DWORD FileCreationDisposition;
		if (flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS
			|| flags & PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS)
			FileCreationDisposition = CREATE_NEW;
		else
			FileCreationDisposition = CREATE_ALWAYS;


		wstring file(L"\\\\?\\");
		if (!out)
			out = name;

		if (wcsncmp(out, L"\\\\?\\", 4) == 0
			|| _wcsnicmp(out, L"\\\\UNC\\", 6) == 0)
			file = out;
		else if (wcsncmp(out, L"\\\\", 4) == 0)
		{
			file = L"\\\\UNC";
			file += out;
		}
		else
		{
			DWORD len = GetFullPathNameW(out, 0, nullptr, nullptr);
			if (len == 0)
				return nullptr;
			file.resize(len + 3);
			GetFullPathNameW(out, len, const_cast<LPWSTR>(file.c_str() + 4), nullptr);
			file += '\\';
			file += name;
		}
		file.shrink_to_fit();

		if (!AutoCreateDirectory(file))
			return nullptr;

		hFile = CreateFileW(file.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			FileCreationDisposition,
			0,
			nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			if (GetLastError() == ERROR_FILE_EXISTS)
			{
				if (flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS)
					return nullptr;
			}
			else
				return nullptr;
	}

	return hFile;
}

bool Read(
	HANDLE hPSF,
	PVOID data,
	const FileInfo& info,
	DWORD& Err
)
{
	SetFilePointer(hPSF, info.deltaSource.offset, nullptr, 0);
	if (ReadFile(hPSF, data, info.deltaSource.length, nullptr, nullptr))
		return true;
	else
		Err = GetLastError();

	return false;
}

DWORD Write(
	PVOID Buffer,
	HANDLE hFile,
	const FileInfo& info,
	DWORD flags,
	DWORD& Err
)
{
	BOOL Ret;
	DWORD size = info.deltaSource.length;

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_VERIFY)
	{
		if (info.deltaSource.Hash.alg == INVALID_FLAG)
		{
			Err = ERROR_NOT_SUPPORTED;
			goto BeginWrite;
		}

		bool good;
		if (!VerifyData(reinterpret_cast<PBYTE>(Buffer),
			info.deltaSource.length,
			info.deltaSource.Hash.alg,
			info.deltaSource.Hash.value.get(),
			good))
		{
			if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS))
			{
				Err = GetLastError();
				return 0;
			}

			Err = GetLastError();
			goto BeginWrite;
		}

		if (!good)
			Err = ERROR_INVALID_DATA;
		if (!good && !(flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA))
			return 0;
	}

BeginWrite:
	if (info.deltaSource.type != INVALID_FLAG - 1
		&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT))
	{
		DELTA_INPUT ddi;
		ddi.Editable = FALSE;
		ddi.lpcStart = Buffer;
		ddi.uSize = info.deltaSource.length;

		DELTA_OUTPUT DO;
		Ret = ApplyDeltaB(info.deltaSource.type, {}, ddi, &DO);
		if (!Ret)
		{
			Err = GetLastError();
			return 0;
		}

		size = static_cast<DWORD>(DO.uSize);
		Ret = WriteFile(hFile, DO.lpStart, size, nullptr, nullptr);
		DeltaFree(DO.lpStart);
	}
	else
		Ret = WriteFile(hFile, Buffer, info.deltaSource.length, nullptr, nullptr);
	if (!Ret)
		Err = GetLastError();
	else if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_DO_NOT_SET_FILE_TIME))
	{
		SetFileTime(hFile, &info.time, &info.time, &info.time);
		return size;
	}
	return 0;
}
