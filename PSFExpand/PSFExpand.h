#pragma once

#ifndef PSFEXPAND_H
#define PSFEXPAND_H

#include <Windows.h>

#include <memory>
#include <initializer_list>
#include <string>
#include <vector>

constexpr BYTE FLAG_ARG_LIST_DETAIL = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERBOSE = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXPAND_SINGLETHREAD = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_NOPROGRESSDISPLAY = 0x0004;

::std::unique_ptr<WCHAR[]> GetString(UINT);

PWSTR GetSubstringFromArgString(PWSTR pString, UINT ArgLen);

UINT CompareStrings(PCWSTR pString, ::std::initializer_list<PCWSTR> StringList, ::std::initializer_list<UINT> StringIndexForComparingHeadOnly = {});

bool List(PCWSTR pXml, bool DisplayDetail, ::std::vector<PWSTR>& Files);

inline
void PreProcessFileStrings(::std::vector<PWSTR>& Files)
{
	for (auto& i : Files)
		for (auto j = wcslen(i); j; j--)
			if (i[j] == '/')
				i[j] = '\\';
}

bool Extract(PCWSTR pXml, PCWSTR pPsf, ::std::vector<PWSTR>& Files, PCWSTR pDestination, bool Verify, bool Verbose);

bool Expand(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOutDir, BYTE Flags);

#endif // !PSFEXPAND_H
