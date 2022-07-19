#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>
#include <string>
using namespace std;

#include <Shlwapi.h>

bool Extract(PCWSTR pXml, PCWSTR pPsf, PWSTR pFile, PCWSTR pDestination, bool Verify, bool Verbose)
{
	PreProcessScreeners(&pFile, 1);

	unique_ptr<PSF, void(*)(HPSF)> hPSF(PSFExtHandler_OpenFile(wcsncmp(pPsf, L"\\\\", 2) == 0 ? pPsf : (wstring(L"\\\\?\\") + pPsf).c_str(), pXml),
		[](HPSF hPSF)
		{
			DWORD Err = GetLastError();
			PSFExtHandler_ClosePSF(hPSF);
			SetLastError(Err);
		}
	);
	if (!hPSF)
		return false;

	DWORD nFileCount = PSFExtHandler_GetFileCount(hPSF.get());

	wcout << GetString(Extracting);

	DWORD n = 0;
	for (DWORD i = 0; i != nFileCount; ++i)
	{
		DWORD strSize;
		PSFEXTHANDLER_FILE_TYPE type;
		if (!PSFExtHandler_GetFileInfo(hPSF.get(), i, nullptr, &strSize, nullptr, nullptr, &type, nullptr))
			break;

		wstring File;
		File.resize(strSize / 2 - 1);
		if (!PSFExtHandler_GetFileInfo(hPSF.get(), i, const_cast<PWSTR>(File.c_str()), &strSize, nullptr, nullptr, &type, nullptr))
			break;

		if (PathMatchSpecW(File.c_str(), pFile))
			if (!PSFExtHandler_ExtractFileToDirectoryByIndex(hPSF.get(),
				i,
				pDestination, nullptr,
				PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS | (Verify ? PSFEXTHANDLER_EXTRACT_FLAG_VERIFY : 0)))
				return false;
			else
			{
				if (Verbose)
					wcout << File << '\n';
				++n;
			}
	}

	cout << '\n';
	wprintf(GetString(Extracted_File_Count).get(), n);
	return true;
}
