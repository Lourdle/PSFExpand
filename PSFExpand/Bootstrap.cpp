#include "pch.h"
#include <Windows.h>

#include "resource.h"
#include "PSFExpand.h"

#include "../PSFExtractionHandler/PSFExtractionHandler.h"

#include <iostream>
#include <string>
using namespace std;

static PCWSTR	g_pPsfFile;
static PCWSTR	g_pXmlFile;
static BYTE		g_Flags;

constexpr BYTE FLAG_ARG_LIST_DETAIL = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXTRACT_VERBOSE = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_VERIFY = 0x0001;
constexpr BYTE FLAG_ARG_EXPAND_SINGLETHREAD = 0x0002;
constexpr BYTE FLAG_ARG_EXPAND_NOPROGRESSDISPLAY = 0x0004;

struct Error
{
	static void InvalidArg(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Unknown_Option).get(), pString);
		wcerr << GetString(Tip);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void InvalidCmdline(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Invalid_Cmdline).get(), pString);
		wcerr << GetString(Tip).get();
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void MissingArg(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Option_Needs_an_Arg).get(), pString);
		wcerr << GetString(Tip);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void InvalidHelpArg()
	{
		wcerr << GetString(Invalid_Help_Option);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void MissingOption(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Missing_Option).get(), pString);
		wcerr << GetString(Tip);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void MissingOperationCmd()
	{
		wcerr << GetString(No_Operation_Command);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}
};

struct PrintHelp
{
	static void ProgramHelp()
	{
		wcout << L"PSFExpand.exe {operation_command} [<operation_Optionuments>]\n\n";
		wcout << GetString(Program_Description);
		wprintf(L"\n\n%ls\n\n  /List\t\t%ls  /Extract\t%ls  /Expand\t%ls",
			GetString(Operation_Commands).get(), GetString(List_Description).get(), GetString(Extract_Description).get(), GetString(Expand_Description).get());
		wcout << '\n' << GetString(Tip);
		ExitProcess(0);
	}

	static void ListHelp()
	{
		wcout << L"/List /XmlFile:<path_to_xml_file> [/DisplayDetail] [/Screen {criterias}]\n\n";
		wprintf(L"%ls\n  /XmlFile\t\t%ls\n  /DisplayDetail\t%ls\t\t\n  /Screen {criterias}\t%ls\n\n%ls\n\n",
			GetString(List_Description).get(), GetString(XmlFile_Option).get(),
			GetString(DisplayDetail_Option).get(), GetString(Screen_Option).get(), GetString(Examples).get());
		wcout << L"  PSFExpand.exe /List /DisplayDetail /XmlFile:D:\\express.psf.cix.xml\n  PSFExpand.exe /List /XmlFile:D:\\express.psf.cix.xml /Screen *.dll\n";
		ExitProcess(0);
	}

	static void ExtractHelp()
	{
		wcout << L"/Extract /XmlFile:<path_to_xml_file> /PsfFile:<path_to_psf_file> /File:<file_name>\n  /StorageDirectory:<path_to_storage_dir> [/Verify] [/Verbose]\n\n";
		wprintf(L"%ls\n  /XmlFile\t\t%ls\n  /PsfFile\t\t%ls\n  /File\t\t\t%ls\n  /StorageDirectory\t%ls\n  /Verify\t\t%ls\n  /Verbose\t\t%ls\n\n%ls\n\n",
			GetString(Extract_Description).get(), GetString(XmlFile_Option).get(), GetString(PsfFile_Option).get(),
			GetString(File_Option).get(), GetString(StorageDirectory_Option).get(), GetString(Verify_Option).get(), GetString(Verbose_Option).get(), GetString(Examples).get());
		wcout << L"  PSFExpand.exe /Extract /PsfFile:D:\\Windows10.0-KB0000000-x64.psf /File:historycix.cab /Verify\n    /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml /StorageDirectory:D:\\Windows10.0-KB0000000-x64\n\n  PSFExpand.exe /Extract /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml /File:*.dll\n    /PsfFile:D:\\Windows10.0-KB0000000-x64.psf /StorageDirectory:D:\\Windows10.0-KB0000000-x64\\dlls\n";
		ExitProcess(0);
	}

