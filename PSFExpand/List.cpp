#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <vector>
#include <iostream>
using namespace std;

#include <Shlwapi.h>

bool List(PCWSTR pXml, bool DisplayDetail, vector<PWSTR>& Files)
{
	PreProcessFileStrings(Files);

	HPSF hPSF = PSFExtHandler_OpenFile(nullptr, pXml);
	if (!hPSF)
		return false;

	DWORD nFileCount = PSFExtHandler_GetFileCount(hPSF);
	wprintf(GetString(File_Count).get(), L"PSF", nFileCount);

	BOOL Ret = 0;
	DWORD n = 0;

	for (DWORD i = 0; i != nFileCount; ++i)
	{
		DWORD strSize;
		DWORD FileSize;
		PSFEXTHANDLER_FILE_TYPE type;

		Ret = PSFExtHandler_GetFileInfo(hPSF, i, nullptr, &strSize, &FileSize, nullptr, &type, nullptr);
		if (!Ret)
			break;

		wstring File;
		File.resize(strSize / 2 - 1);
		Ret = PSFExtHandler_GetFileInfo(hPSF, i, const_cast<PWSTR>(File.c_str()), &strSize, &FileSize, nullptr, &type, nullptr);
		if (!Ret)
			break;

		if (!Files.empty())
		{
			for (const auto j : Files)
				if (PathMatchSpecW(File.c_str(), j))
					goto PrintFileInfo;
			continue;
		}

	PrintFileInfo:
		++n;
		wcout << File << '\n';
		if (DisplayDetail)
			wcout << GetString(File_Size) << FileSize << ' ' << 'B' << '\n' <<
			GetString(File_Type) <<
			(type == PSFEXTHANDLER_FILE_TYPE_PA30 ? L"PA30" :
				type == PSFEXTHANDLER_FILE_TYPE_PA19 ? L"PA19" :
				type == PSFEXTHANDLER_FILE_TYPE_RAW ? L"RAW" : L"UNKNOWN") << '\n'
			<< '\n';
	}

	DWORD Err = GetLastError();
	PSFExtHandler_ClosePSF(hPSF);
	SetLastError(Err);

	if (!Files.empty() && Ret)
	{
		cout << '\n';
		wprintf(GetString(Satified_File_Count).get(), n);
	}

	return Ret;
}
