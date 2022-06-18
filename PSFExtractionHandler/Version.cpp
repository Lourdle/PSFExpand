#include "pch.h"
#include "PSFExtHandlerFrame.h"

DWORD
PSFEXTRACTIONHANDLER_API
PSFExtHandler_GetVersion()
{
	return PSFEXTHANDLER_CURRENT_VERSION;
}

PCWSTR
PSFEXTRACTIONHANDLER_API
PSFExtHandler_GetVersionString()
{
	return L"2.0.1";
}