	static void ExpandHelp()
	{
		wcout << L"/Expand /XmlFile:<path_to_xml_file> /PsfFile:<path_to_psf_file> /StorageDirectory:<path_to_storage_dir>\n  [/Verify] [/SingleThread] [/NoProgressDisplay]\n\n";
		wprintf(L"%ls\n  /XmlFile\t\t%ls\n  /PsfFile\t\t%ls\n  /StorageDirectory\t%ls\n  /Verify\t\t%ls\n  /SingleThread\t\t%ls\n  /NoProgressDisplay\t%ls\n\n%ls\n\n",
			GetString(Expand_Description).get(), GetString(XmlFile_Option).get(), GetString(PsfFile_Option).get(), GetString(StorageDirectory_Option).get(),
			GetString(Verify_Option).get(), GetString(SingleThread_Option).get(), GetString(NoProgressDisplay_Option).get(), GetString(Example).get());
		wcout << L"  PSFExpand.exe /Expand /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml\n    /PsfFile:D:\\Windows10.0-KB0000000-x64.psf /StorageDirectory:D:\\Windows10.0-KB0000000-x64 /Verify\n";
		ExitProcess(0);
	}
};

static bool GetFiles(PWSTR pString)
{
	switch (CompareStrings(pString,
		{
			L"XmlFile",
			L"PsfFile"
		},
		{ 0,1 }))
	{
	case 0:
		g_pXmlFile = GetSubstringFromArgString(pString, 7);
		if (!g_pXmlFile)
			Error::InvalidArg(pString);
		else if (g_pXmlFile == reinterpret_cast<PCWSTR>(-1))
			Error::MissingArg(pString);
		break;
	case 1:
		g_pPsfFile = GetSubstringFromArgString(pString, 7);
		if (!g_pPsfFile)
			Error::InvalidArg(pString);
		else if (g_pPsfFile == reinterpret_cast<PCWSTR>(-1))
			Error::MissingArg(pString);
		break;
	default:
		return false;
	}

	return true;
}

static void ShowErrorMessage()
{
	LPWSTR lpErrMsg;
	DWORD dwError = GetLastError();

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&lpErrMsg),
		0, NULL);

	wcerr << lpErrMsg;
	LocalFree(lpErrMsg);
	
	ExitProcess(dwError);
}

static int ListArg(int argc, wchar_t** argv)
{
	PWSTR* Screeners = nullptr;
	int ScreenerCount = 0;
	for (int i = 1; i != argc; ++i)
	{
		if (argv[i][0] != '/')
			Error::InvalidCmdline(argv[i]);

		bool ret = GetFiles(argv[i] + 1);

		switch (CompareStrings(argv[i] + 1,
			{
				L"DisplayDetail",
				L"Screen",
				L"Help",
				L"H",
				L"?"
			},
			{ }))
		{
		case 0:
			g_Flags |= FLAG_ARG_LIST_DETAIL;
			break;
		case 1:
			if (i == argc - 1)
				Error::MissingArg(argv[i] + 1);

			Screeners = argv + i + 1;
			ScreenerCount = argc - i - 1;
			i = argc - 1;
			break;
		case 2:case 3:case 4:
			if (i == argc - 1)
				PrintHelp::ListHelp();
			else
				Error::InvalidHelpArg();
			break;
		default:
			if (ret
				&& g_pXmlFile
				&& !g_pPsfFile)
				continue;

			Error::InvalidArg(argv[i] + 1);
		}
	}

	if (!g_pXmlFile)
		Error::MissingOption(L"XmlFile");

	if (!List(g_pXmlFile, g_Flags & FLAG_ARG_LIST_DETAIL, Screeners, ScreenerCount))
		ShowErrorMessage();

	if (ScreenerCount == 0)
		wcout << GetString(Done);

	return 0;
}

static int ExtractArg(int argc, wchar_t** argv)
{
	PWSTR pFile = nullptr;
	PCWSTR pOut = nullptr;

	for (int i = 1; i != argc; ++i)
	{
		if (argv[i][0] != '/')
			Error::InvalidCmdline(argv[i]);

		if (GetFiles(argv[i] + 1))
			continue;

		switch (CompareStrings(argv[i] + 1,
			{
				L"Help",
				L"H",
				L"?",
				L"File",
				L"StorageDirectory",
				L"Verify",
				L"Verbose"
			},
			{ 3,4 }))
		{
		case 0:case 1:case 2:
			if (i == argc - 1)
				PrintHelp::ExtractHelp();
			else
				Error::InvalidHelpArg();
		case 3:
			pFile = GetSubstringFromArgString(argv[i] + 1, 4);
			if (!pFile)
				Error::InvalidArg(argv[i] + 1);
			else if (pFile == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);
			break;
		case 4:
			pOut = GetSubstringFromArgString(argv[i] + 1, 16);
			if (!pOut)
				Error::InvalidArg(argv[i] + 1);
			else if (pOut == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);
			break;
		case 5:
			g_Flags |= FLAG_ARG_EXTRACT_VERIFY;
			break;
		case 6:
			g_Flags |= FLAG_ARG_EXTRACT_VERBOSE;
			break;
		default:
			Error::InvalidArg(argv[i] + 1);
		}
	}

	if (!g_pPsfFile)
		Error::MissingOption(L"PsfFile");
	if (!g_pXmlFile)
		Error::MissingOption(L"XmlFile");
	if (!pFile)
		Error::MissingOption(L"File");
	if (!pOut)
		Error::MissingOption(L"StorageDirectory");

	if (!Extract(g_pXmlFile, g_pPsfFile, pFile, pOut, g_Flags & FLAG_ARG_EXTRACT_VERIFY, g_Flags * FLAG_ARG_EXTRACT_VERBOSE))
		ShowErrorMessage();
	wcout << GetString(Done);

	return 0;
}

