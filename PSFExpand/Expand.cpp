#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>

using namespace std;

static inline wstring MakePsfPathString(PCWSTR pCab, PCWSTR pCabFile)
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
		for (int i = 0; i < 3; ++i)
			Psf.rbegin()[i] = L"psf"[i];

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

bool Expand(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOut, BYTE Flags)
{
	PWSTR pCab = nullptr;
	unique_ptr<WCHAR[]> CabFile;
	if(pCabFile)
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

		if(!CreateDirectoryW(pOut,nullptr))
			return false;
	}

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
		CmdLine+=L" -f:* ";
		CmdLine += '\"';
		CmdLine += pOut;
		CmdLine += '\"';
		PROCESS_INFORMATION pi;
		if (!CreateProcessW(nullptr,
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
