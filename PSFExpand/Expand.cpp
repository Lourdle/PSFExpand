#include "pch.h"
#include <Windows.h>
#include "PSFExpand.h"
#include "resource.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>
#include <thread>
#include <mutex>

#include <omp.h>

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

	if (wcsncmp(pPathName, L"\\\\?\\", 4) != 0)
	{
		if (PathIsUNCW(pPathName))
		{
			Name += L"UNC";
			Name += pPathName + 1;
			return Name;
		}
		else
			Name += pPathName;

		pLongName = Name.c_str();
	}
	else
		pLongName = pPathName;
	if (PathIsUNCW(pLongName))
		return pLongName;


	wstring ShortName;
	Length = GetShortPathNameW(pLongName, nullptr, 0);
	if (!Length)
		return L"";
	ShortName.resize(Length - 1);
	GetShortPathNameW(pLongName, const_cast<LPWSTR>(ShortName.c_str()), Length);
	ShortName.erase(ShortName.cbegin(), ShortName.cbegin() + 4);
	return ShortName;
}

static wstring LongPathName(PCWSTR pPathName)
{
	wstring FullName = FullPathName(pPathName);
	if (FullName == L"")
		return L"";
	if (wcsncmp(FullName.c_str(), L"\\\\?\\", 4) != 0)
	{
		wstring tmp = L"\\\\?\\";
		if (PathIsUNCW(pPathName))
		{
			tmp += L"UNC";
			tmp += FullName.c_str() + 1;
			return tmp;
		}
		tmp.swap(FullName);
		FullName += tmp;
	}
	else if (PathIsUNCW(FullName.c_str()))
		return FullName;

	pPathName = FullName.c_str();
	DWORD Length;
	Length = GetLongPathNameW(pPathName, nullptr, 0);
	if (!Length)
		return L"";

	wstring Name;
	Name.resize(Length - 1);

	GetLongPathNameW(pPathName, const_cast<LPWSTR>(Name.c_str()), Length);
	Name.erase(Name.cbegin(), Name.cbegin() + 4);
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
	out << L"PSF" << ' ' << GetString(FilePath) << '\n' << LongPathName(pPsfFile) << '\n';
	if (!AccessFile(pPsfFile))
		return false;

	out << L"XML" << ' ' << GetString(FilePath) << '\n';
	if (!pXmlFile)
		out << LongPathName(pOutDir) << L"\\express.psf.cix.xml" << '\n';
	else
	{
		out << pXmlFile << '\n';
		if (!AccessFile(pXmlFile))
			return false;
	}
	out << '\n';

	return true;
}


struct ReportStruct
{
	int Progress;
	HANDLE hEvent;
	bool Exit;
};

static void ReportThread(ReportStruct* Data)
{
	while (!Data->Exit)
	{
		WaitForSingleObject(Data->hEvent, INFINITE);
		printf("\r%d%%", Data->Progress);
	}
}

struct CabExpansion
{
	ReportStruct* pReportStruct;
	ULONG start, end, n, & complited;
	bool& cancel;
	mutex& Mutex;
	int progress;
};

static BOOL CabExpansionCallback(PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE State, PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO info, CabExpansion* Data)
{
	if (Data->start > Data->n)
		*info.phFile = INVALID_HANDLE_VALUE;
	else if (Data->n >= Data->end
		|| Data->cancel)
		return FALSE;
	else if (State == PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE)
	{
		Data->Mutex.lock();
		++Data->complited;
		Data->Mutex.unlock();
		Data->progress = static_cast<int>(static_cast<DWORD>(Data->complited * 100) / info.pProgressInfo->ulTotalFiles);
		if (Data->pReportStruct->Progress < Data->progress)
		{
			Data->Mutex.lock();
			Data->pReportStruct->Progress = Data->progress;
			Data->Mutex.unlock();
			SetEvent(Data->pReportStruct->hEvent);
		}
		return TRUE;
	}
	++Data->n;
	return TRUE;
}

