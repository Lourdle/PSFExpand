#ifndef PSFEXTHANDLERFRAM_H
#define PSFEXTHANDLERFRAM_H

#include <msdelta.h>
#define INVALID_FLAG (-1)

#ifndef dllimport
#define dllimport dllexport
#endif
#include "PSFExtractionHandler.h"

struct FileInfo
{
	std::wstring name;
	FILETIME time;
	struct Source
	{
		DELTA_FLAG_TYPE type;
		ULONG offset;
		ULONG length;
		struct
		{
			ALG_ID alg;
			std::unique_ptr<BYTE[]> value;
		}Hash;
	}deltaSource;
};

using FileList = FileInfo*;

class RefCountMT
{
public:
	RefCountMT() : pulCount(new ULONG(1))
	{}
	RefCountMT(const RefCountMT& ref) : pulCount(ref.pulCount)
	{
		++* pulCount;
	}
	~RefCountMT()
	{
		if (-- * pulCount == 0)
			delete pulCount;
	}

	ULONG GetRefCount()
	{
		return *pulCount;
	}

private:
	ULONG* pulCount;
};

struct PSF : RefCountMT
{
	PSF() = default;
	PSF(const PSF& ref) : RefCountMT(ref),
		hGlobalEvent(ref.hGlobalEvent), hEvent(CreateEventW(nullptr, FALSE, TRUE, nullptr)), hPSF(ref.hPSF), hFileEvent(ref.hFileEvent), FileCount(ref.FileCount), Files(ref.Files)
	{}

	HANDLE hGlobalEvent;
	HANDLE hEvent;

	HANDLE hPSF;
	HANDLE hFileEvent;

	DWORD FileCount;
	FileList Files;
};


#define CheckHandle(hObject, Expression)\
if (hObject == nullptr\
	|| hObject == INVALID_HANDLE_VALUE)\
{\
	SetLastError(ERROR_INVALID_HANDLE);\
	Expression;\
}\
else\
	++hObject;


HANDLE AutoCreateFile(
	PCWSTR pFile,
	PCWSTR pOutFile,
	DWORD dwFlags
);

bool Read(
	HANDLE hPSF,
	PVOID pBuffer,
	const FileInfo& FileInfo,
	DWORD& Error
);

DWORD Write(
	PVOID Buffer,
	HANDLE hFile,
	const FileInfo& FileInfo,
	DWORD dwFlags,
	DWORD& Error
);


#include <omp.h>

inline
DWORD AssignThreadTask(DWORD dwTaskCount, DWORD dwCurrentThread, DWORD& dwRange)
{
	int n = omp_get_num_procs();

	dwRange = dwTaskCount / n;
	DWORD remainder = dwTaskCount % n;
	DWORD start = dwRange * dwCurrentThread;

	if (remainder > dwCurrentThread)
		++dwRange;

	if (remainder != 0)
		start += remainder > dwCurrentThread ? dwCurrentThread : remainder;

	return start;
}

class Event
{
	HANDLE hEvent;
public:
	Event() = delete;
	Event(const Event&&) = delete;
	Event(Event&&) = delete;
	Event(HANDLE hEvent) : hEvent(hEvent)
	{
		WaitForSingleObject(hEvent, INFINITE);
	}
	~Event()
	{
		DWORD dwError = GetLastError();
		SetEvent(hEvent);
		SetLastError(dwError);
	}
};

#endif // !PSFEXTHANDLERFRAM_H
