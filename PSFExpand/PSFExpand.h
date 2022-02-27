#pragma once

#ifndef PSFEXPAND_H
#define PSFEXPAND_H

#include <Windows.h>

#include <memory>
#include <initializer_list>
#include <string>

constexpr BYTE FLAG_ARG_LIST_DETAIL = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERBOSE = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXPAND_SINGLETHREAD = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_NOPROGRESSDISPLAY = 0x0004;


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

bool Expand(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOutDir, BYTE Flags);

#endif // !PSFEXPAND_H