bool Expand(PCWSTR pCabFile, PCWSTR pPsfFile, PCWSTR pXmlFile, PCWSTR pOut, BYTE Flags)
{
	wstring CabFile;
	if (pCabFile)
	{
		if (!AccessFile(pCabFile))
			return false;

		CabFile = LongPathName(pCabFile);
		if (CabFile == L"")
			return false;
		pCabFile = CabFile.c_str();

		out << L"CAB" << ' ' << GetString(FilePath) << '\n' << pCabFile << '\n';
	}

	wstring OutDir;
	if (!pOut)
	{
		OutDir = CabFile;
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
		wstring Out = pOut;

		if (!SetCurrentDirectoryW(Out.c_str()))
			if (!CreateDirectoryW(
				Out.c_str(),
				nullptr))
				if (GetLastError() != ERROR_ALREADY_EXISTS
					|| !IsAnEmptyDirectory(Out.c_str()))
					return false;
	}

	if (!PrintFileInfo(pCabFile, pPsfFile, pXmlFile, pOut))
		return false;


	ReportStruct ProcData;
	ProcData.Progress = 0;
	ProcData.Exit = false;
	ProcData.hEvent = Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? nullptr : CreateEventW(nullptr, FALSE, FALSE, nullptr);

	if (pCabFile)
	{
		wprintf(GetString(Expanding).get(), L"CAB");

		HANDLE hCab = PSFExtHandler_util_OpenCabinet(pCabFile);
		WORD n = PSFExtHandler_util_CabinetGetFileCount(hCab);
		wprintf(GetString(File_Count).get(), L"CAB", n);
		SetCurrentDirectoryW(OutDir.c_str());
		thread Thread(Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? [](ReportStruct*) {} : ReportThread, &ProcData);
		if (Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY)
			Thread.detach();
		int nThreads = omp_get_num_procs();
		if (Flags & FLAG_ARG_EXPAND_SINGLETHREAD)
			nThreads = 1;


		DWORD Err = ERROR_SUCCESS;
		mutex Mutex;
		bool cancel = false;
		DWORD complited = nThreads;
		omp_set_num_threads(nThreads);
#pragma omp parallel
		{
			HANDLE hCabinet = hCab;
			if (omp_get_thread_num() != 0)
				hCabinet = PSFExtHandler_util_CabinetCopyHandle(hCab);

			CabExpansion Data = { &ProcData, 0, 0, 0, complited, cancel,  Mutex, 0 };
			{
				ULONG range = n / nThreads;
				ULONG remainder = n % nThreads;
				Data.start = range * omp_get_thread_num();

				if (remainder > static_cast<DWORD>(omp_get_thread_num()))
					++range;

				if (remainder != 0)
					Data.start += remainder > static_cast<DWORD>(omp_get_thread_num()) ? omp_get_thread_num() : remainder;
				Data.end = Data.start + range;
			}

			if (!PSFExtHandler_util_ExpandCabinet(hCabinet,
				Flags & FLAG_ARG_EXPAND_SINGLETHREAD && Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? nullptr : reinterpret_cast<PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK>(CabExpansionCallback), &Data))
				if (Err != ERROR_SUCCESS
					&& GetLastError() != ERROR_CANCELLED)
				{
					Err = GetLastError();
					cancel = true;
				}
			PSFExtHandler_util_CloseCabinet(hCabinet);
		}
		if (Err != ERROR_SUCCESS && Err != ERROR_CANCELLED)
		{
			ProcData.Exit = true;
			SetEvent(ProcData.hEvent);
			Thread.join();
			out << '\n' << '\n';

			SetLastError(Err);
			return false;
		}
		if (!(Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY))
		{
			ProcData.Exit = true;
			SetEvent(ProcData.hEvent);
			Thread.join();
			out << '\n' << '\n';
		}
	}

	HPSF hPSF;
	wstring XmlFile;
	if (!pXmlFile)
	{
		XmlFile = pOut;
		XmlFile += L"\\express.psf.cix.xml";
		if (!PathIsUNCW(XmlFile.c_str()))
			XmlFile = ShortPathName(XmlFile.c_str());
		else
			XmlFile.erase(2, 7);
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

	ProcData.Exit = false;
	ProcData.Progress = 0;
	thread Thread(Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? [](ReportStruct*) {} : ReportThread, &ProcData);
	if (Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY)
		Thread.detach();
	BOOL ret = PSFExtHandler_ExpandPSF(hPSF, pOut,
		PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS
		| PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD
		| PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
		| (Flags & FLAG_ARG_EXPAND_SINGLETHREAD ? PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD : 0)
		| (Flags & FLAG_ARG_EXPAND_VERIFY ? PSFEXTHANDLER_EXTRACT_FLAG_VERIFY : 0),
		Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? nullptr :
		[](const PSFEXTHANDLER_EXPAND_INFO* iep, PVOID Data)
		{
			if (reinterpret_cast<ReportStruct*>(Data)->Progress != static_cast<int>(static_cast<ULONGLONG>(iep->dwCompletedBytes) * 100 / iep->dwTotalBytes))
			{
				reinterpret_cast<ReportStruct*>(Data)->Progress = static_cast<int>(static_cast<ULONGLONG>(iep->dwCompletedBytes) * 100 / iep->dwTotalBytes);
				SetEvent(reinterpret_cast<ReportStruct*>(Data)->hEvent);
			}

			return TRUE;
		}
	, &ProcData);
	if (!(Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY))
	{
		ProcData.Exit = true;
		SetEvent(ProcData.hEvent);
		Thread.join();
		CloseHandle(ProcData.hEvent);
		out << '\n';
	}

	PSFExtHandler_ClosePSF(hPSF);

	return ret;
}
