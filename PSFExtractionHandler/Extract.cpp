#include "pch.h"
#include "framework.h"
#include "PSFExtHandlerFrame.h"

using namespace std;


PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_ExtractFile(
	HPSF hPSF,
	PCWSTR target,
	PCWSTR out,
	WORD flags)
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

static bool CreateDirectoryWhereTheFileLocated(wstring file)
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

PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_ExtractFileByIndex(
	HPSF hPSF,
	DWORD index,
	PCWSTR out,
	WORD flags
)
{
	CheckHandle(hPSF, return FALSE);

	if (!hPSF->hPSF)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE)
		CheckHandle(hPSF, return FALSE);

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS
		&& flags & PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS
		|| flags >> 8
		|| index >= hPSF->FileCount)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const auto& File = hPSF->Files[index];
	DWORD BaseError = ERROR_SUCCESS;

	if (File.deltaSource.type == INVALID_FLAG)
	{
		BaseError = ERROR_NOT_SUPPORTED;
		if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT))
		{
			SetLastError(BaseError);
			return FALSE;
		}
	}

	if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS)
		&& File.deltaSource.Hash.alg == INVALID_FLAG)
	{
		SetLastError(ERROR_NOT_SUPPORTED);
		return FALSE;
	}

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
			out = File.name.c_str();

		if (wcsncmp(out, L"\\\\?\\", 4) == 0
			|| _wcsnicmp(out, L"\\\\UNC\\", 6) == 0)
			file = out;
		else if (wcsncmp(out, L"\\\\", 4) == 0)
		{
			file = L"\\\\UNC";
			file += out + 1;
		}
		else
		{
			DWORD len = GetFullPathNameW(out, 0, nullptr, nullptr);
			if (len == 0)
				return FALSE;
			file.resize(len + 3);
			GetFullPathNameW(out, len, const_cast<LPWSTR>(file.c_str() + 4), nullptr);
		}
		file.shrink_to_fit();

		if (!CreateDirectoryWhereTheFileLocated(file))
			return FALSE;

		hFile = CreateFileW(file.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			FileCreationDisposition,
			0,
			nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			if (GetLastError() == ERROR_FILE_EXISTS)
				if (flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS)
					return FALSE;
				else
					return TRUE;
			else
				return FALSE;
	}

	bool ret = Extract(hPSF->hPSF, hFile, &hPSF->Files[index].deltaSource, flags, BaseError);

	if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE))
		CloseHandle(hFile);

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
	WORD flags)
{
	CheckHandle(hPSF, return FALSE);

	if (!hPSF->hPSF)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

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
	WORD flags
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
