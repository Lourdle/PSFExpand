#pragma once

#ifndef PSFEXPAND_H
#define PSFEXPAND_H

#include <Windows.h>

#include <memory>
#include <initializer_list>
#include <string>

std::unique_ptr<WCHAR[]> GetString(UINT);

PWSTR GetSubstringFromArgString(PWSTR pString, UINT ArgLen);

UINT CompareStrings(PCWSTR pString, std::initializer_list<PCWSTR> StringList, std::initializer_list<UINT> StringIndexForComparingHeadOnly);

bool List(PCWSTR pXml, bool DisplayDetail, const PWSTR* Screeners, int nScreenerCount);

inline
void PreProcessScreeners(const PWSTR* Screeners, int nScreenerCount)
{
	for (int i = 0; i != nScreenerCount; ++i)
		for (auto j = wcslen(Screeners[i]); j; j--)
			if (Screeners[i][j] == '/')
				Screeners[i][j] = '\\';
}
bool Screen(std::wstring String, PCWSTR pScreener);

bool Extract(PCWSTR pXml, PCWSTR pPsf, PWSTR pFile, PCWSTR pDestination, bool Verify, bool Verbose);

#endif // !PSFEXPAND_H
