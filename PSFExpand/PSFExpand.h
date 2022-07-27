#pragma once

#ifndef PSFEXPAND_H
#define PSFEXPAND_H

#include <Windows.h>

#include <memory>
#include <initializer_list>
#include <string>
#include <vector>
#include <iostream>

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


static struct _out
{
	_out& operator<<(const ::std::wstring& String)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), String.c_str(), static_cast<DWORD>(String.size()), nullptr, nullptr);
		return *this;
	}

	_out& operator<<(WCHAR Char)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &Char, 1, nullptr, nullptr);
		return *this;
	}

	_out& operator<<(CHAR Char)
	{
		return *this << static_cast<WCHAR>(Char);
	}

	_out& operator<<(PCWSTR pString)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), pString, static_cast<DWORD>(wcslen(pString)), nullptr, nullptr);
		return *this;
	}

	template<typename T>
	_out& operator<<(T Value)
	{
		::std::wcout << Value;
		return *this;
	}
}out;

#endif // !PSFEXPAND_H
