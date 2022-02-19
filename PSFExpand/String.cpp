#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"

using namespace std;

unique_ptr<WCHAR[]> GetString(UINT id)
{
	PCWSTR str;
	auto len = LoadStringW(nullptr, id, reinterpret_cast<LPWSTR>(&str), 0);

	PWSTR String = new WCHAR[len + 1];
	wcsncpy_s(String, len + 1, str, len);
	return unique_ptr<WCHAR[]>(String);
}

PWSTR GetSubstringFromArgString(PWSTR pString, UINT ArgLen)
{
	if ((pString[ArgLen] == ':' || pString[ArgLen] == '=')
		&& pString[ArgLen + 1] != '\0')
		return pString + ArgLen + 1;
	else if (pString[ArgLen] == '\0' || pString[ArgLen + 1] == '\0')
		return reinterpret_cast<PWSTR>(-1);
	else
		return nullptr;
}

UINT CompareStrings(PCWSTR pString, initializer_list<PCWSTR>StringList, initializer_list<UINT> comparehead)
{
	for (auto i = StringList.begin(); i != StringList.end(); ++i)
	{
		auto wcsicmp = _wcsicmp;
		auto j = i - StringList.begin();
		for (auto i : comparehead)
			if (i == j)
			{
				wcsicmp = [](PCWSTR s1, PCWSTR s2)
				{
					return _wcsnicmp(s1, s2, wcslen(s2));
				};
				break;
			}
		if (!wcsicmp(pString, *i))
			return static_cast<UINT>(j);
	}
	return -1;
}
