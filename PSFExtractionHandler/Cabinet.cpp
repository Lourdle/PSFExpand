#include "pch.h"
#include "framework.h"

struct File
{
	HANDLE hEvent;
	HANDLE hFile;
};

struct SharedFile
{
	File* File;
	LONG Offset;
};

#include <fdi.h>

static FNALLOC(fnAlloc)
{
	return new BYTE[cb];
}

static FNOPEN(fnOpen)
{
#if defined(_AMD64_) || defined(_ARM64_)
#define strhextoptr strtoull
#else defined(_X86_) || defined(_ARM_)
#define strhextoptr strtoul
#endif

	auto hFile = new SharedFile;
	hFile->File = reinterpret_cast<decltype(SharedFile::File)>(strhextoptr(pszFile, nullptr, 16));
	hFile->Offset = 0;
	return reinterpret_cast<INT_PTR>(hFile);
#undef strhextoptr
}

static FNREAD(fnRead)
{
	DWORD dwBytesRead;
	WaitForSingleObject(reinterpret_cast<SharedFile*>(hf)->File->hEvent, INFINITE);
	SetFilePointer(reinterpret_cast<SharedFile*>(hf)->File->hFile, reinterpret_cast<SharedFile*>(hf)->Offset, nullptr, FILE_BEGIN);
	if (!ReadFile(reinterpret_cast<SharedFile*>(hf)->File->hFile, pv, cb, &dwBytesRead, nullptr))
		dwBytesRead = -1;
	else
		reinterpret_cast<SharedFile*>(hf)->Offset += dwBytesRead;
	SetEvent(reinterpret_cast<SharedFile*>(hf)->File->hEvent);
	return dwBytesRead;
}

static FNCLOSE(fnClose)
{
	delete reinterpret_cast<SharedFile*>(hf);
	return 0;
}

struct OutFile
{
	HANDLE hFile;
	bool UserHandle;
	FILETIME FileTime;
};

static FNWRITE(fnWrite)
{
	DWORD dwBytesWritten;

	if (!WriteFile(reinterpret_cast<OutFile*>(hf)->hFile, pv, cb, &dwBytesWritten, nullptr))
		dwBytesWritten = -1;

	return dwBytesWritten;
}

static FNSEEK(fnSeek)
{
	auto before = reinterpret_cast<SharedFile*>(hf)->Offset;
	switch (seektype)
	{
	case FILE_BEGIN:
		reinterpret_cast<SharedFile*>(hf)->Offset = dist;
		break;
	case FILE_CURRENT:
		reinterpret_cast<SharedFile*>(hf)->Offset += dist;
		break;
	case FILE_END:
		reinterpret_cast<SharedFile*>(hf)->Offset = GetFileSize(reinterpret_cast<SharedFile*>(hf)->File->hFile, nullptr) - dist;
	}
	return before;
}


struct Cab;
#define HANDLE Cab*
#define dllimport dllexport
#include "PSFExtractionHandler.h"
#undef HANDLE
#include "PSFExtHandlerFrame.h"
struct Cab : RefCountMT
{
	Cab() = default;
	Cab(const Cab& ref) : RefCountMT(ref),
		hGlobalEvent(ref.hGlobalEvent), hEvent(CreateEventW(nullptr, FALSE, TRUE, nullptr)), hFile(ref.hFile), wTotal(ref.wTotal),
		hFDI(FDICreate(
			fnAlloc, operator delete[],
			fnOpen, fnRead, fnWrite, fnClose, fnSeek,
			cpuUNKNOWN, &erf))
	{}

	HANDLE hGlobalEvent;
	HANDLE hEvent;
	HFDI hFDI;
	File hFile;
	WORD wTotal;
	ERF erf;
};
#define HANDLE Cab*

