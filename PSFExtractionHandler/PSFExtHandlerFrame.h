#ifndef PSFEXTHANDLERFRAM_H
#define PSFEXTHANDLERFRAM_H

#include <msdelta.h>
#define INVALID_FLAG (-1)

struct FileInfo
{
	std::wstring name;
	//DWORD time;
	struct Source
	{
		DELTA_FLAG_TYPE type;
		DWORD offset;
		DWORD length;
		struct
		{
			ALG_ID alg;
			std::unique_ptr<BYTE[]> value;
		}Hash;
	}deltaSource;
};

using FileList = std::unique_ptr<FileInfo[]>;

struct PSF
{
	HANDLE hPSF = nullptr;
	std::wstring PSF;

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


bool Extract(
	HANDLE hPSF,
	HANDLE hFile,
	const FileInfo::Source* src,
	WORD flags,
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

#define dllimport dllexport
#include "PSFExtractionHandler.h"


#endif // !PSFEXTHANDLERFRAM_H
