#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <vector>
#include <iostream>
using namespace std;

bool Screen(wstring String, PCWSTR pScreener)
{
	wstring Screener = pScreener;
	for (auto& i : Screener)
		i = tolower(i);
	for (auto& i : String)
		i = tolower(i);

	size_t last = 0;
	vector<wstring> vStrings;
	vector<bool> v;
	for (size_t i = 0; i != Screener.size(); ++i)
		if (Screener[i] == '*' || Screener[i] == '?')
		{
			vStrings.push_back(Screener.substr(last, i - last));
			last = i + 1;
			switch (Screener[i])
			{
			case '*':
				v.push_back(false);
				break;
			case '?':
				v.push_back(true);
				break;
			}
		}
	vStrings.push_back(Screener.substr(last, Screener.size()));

	size_t lastpos = 0;
	for (size_t i = 0; i != vStrings.size(); ++i)
	{
		auto pos = String.find(vStrings[i], lastpos);
		if (pos == wstring::npos
			|| i == 0 && pos != 0
			|| i != 0 && v[i - 1] == true && lastpos + vStrings[i].size() != pos)
			return false;

		lastpos = pos;
		if (i != vStrings.size() - 1)
			lastpos += v[i] ? 1 : 0;
		else if (lastpos + vStrings[i].size() != String.size() && vStrings[i].size())
			return false;
	}

	return true;
}

bool List(PCWSTR pXml, bool DisplayDetail, const PWSTR* Screeners, int nScreenerCount)
{
	PreProcessScreeners(Screeners, nScreenerCount);

	HPSF hPSF = PSFExtHandler_OpenFile(nullptr, pXml);
	if (!hPSF)
		return false;

	DWORD nFileCount = PSFExtHandler_GetFileCount(hPSF);
	wprintf(GetString(File_Count).get(), nFileCount);

	BOOL Ret = 0;

	DWORD n = 0;

	for (DWORD i = 0; i != nFileCount; ++i)
	{
		DWORD strSize;
		DWORD FileSize;
		PSFEXTHANDLER_FILE_TYPE type;

		Ret = PSFExtHandler_GetFileInfo(hPSF, i, nullptr, &strSize, &FileSize, &type);
		if (!Ret)
			break;

		wstring File;
		File.resize(strSize / 2 - 1);
		Ret = PSFExtHandler_GetFileInfo(hPSF, i, const_cast<PWSTR>(File.c_str()), &strSize, &FileSize, &type);
		if (!Ret)
			break;

		if (Screeners)
		{
			bool ret = true;
			for (int j = 0; j != nScreenerCount; ++j)
				if (Screen(File, Screeners[j]))
					ret = true;
				else
				{
					ret = false;
					break;
				}
			if (!ret)
				continue;
		}
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

	if (Screeners && Ret)
		wprintf(GetString(Satified_File_Count).get(), n);

	return Ret;
}