PSFEXTRACTIONHANDLER_API
HANDLE PSFExtHandler_util_OpenCabinet(PCWSTR pCabFile)
{
	HANDLE hCab = new Cab;

	hCab->hFDI = FDICreate(
		fnAlloc, operator delete[],
		fnOpen, fnRead, fnWrite, fnClose, fnSeek,
		cpuUNKNOWN, &hCab->erf);

	FDICABINETINFO info;
	hCab->hFile.hFile = CreateFileW(pCabFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if (hCab->hFile.hFile == INVALID_HANDLE_VALUE)
	{
		FDIDestroy(hCab->hFDI);
		delete hCab;
		return nullptr;
	}

	hCab->hFile.hEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
	SharedFile File = { &hCab->hFile, 0 };
	if (!FDIIsCabinet(hCab->hFDI, reinterpret_cast<INT_PTR>(&File), &info))
	{
		FDIDestroy(hCab->hFDI);
		CloseHandle(hCab->hFile.hFile);
		CloseHandle(hCab->hFile.hEvent);
		delete hCab;
		SetLastError(ERROR_BAD_FORMAT);
		return nullptr;
	}
	hCab->wTotal = info.cFiles;
	hCab->hGlobalEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
	hCab->hEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);

	return hCab;
}

PSFEXTRACTIONHANDLER_API
WORD PSFExtHandler_util_CabinetGetFileCount(HANDLE hCabinet)
{
	return hCabinet->wTotal;
}

static DWORD FDIErrToWindowsError(HANDLE h)
{
	switch (h->erf.erfOper)
	{
	case FDIERROR_NONE:
		return ERROR_SUCCESS;
	case FDIERROR_CABINET_NOT_FOUND:
		return ERROR_FILE_NOT_FOUND;
	case FDIERROR_NOT_A_CABINET:
		return ERROR_BAD_FORMAT;
	case FDIERROR_UNKNOWN_CABINET_VERSION:case FDIERROR_CORRUPT_CABINET:case FDIERROR_BAD_COMPR_TYPE:
		return ERROR_INVALID_DATA;
	case FDIERROR_ALLOC_FAIL:
		return ERROR_OUTOFMEMORY;
	case FDIERROR_USER_ABORT:
		return ERROR_CANCELLED;
	default:
		return ERROR_INVALID_FUNCTION;
	}
}

struct ExpansionInfo
{
	PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfn;
	PVOID pv;
	PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS info;
	DWORD err = ERROR_SUCCESS;
};


static FNFDINOTIFY(fnFDINotify)
{
	switch (fdint)
	{
	case fdintCOPY_FILE:
	{
		OutFile* hFile = new OutFile();
		if (reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pfn)
		{
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.pCurrentFile = pfdin->psz1;
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.ulSize = static_cast<WORD>(pfdin->cb);

			DosDateTimeToFileTime(pfdin->date, pfdin->time, &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.FileTime);
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.hFile = nullptr;
			if (&hFile->hFile)
				if (!reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pfn(PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE, { &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info }, reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pv))
				{
					delete hFile;
					reinterpret_cast<ExpansionInfo*>(pfdin->pv)->err = ERROR_CANCELLED;
					return -1;
				}
			if (reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.hFile == INVALID_HANDLE_VALUE)
			{
				delete hFile;
				return 0;
			}
			else if (reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.hFile)
			{
				hFile->hFile = reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.hFile;
				hFile->UserHandle = true;
				return reinterpret_cast<INT_PTR>(hFile);
			}
		}
		for (size_t i = 0; pfdin->psz1[i] != '\0'; ++i)
			if (pfdin->psz1[i] == '\\')
			{
				pfdin->psz1[i] = '\0';
				if (!CreateDirectoryA(pfdin->psz1, nullptr)
					&& GetLastError() != ERROR_ALREADY_EXISTS)
				{
					reinterpret_cast<ExpansionInfo*>(pfdin->pv)->err = GetLastError();
					return -1;
				}
				pfdin->psz1[i] = '\\';
			}
		hFile->hFile = CreateFileA(pfdin->psz1, GENERIC_WRITE | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hFile->hFile == INVALID_HANDLE_VALUE)
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->err = GetLastError();
		return reinterpret_cast<INT_PTR>(hFile);
	}
	case fdintCLOSE_FILE_INFO:
	{
		bool ret = true;
		if (reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pfn)
		{
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.pCurrentFile = pfdin->psz1;
			++reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.ulComplitedFiles;
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.hFile = reinterpret_cast<OutFile*>(pfdin->hf)->UserHandle ? reinterpret_cast<HANDLE>(reinterpret_cast<OutFile*>(pfdin->hf)->hFile) : nullptr;
			ret = reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pfn(PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE, { &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info }, reinterpret_cast<ExpansionInfo*>(pfdin->pv)->pv);
			if (reinterpret_cast<OutFile*>(pfdin->hf)->UserHandle)
			{
				delete reinterpret_cast<OutFile*>(pfdin->hf);
				if (!ret)
					reinterpret_cast<ExpansionInfo*>(pfdin->pv)->err = ERROR_CANCELLED;
				return ret;
			}
		}
		SetFileTime(reinterpret_cast<OutFile*>(pfdin->hf)->hFile, &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.FileTime, &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.FileTime, &reinterpret_cast<ExpansionInfo*>(pfdin->pv)->info.FileTime);
		CloseHandle(reinterpret_cast<OutFile*>(pfdin->hf)->hFile);
		delete reinterpret_cast<OutFile*>(pfdin->hf);
		if (!ret)
			reinterpret_cast<ExpansionInfo*>(pfdin->pv)->err = ERROR_CANCELLED;
		return ret;
	}
	default:
		return 0;
	}
}

PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_util_ExpandCabinet(HANDLE hCabinet, PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfnProgressCallback, PVOID pvUserData)
{
	CHAR null = '\0';
	WaitForSingleObject(hCabinet->hEvent,INFINITE);

	CHAR hFilesAddr[sizeof(void*) * 2 + 1];
	{
		hFilesAddr[sizeof(void*) * 2] = '\0';
		auto ulptr = reinterpret_cast<ULONG_PTR>(&hCabinet->hFile);
		for (int i = 0; i != sizeof(void*) * 2; hFilesAddr[i++] = '0');
		for (int i = 0; ulptr; ulptr /= 16, ++i)
		{
			CHAR r = static_cast<CHAR>(ulptr % 16);
			if (r >= 0 && r <= 9)
				hFilesAddr[sizeof(void*) * 2 - 1 - i] = '0' + r;
			else
				hFilesAddr[sizeof(void*) * 2 - 1 - i] = 'a' + r - 10;
		}
	}

	ExpansionInfo info;
	info.pfn = pfnProgressCallback;
	info.info.ulTotalFiles = hCabinet->wTotal;
	info.info.ulComplitedFiles = 0;
	info.pv = pvUserData;
	bool ret = FDICopy(hCabinet->hFDI, hFilesAddr, &null, 0, fnFDINotify, nullptr, &info);
	if (info.err == ERROR_SUCCESS)
		info.err = FDIErrToWindowsError(hCabinet);

	SetEvent(hCabinet->hEvent);
	SetLastError(info.err);
	return ret;
}

PSFEXTRACTIONHANDLER_API
VOID PSFExtHandler_util_CloseCabinet(HANDLE hCabinet)
{
	WaitForMultipleObjects(2, &hCabinet->hGlobalEvent, TRUE, INFINITE);

	if (hCabinet->GetRefCount() == 1)
	{
		CloseHandle(hCabinet->hFile.hFile);
		CloseHandle(hCabinet->hFile.hEvent);
		CloseHandle(hCabinet->hGlobalEvent);
	}
	else
		SetEvent(hCabinet->hGlobalEvent);

	FDIDestroy(hCabinet->hFDI);
	CloseHandle(hCabinet->hEvent);
	delete hCabinet;
}

PSFEXTRACTIONHANDLER_API
HANDLE PSFExtHandler_util_CabinetCopyHandle(HANDLE hCabinet)
{
	WaitForSingleObject(hCabinet->hGlobalEvent, INFINITE);
	HANDLE hNew = new Cab(*hCabinet);
	SetEvent(hCabinet->hGlobalEvent);
	return hNew;
}
