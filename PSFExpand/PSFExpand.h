#pragma once

#ifndef PSFEXPAND_H
#define PSFEXPAND_H

#include <Windows.h>

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


struct _out;

class String
{
	friend String GetString(UINT);
	friend struct _out;
public:
	String(String&&);
	String() = default;
	String(const String&) = default;
	~String();

	PCWSTR get();
private:
	PCWSTR pString;
	ULONG Length;
	bool bNeedToFree;
};

String GetString(UINT);

static struct _out
{
	_out& operator<<(const ::std::wstring& StdString)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), StdString.c_str(), static_cast<DWORD>(StdString.size()), nullptr, nullptr);
		return *this;
	}

	_out& operator<<(WCHAR Char)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &Char, 1, nullptr, nullptr);
		return *this;
	}

	_out& operator<<(CHAR Char)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &Char, 1, nullptr, nullptr);
		return *this;
	}

	_out& operator<<(PCWSTR pString)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), pString, static_cast<DWORD>(wcslen(pString)), nullptr, nullptr);
		return *this;
	}

	_out& operator<<(PCSTR pString)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), pString, static_cast<DWORD>(strlen(pString)), nullptr, nullptr);
		return *this;
	}

	_out& operator<<(const String& Str)
	{
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), Str.pString, Str.Length, nullptr, nullptr);
		return *this;
	}

	template<typename T>
	_out& operator<<(T Value)
	{
		::std::cout << Value << ::std::flush;
		return *this;
	}
}out;

#endif // !PSFEXPAND_H
