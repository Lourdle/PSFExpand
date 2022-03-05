#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>

using namespace std;

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

static wstring MakePsfPathString(PCWSTR pCab, PCWSTR pCabFile)
{
	wstring Psf;
	Psf = pCabFile;

	if (Screen(pCab, L"Windows*.*-KB???????-*_*.cab"))
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
bool PrintFileInfo(PCWSTR pCab, PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOutDir)
{
	if (pCabFile)
	{
		wcout << L"CAB" << ' ' << GetString(FilePath) << '\n' << pCabFile << '\n';
		if (!AccessFile(pCabFile))
			return false;
	}

	wstring Psf;
	if (!pPsfFile)
	{
		Psf = MakePsfPathString(pCab, pCabFile);
		pPsfFile = Psf.c_str();
	}
	wcout << L"PSF" << ' ' << GetString(FilePath) << '\n' << pPsfFile << '\n';
	if (!AccessFile(pPsfFile))
		return false;

	wcout << L"XML" << ' ' << GetString(FilePath) << '\n';
	if (!pXmlFile)
		wcout << pOutDir << L"\\express.psf.cix.xml" << '\n';
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
	PWSTR pCab = nullptr;
	unique_ptr<WCHAR[]> CabFile;
	if (pCabFile)
	{
		DWORD Length = GetFullPathNameW(pCabFile, 0, nullptr, nullptr);
		if (!Length)
			return false;

		PWSTR pFile = new WCHAR[Length];
		GetFullPathNameW(pCabFile, Length, pFile, &pCab);

		CabFile.reset(pFile);
	}

	wstring OutDir;
	if (!pOut)
	{
		OutDir = CabFile.get();
		OutDir.erase(OutDir.cbegin() + OutDir.size() - 4, OutDir.cend());

		pOut = OutDir.c_str();
	}
	if (pCabFile && !CreateDirectoryW(pOut, nullptr))
		if (GetLastError() != ERROR_ALREADY_EXISTS || !IsAnEmptyDirectory(OutDir))
			return false;

	if (!PrintFileInfo(pCab, CabFile.get(), pPsfFile, pXmlFile, pOut))
		return false;

	if (pCabFile)
	{
		wcout << GetString(Expanding_Cab);

		HANDLE hNul = CreateFileW(L"NUL",
			GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_WRITE_THROUGH,
			nullptr);

		STARTUPINFOW si = { sizeof(si) };
		si.hStdOutput = hNul;
		si.dwFlags = STARTF_USESTDHANDLES;

		wstring CmdLine(L"Expand.exe ");
		CmdLine += pCabFile;
		CmdLine += L" -f:* ";
		CmdLine += '\"';
		CmdLine += pOut;
		CmdLine += '\"';
		PROCESS_INFORMATION pi;
		if (!CreateProcessW(
			nullptr,
			const_cast<LPWSTR>(CmdLine.c_str()),
			nullptr,
			nullptr,
			TRUE,
			CREATE_NO_WINDOW,
			nullptr,
			nullptr,
			&si,
			&pi))
		{
			CloseHandle(hNul);
			return false;
		}

		CloseHandle(pi.hThread);

		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(hNul);

		DWORD ExitCode;
		GetExitCodeProcess(pi.hProcess, &ExitCode);
		CloseHandle(pi.hProcess);

		if (ExitCode != ERROR_SUCCESS)
		{
			SetLastError(ExitCode);
			return false;
		}
		else
			wcout << GetString(Done);

		cout << '\n';
	}

	HPSF hPSF;
	if (!pPsfFile)
	{
		auto Psf = MakePsfPathString(pCab, CabFile.get());
		if (Psf == L"")
			return false;

		hPSF = PSFExtHandler_OpenFile(Psf.c_str(), pXmlFile ? pXmlFile : (wstring(pOut) + L"\\express.psf.cix.xml").c_str());
	}
	else
		hPSF = PSFExtHandler_OpenFile(pPsfFile, pXmlFile ? pXmlFile : (wstring(pOut) + L"\\express.psf.cix.xml").c_str());

	if (!hPSF)
		return false;

	DWORD n = PSFExtHandler_GetFileCount(hPSF);

	wprintf(GetString(File_Count).get(), n);
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
			BYTE Progress = static_cast<BYTE>(iep->dwCompletedBytes / (iep->dwTotalBytes / 100));

			if (Progress != *reinterpret_cast<PBYTE>(pProgress))
			{
				*reinterpret_cast<PBYTE>(pProgress) = Progress;
				printf("\r%d%%", Progress);
			}

			return TRUE;
		}
	, &Progress);
	cout << '\n';

	PSFExtHandler_ClosePSF(hPSF);

	return ret;
}
