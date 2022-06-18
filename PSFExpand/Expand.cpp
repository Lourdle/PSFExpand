#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>

using namespace std;

#include <Shlwapi.h>

inline
static bool AccessFile(PCWSTR pFile)
{
	HANDLE hFile = CreateFileW(pFile,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	else
		return false;
}

inline
static bool IsAnEmptyDirectory(wstring Dir)
{
	Dir += L"\\*.*";

	WIN32_FIND_DATAW wfd;
	HANDLE hFindFile = FindFirstFileW(Dir.c_str(), &wfd);

	if (!hFindFile)
		return false;

	FindNextFileW(hFindFile, &wfd);
	BOOL ret = FindNextFileW(hFindFile, &wfd);
	FindClose(hFindFile);

	return !ret;
}

inline
static bool IsUNCName(PCWSTR pPathName)
{
	if (wcslen(pPathName) >= 2)
		if (pPathName[0] == '\\' && pPathName[1] == '\\')
			return true;
		else
			return false;
	else
		return false;
}

static wstring FullPathName(PCWSTR pPathName)
{
	DWORD Length = GetFullPathNameW(pPathName, 0, nullptr, nullptr);
	if (!Length)
		return L"";

	wstring Name;
	Name.resize(Length - 1);

	GetFullPathNameW(pPathName, Length, const_cast<LPWSTR>(Name.c_str()), nullptr);
	return Name;
}

static wstring ShortPathName(PCWSTR pPathName)
{
	wstring FullName = FullPathName(pPathName);
	if (FullName == L"")
		return L"";
	else
		pPathName = FullName.c_str();

	wstring Name = L"\\\\?\\";
	DWORD Length;
	PCWSTR pLongName;
	bool unc = false;

	if (wcsncmp(pPathName, L"\\\\?\\", 4))
	{
		if (IsUNCName(pPathName))
		{
			Name += L"UNC\\";
			Name += pPathName;
			unc = true;
		}
		else
			Name += pPathName;

		pLongName = Name.c_str();
	}
	else
		pLongName = pPathName;

	wstring ShortName;
	Length = GetShortPathNameW(pLongName, nullptr, 0);
	if (!Length)
		return L"";
	ShortName.resize(Length - 1);
	GetShortPathNameW(pLongName, const_cast<LPWSTR>(ShortName.c_str()), Length);
	ShortName.erase(ShortName.cbegin(), ShortName.cbegin() + (unc ? 8 : 4));
	return ShortName;
}

static wstring LongPathName(PCWSTR pPathName)
{
	wstring FullName = FullPathName(pPathName);
	bool unc = false;
	if (FullName == L"")
		return L"";
	else if (wcsncmp(FullName.c_str(), L"\\\\?\\", 4))
	{
		wstring tmp = L"\\\\?\\";
		if (IsUNCName(pPathName))
		{
			tmp += L"UNC\\";
			unc = true;
		}

		tmp.swap(FullName);
		FullName += tmp;
	}
	pPathName = FullName.c_str();
	DWORD Length;
	Length = GetLongPathNameW(pPathName, nullptr, 0);
	if (!Length)
		return L"";

	wstring Name;
	Name.resize(Length - 1);

	GetLongPathNameW(pPathName, const_cast<LPWSTR>(Name.c_str()), Length);
	Name.erase(Name.cbegin(), Name.cbegin() + (unc ? 8 : 4));
	return Name;
}

static wstring MakePsfPathString(PCWSTR pCabFile)
{
	wstring Psf;
	Psf = pCabFile;
	PCWSTR pCab = pCabFile + wcslen(pCabFile);
	for (; pCab; pCab--)
		if (*pCab == '\\')
		{
			++pCab;
			break;
		}

	if (PathMatchSpecW(pCab, L"Windows*.*-KB???????-*_*.cab"))
	{
		Psf.erase(pCab - pCabFile + Psf.cbegin(), Psf.cend());

		wstring File(pCabFile);
		auto pos = File.find('_') + 1;
		File.erase(File.cbegin() + pos, File.cend());
		File += L"*.psf";

		WIN32_FIND_DATAW wfd;
		HANDLE hFindFile = FindFirstFileW(File.c_str(), &wfd);

		DWORD Err = GetLastError();
		FindClose(hFindFile);
		SetLastError(Err);

		if (hFindFile == INVALID_HANDLE_VALUE)
			return L"";

		Psf += wfd.cFileName;
	}
	else
	{
		if (_wcsicmp(Psf.c_str() + Psf.size() - 4, L".cab"))
			SetLastError(ERROR_BAD_FORMAT);

		for (int i = 0; i < 3; ++i)
			Psf.begin()[i + Psf.size() - 3] = L"psf"[i];

		WIN32_FIND_DATAW wfd;
		HANDLE hFindFile = FindFirstFileW(Psf.c_str(), &wfd);

		DWORD Err = GetLastError();
		FindClose(hFindFile);
		SetLastError(Err);

		if (hFindFile == INVALID_HANDLE_VALUE)
			return L"";
	}

	return Psf;
}

inline
static bool PrintFileInfo(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOutDir)
{
	wstring Psf;
	if (!pPsfFile)
	{
		Psf = MakePsfPathString(pCabFile);
		pPsfFile = Psf.c_str();
	}
	wcout << L"PSF" << ' ' << GetString(FilePath) << '\n' << LongPathName(pPsfFile) << '\n';
	if (!AccessFile(pPsfFile))
		return false;

	wcout << L"XML" << ' ' << GetString(FilePath) << '\n';
	if (!pXmlFile)
		wcout << LongPathName(pOutDir) << L"\\express.psf.cix.xml" << '\n';
	else
	{
		wcout << pXmlFile << '\n';
		if (!AccessFile(pXmlFile))
			return false;
	}
	cout << '\n';

	return true;
}

bool Expand(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOut, BYTE Flags)
{
	wstring CabFile;
	if (pCabFile)
	{
		if (!AccessFile(pCabFile))
			return false;

		if (_wcsicmp(pCabFile + wcslen(pCabFile) - 4, L".cab"))
		{
			SetLastError(ERROR_BAD_FORMAT);
			return false;
		}

		CabFile = ShortPathName(pCabFile);
		if (CabFile == L"")
			return false;
		pCabFile = CabFile.c_str();

		wcout << L"CAB" << ' ' << GetString(FilePath) << '\n' << LongPathName(pCabFile) << '\n';
	}

	wstring OutDir;
	if (!pOut)
	{
		OutDir = LongPathName(CabFile.c_str());
		OutDir.erase(OutDir.cend() - 4, OutDir.cend());
	}
	else
	{
		OutDir = FullPathName(pOut);
		if (OutDir == L"")
			return false;
	}
	pOut = OutDir.c_str();

	if (pCabFile)
	{
		auto Out = wstring(L"\\\\?\\") + (IsUNCName(pOut) ? L"UNC\\" : L"");
		Out += pOut;

		if (!CreateDirectoryW(
			Out.c_str(),
			nullptr))
			if (GetLastError() != ERROR_ALREADY_EXISTS
				|| !IsAnEmptyDirectory(Out.c_str()))
				return false;
	}

	if (!PrintFileInfo(pCabFile, pPsfFile, pXmlFile, pOut))
		return false;

	if (pCabFile)
	{
		wprintf(GetString(Expanding).get(), L"CAB");

		HANDLE hCab = PSFExtHandler_util_OpenCabinet(pCabFile);
		WORD n = PSFExtHandler_util_CabinetGetFileCount(hCab);
		wprintf(GetString(File_Count).get(), L"CAB", n);
		SetCurrentDirectoryW(OutDir.c_str());
		PSFExtHandler_util_ExpandCabinet(hCab,
			[](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE State, const PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS* info, PHANDLE, PVOID)
			{
				if (State == State_CloseFile)
					cout << '\r' << static_cast<DWORD>(info->wComplitedFiles * 100) / info->wTotalFiles << '%';
			}, nullptr);
		PSFExtHandler_util_CloseCabinet(hCab);
		cout << '\n' << endl;
	}

	HPSF hPSF;
	wstring XmlFile;
	if (!pXmlFile)
	{
		XmlFile = LongPathName(pOut);
		if (XmlFile == L"")
			return false;
		XmlFile += L"\\express.psf.cix.xml";
		pXmlFile = XmlFile.c_str();
	}
	if (!pPsfFile)
	{
		auto Psf = MakePsfPathString(CabFile.c_str());
		if (Psf == L"")
			return false;

		hPSF = PSFExtHandler_OpenFile(Psf.c_str(), pXmlFile);
	}
	else
		hPSF = PSFExtHandler_OpenFile(pPsfFile, pXmlFile);

	if (!hPSF)
		return false;

	DWORD n = PSFExtHandler_GetFileCount(hPSF);

	wprintf(GetString(Expanding).get(), L"PSF");
	wprintf(GetString(File_Count).get(), L"PSF", n);
	BYTE Progress = 0;

	BOOL ret = PSFExtHandler_ExpandPSF(hPSF, pOut,
		PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS
		| PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD
		| PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
		| (Flags & FLAG_ARG_EXPAND_SINGLETHREAD ? PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD : 0)
		| (Flags & FLAG_ARG_EXPAND_VERIFY ? PSFEXTHANDLER_EXTRACT_FLAG_VERIFY : 0),
		Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? reinterpret_cast<PSFEXTHANDLER_PROGRESS_PROC>(NULL) :
		[](const PSFEXTHANDLER_EXPAND_INFO* iep, PVOID pProgress)
		{
			BYTE Progress = static_cast<BYTE>(static_cast<ULONGLONG>(iep->dwCompletedBytes) * 100 / iep->dwTotalBytes);

			if (Progress != *reinterpret_cast<PBYTE>(pProgress))
			{
				*reinterpret_cast<PBYTE>(pProgress) = Progress;
				cout << '\r' << static_cast<int>(Progress) << '%';
			}

			return TRUE;
		}
	, &Progress);
	cout << '\n';

	PSFExtHandler_ClosePSF(hPSF);

	return ret;
}
