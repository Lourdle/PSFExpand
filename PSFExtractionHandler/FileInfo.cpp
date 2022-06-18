#include "pch.h"
#include "framework.h"
#include "PSFExtHandlerFrame.h"


DWORD
PSFEXTRACTIONHANDLER_API
PSFExtHandler_GetLongestFileNameLength(HPSF hpsf)
{
	CheckHandle(hpsf, return 0);
		
	SetLastError(ERROR_SUCCESS);

	DWORD Length = 0;
	for (LONG i = 0; i != hpsf->FileCount; ++i)
	{
		DWORD len = static_cast<DWORD>(hpsf->Files.get()[i].name.size());
		if (len > Length)
			Length = len;
	}
	return Length;
}

DWORD
PSFEXTRACTIONHANDLER_API
PSFExtHandler_GetFileCount(HPSF hpsf)
{
	CheckHandle(hpsf, return 0);

	SetLastError(ERROR_SUCCESS);
	return hpsf->FileCount;
}

BOOL
PSFEXTRACTIONHANDLER_API
PSFExtHandler_GetFileInfo(
	HPSF hpsf,
	DWORD i,
	PWSTR name,
	PDWORD size,
	PDWORD fsize,
	PFILETIME time,
	PSFEXTHANDLER_FILE_TYPE* type
)
{
	CheckHandle(hpsf, return FALSE);

	if (i >= hpsf->FileCount
		|| name && !size)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const auto& File = hpsf->Files.get()[i];
	if (type)
		switch (File.deltaSource.type)
		{
		case DELTA_FLAG_NONE:
			*type = PSFEXTHANDLER_FILE_TYPE_PA30;
			break;
		case DELTA_APPLY_FLAG_ALLOW_PA19:
			*type = PSFEXTHANDLER_FILE_TYPE_PA19;
			break;
		case INVALID_FLAG - 1:
			*type = PSFEXTHANDLER_FILE_TYPE_RAW;
			break;
		case INVALID_FLAG:
			*type = PSFEXTHANDLER_FILE_TYPE_UNKNOWN;
			break;
		}

	if (fsize)
		*fsize = File.deltaSource.length;

	if (time)
		*time = File.time;

	SetLastError(ERROR_SUCCESS);

	if (!size)
		return TRUE;

	auto Size = *size;
	*size = 2 * (static_cast<DWORD>(File.name.size()) + 1);

	if (!name)
		return TRUE;

	if (Size < *size)
	{
		SetLastError(ERROR_MORE_DATA);
		return FALSE;
	}
	else
		wcscpy_s(name, Size / 2, File.name.c_str());

	return TRUE;
}