int ExpandArg(int argc, wchar_t** argv)
{
	PCWSTR pOut = nullptr;

	for (int i = 1; i != argc; ++i)
	{
		if (argv[i][0] != '/')
			Error::InvalidCmdline(argv[i]);

		if (GetFiles(argv[i] + 1))
			continue;

		switch (CompareStrings(argv[i] + 1,
			{
				L"Help",
				L"H",
				L"?",
				L"StorageDirectory",
				L"Verify",
				L"SingleThread",
				L"NoProgressDisplay"
			},
			{ 3 }))
		{
		case 0:case 1:case 2:
			PrintHelp::ExpandHelp();
		case 3:
			pOut = GetSubstringFromArgString(argv[i] + 1, 16);
			if (!pOut)
				Error::InvalidArg(argv[i] + 1);
			else if (pOut == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);
			break;
		case 4:
			g_Flags |= FLAG_ARG_EXPAND_VERIFY;
			break;
		case 5:
			g_Flags |= FLAG_ARG_EXPAND_SINGLETHREAD;
			break;
		case 6:
			g_Flags |= FLAG_ARG_EXPAND_NOPROGRESSDISPLAY;
			break;
		default:
			Error::InvalidArg(argv[i] + 1);
		}
	}

	if (!g_pPsfFile)
		Error::MissingOption(L"PsfFile");
	if (!g_pXmlFile)
		Error::MissingOption(L"XmlFile");
	if (!pOut)
		Error::MissingOption(L"StorageDirectory");

	HPSF hPSF = PSFExtHandler_OpenFile(g_pPsfFile, g_pXmlFile);
	if (!hPSF)
		ShowErrorMessage();
	DWORD n = PSFExtHandler_GetFileCount(hPSF);

	wprintf(GetString(File_Count).get(), n);
	BYTE Progress = 0;

	BOOL ret = PSFExtHandler_ExpandPSF(hPSF, pOut,
		PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS
		| PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD
		| PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
		| (g_Flags & FLAG_ARG_EXPAND_SINGLETHREAD ? PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD : 0)
		| (g_Flags & FLAG_ARG_EXPAND_VERIFY ? PSFEXTHANDLER_EXTRACT_FLAG_VERIFY : 0),
		g_Flags & FLAG_ARG_EXPAND_NOPROGRESSDISPLAY ? reinterpret_cast<PSFEXTHANDLER_PROGRESS_PROC>(NULL) :
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
	if (ret)
		wcout << '\n' << GetString(Done);
	else
		ShowErrorMessage();

	return 0;
}

int wmain(int argc, wchar_t** argv)
{
	setlocale(LC_ALL, "");
	wcout.imbue(locale(""));

	wcout << '\n' << GetString(Program) << '\n' << "PSF Extraction Handler API " << GetString(Version) << L": " << PSFExtHandler_GetVersionString() << '\n' << '\n';

	if (argc == 1)
	{
		PrintHelp::ProgramHelp();
		return 0;
	}

	if (argv[1][0] != '/')
		Error::InvalidCmdline(argv[1]);
	switch (CompareStrings(argv[1] + 1,
		{
			L"Help",
			L"H",
			L"?",
			L"List",
			L"Extract",
			L"Expand"
		},
		{ }))
	{
	case 0:case 1:case 2:
		if (argc == 2)
			PrintHelp::ProgramHelp();
		break;
	case 3:
		return ListArg(argc - 1, argv + 1);
	case 4:
		return ExtractArg(argc - 1, argv + 1);
	case 5:
		return ExpandArg(argc - 1, argv + 1);
	default:
		Error::MissingOperationCmd();
	}

	return 0;
}
