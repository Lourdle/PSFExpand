#include "pch.h"
#include "framework.h"
#include "PSFExtHandlerFrame.h"

using namespace std;


PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_ExtractFile(
	HPSF hPSF,
	PCWSTR target,
	PCWSTR out,
	DWORD flags)
{
	CheckHandle(hPSF, return FALSE);

	if (!hPSF->hPSF)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	for (DWORD i = 0; i != hPSF->FileCount; ++i)
		if (!_wcsicmp(target, hPSF->Files[i].name.c_str()))
			return PSFExtHandler_ExtractFileByIndex(hPSF - 1, i, out, flags);

	SetLastError(ERROR_FILE_NOT_FOUND);
	return FALSE;
}


PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_ExtractFileByIndex(
	HPSF hPSF,
	DWORD index,
	PCWSTR out,
	DWORD flags
)
{
	CheckHandle(hPSF, return FALSE);
	WaitForSingleObject(hPSF->hEvent, INFINITE);

	if (!hPSF->hPSF)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE)
		CheckHandle(hPSF, return FALSE);

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS
		&& flags & PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS
		|| index >= hPSF->FileCount)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const auto& File = hPSF->Files[index];
	DWORD BaseError = ERROR_SUCCESS;
	
	HANDLE hFile = AutoCreateFile(File.name.c_str(), out, flags);
	if (!hFile)
		return FALSE;

	PVOID Data = new BYTE[hPSF->Files[index].deltaSource.length];
	WaitForSingleObject(hPSF->hFileEvent, INFINITE);
	if (!Read(hPSF->hPSF, Data, hPSF->Files[index], BaseError))
	{
		delete[] Data;
		SetEvent(hPSF->hFileEvent);
		SetLastError(BaseError);
		return FALSE;
	}
	SetEvent(hPSF->hFileEvent);
	bool ret = Write(Data, hFile, hPSF->Files[index], flags, BaseError);
	delete[] Data;

	if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE))
		CloseHandle(hFile);

	SetEvent(hPSF->hEvent);
	SetLastError(BaseError);
	if (!ret)
		return FALSE;
	else
		return TRUE;
}

PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_ExtractFileToDirectory(
	HPSF hPSF,
	PCWSTR target,
	PCWSTR outdir,
	PCWSTR out,
	DWORD flags)
{
	CheckHandle(hPSF, return FALSE);

	for (DWORD i = 0; i != hPSF->FileCount; ++i)
		if (!_wcsicmp(target, hPSF->Files[i].name.c_str()))
			return PSFExtHandler_ExtractFileToDirectoryByIndex(hPSF - 1, i, outdir, out, flags);

	SetLastError(ERROR_FILE_NOT_FOUND);
	return FALSE;
}

PSFEXTRACTIONHANDLER_API
BOOL
PSFExtHandler_ExtractFileToDirectoryByIndex(
	HPSF hPSF,
	DWORD index,
	PCWSTR outdir,
	PCWSTR out,
	DWORD flags
)
{
	CheckHandle(hPSF, return FALSE);

	if (!hPSF->hPSF)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	wstring file;
	if (outdir)
	{
		file = outdir;
		file += '\\';
	}
	file += out ? out : hPSF->Files[index].name;

	return PSFExtHandler_ExtractFileByIndex(hPSF - 1, index, file.c_str(), flags);
}
