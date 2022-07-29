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

namespace Error
{
	static void InvalidArg(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Unknown_Option).get(), pString);
		wcerr << GetString(Tip).get();
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
		wcerr << GetString(Tip).get();
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void InvalidHelpArg()
	{
		wcerr << GetString(Invalid_Help_Option).get();
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void MissingOption(PCWSTR pString)
	{
		fwprintf(stderr, GetString(Missing_Option).get(), pString);
		wcerr << GetString(Tip).get();
		ExitProcess(ERROR_INVALID_PARAMETER);
	}

	static void MissingRequiredArg(UINT ID, bool Plural)
	{
		auto ArgString = GetString(ID);
		auto pArgString = const_cast<PWSTR>(ArgString.get());
		auto len = wcslen(pArgString);
		pArgString[len - 1] = 0;
		if (!Plural)
			if (pArgString[len - 2] == 'S')
				pArgString[len - 2] = 0;

		fwprintf_s(stderr, GetString(No_Required_Arguments).get(), pArgString);
		ExitProcess(ERROR_INVALID_PARAMETER);
	}
}

struct PrintHelp
{
	static void ProgramHelp()
	{
		out << L"PSFExpand.exe [/Lang:<lang>] {operation_command} [<operation_arguments>]\n\n";
		out << GetString(Program_Description);
		wprintf(L"\n\n  /Lang\t\t%ls\t\ten\t- English\n\t\tzh-Hans\t- ¼òÌåÖÐÎÄ\n\n\n%ls\n\n  /List\t\t%ls  /Extract\t%ls  /Expand\t%ls",
			GetString(Lang_Description).get(), GetString(Operation_Commands).get(), GetString(List_Description).get(), GetString(Extract_Description).get(), GetString(Expand_Description).get());
		out << '\n' << GetString(Tip);
		ExitProcess(0);
	}

	static void ListHelp()
	{
		out << L"/List /XmlFile:<path_to_xml_file> [/DisplayDetail] [Filename]\n\n";
		wprintf(L"%ls\n%ls\n\n  /XmlFile\t\t%ls\n  /DisplayDetail\t%ls\n%ls\n\n",
			GetString(List_Description).get(), GetString(Options).get(), GetString(XmlFile_Option).get(),
			GetString(DisplayDetail_Option).get(), GetString(Examples).get());
		out << L"  PSFExpand.exe /List /DisplayDetail /XmlFile:D:\\express.psf.cix.xml\n\n  PSFExpand.exe /List /XmlFile:D:\\express.psf.cix.xml *.dll\n";
		ExitProcess(0);
	}

	static void ExtractHelp()
	{
		out << L"/Extract /XmlFile:<path_to_xml_file> /PsfFile:<path_to_psf_file>  /OutDir:<path_to_out_dir>\n {Filename} [/Verify] [/Verbose]\n\n";
		wprintf(L"%ls\n%ls\n\n  /XmlFile\t\t%ls\n  /PsfFile\t\t%ls\n  /OutDir\t\t%ls\n  /Verify\t\t%ls\n  /Verbose  \t\t%ls\n%ls\n\n",
			GetString(Extract_Description).get(), GetString(Options).get(), GetString(XmlFile_Option).get(), GetString(PsfFile_Option).get(),
			GetString(OutDir_Option).get(), GetString(Verify_Option).get(), GetString(Verbose_Option).get(), GetString(Examples).get());
		out << L"  PSFExpand.exe /Extract /PsfFile:D:\\Windows10.0-KB0000000-x64.psf historycix.cab /Verify\n    /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml /OutDir:D:\\Windows10.0-KB0000000-x64\n\n  PSFExpand.exe /Extract /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml \n    /PsfFile:D:\\Windows10.0-KB0000000-x64.psf /OutDir:D:\\Windows10.0-KB0000000-x64\\dlls *.dll\n";
		ExitProcess(0);
	}

	static void ExpandHelp()
	{
		out << L"/Expand {file_info_options} [/Verify]\n  [/SingleThread] [/NoProgressDisplay]\n\n";
		wprintf(L"%ls\n%ls\n\n  /CabFile\t\t%ls\n  /XmlFile\t\t%ls\n  /PsfFile\t\t%ls\n  /OutDir\t\t%ls\n\n%ls\n\n  /Verify\t\t%ls\n  /SingleThread\t\t%ls\n  /NoProgressDisplay\t%ls\n%ls\n\n",
			GetString(Expand_Description).get(), GetString(File_Info_Options).get(), GetString(CabFile_Option).get(), GetString(XmlFile_Option).get(), GetString(PsfFile_Option).get(), GetString(OutDir_Option).get(), GetString(Options).get(),
			GetString(Verify_Option).get(), GetString(SingleThread_Option).get(), GetString(NoProgressDisplay_Option).get(), GetString(Examples).get());
		out << L"  PSFExpand.exe /Expand /XmlFile:D:\\Windows10.0-KB0000000-x64\\express.psf.cix.xml /Verify\n    /PsfFile:D:\\Windows10.0-KB0000000-x64.psf /OutDir:D:\\Windows10.0-KB0000000-x64\n\n  PSFExpand.exe /Expand /CabFile:D:\\Windows10.0-KB0000000-x64_00000000.cab /Verify\n";
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
		nullptr,
		dwError,
		GetThreadUILanguage(),
		reinterpret_cast<LPWSTR>(&lpErrMsg),
		0, nullptr);

	wcerr << lpErrMsg;
	LocalFree(lpErrMsg);

	ExitProcess(dwError);
}

static int ListArg(int argc, wchar_t** argv)
{
	vector<PWSTR> v;
	for (int i = 1; i != argc; ++i)
	{
		if (argv[i][0] != '/')
		{
			v.push_back(argv[i]);
			continue;
		}

		switch (CompareStrings(argv[i] + 1,
			{
				L"XmlFile",
				L"DisplayDetail",
				L"Help",
				L"H",
				L"?"
			}, { 0 }))
		{
		case 0:
			GetFiles(argv[i] + 1);
			break;
		case 1:
			g_Flags |= FLAG_ARG_LIST_DETAIL;
			break;
		case 2:case 3:case 4:
			if (i == argc - 1 && i == 1)
				PrintHelp::ListHelp();
			else
				Error::InvalidHelpArg();
			break;
		default:
			Error::InvalidArg(argv[i] + 1);
		}
	}

	if (!g_pXmlFile)
		Error::MissingOption(L"XmlFile");

	if (!List(g_pXmlFile, g_Flags & FLAG_ARG_LIST_DETAIL, v))
		ShowErrorMessage();

	if (v.empty())
		out << GetString(Done);

	return 0;
}

static int ExtractArg(int argc, wchar_t** argv)
{
	vector<PWSTR> v;
	PCWSTR pOut = nullptr;

	for (int i = 1; i != argc; ++i)
	{
		if (argv[i][0] != '/')
		{
			v.push_back(argv[i]);
			continue;
		}

		if (GetFiles(argv[i] + 1))
			continue;

		switch (CompareStrings(argv[i] + 1,
			{
				L"Help",
				L"H",
				L"?",
				L"OutDir",
				L"Verify",
				L"Verbose"
			},
			{ 3,4 }))
		{
		case 0:case 1:case 2:
			if (i == argc - 1 && i == 1)
				PrintHelp::ExtractHelp();
			else
				Error::InvalidHelpArg();
		case 3:
			pOut = GetSubstringFromArgString(argv[i] + 1, 6);
			if (!pOut)
				Error::InvalidArg(argv[i] + 1);
			else if (pOut == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);
			break;
		case 4:
			g_Flags |= FLAG_ARG_EXTRACT_VERIFY;
			break;
		case 5:
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
	if (v.empty())
		Error::MissingOption(L"\bFilename");
	if (!pOut)
		Error::MissingOption(L"OutDir");

	if (!Extract(g_pXmlFile, g_pPsfFile, v, pOut, g_Flags & FLAG_ARG_EXTRACT_VERIFY, g_Flags & FLAG_ARG_EXTRACT_VERBOSE))
		ShowErrorMessage();
	out << GetString(Done);

	return 0;
}

int ExpandArg(int argc, wchar_t** argv)
{
	PCWSTR pOut = nullptr;
	PCWSTR pCabFile = nullptr;

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
				L"OutDir",
				L"Verify",
				L"SingleThread",
				L"NoProgressDisplay",
				L"CabFile"
			},
			{ 3,7 }))
		{
		case 0:case 1:case 2:
			if (i == argc - 1 && i == 1)
				PrintHelp::ExpandHelp();
			else
				Error::InvalidHelpArg();
		case 3:
			pOut = GetSubstringFromArgString(argv[i] + 1, 6);
			if (!pOut)
				Error::InvalidArg(argv[i] + 1);
			else if (pOut == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);

			{
				auto Len = wcslen(argv[i]);
				if (argv[i][Len - 1] == '\\' || argv[i][Len - 1] == '/')
					argv[i][Len - 1] = 0;
			}
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
		case 7:
		{
			PWSTR pCab = GetSubstringFromArgString(argv[i] + 1, 7);
			if (!pCab)
				Error::InvalidArg(argv[i] + 1);
			else if (pCab == reinterpret_cast<PCWSTR>(-1))
				Error::MissingArg(argv[i] + 1);

			pCabFile = pCab;
		}
		break;
		default:
			Error::InvalidArg(argv[i] + 1);
		}
	}

	if (!pCabFile
		&& (!g_pPsfFile || !g_pXmlFile || !pOut))
		Error::MissingRequiredArg(File_Info_Options, true);

	if (Expand(pCabFile, g_pPsfFile, g_pXmlFile, pOut, g_Flags))
		out << '\n' << GetString(Done);
	else
		ShowErrorMessage();

	return 0;
}

int wmain(int argc, wchar_t** argv)
{
	setlocale(LC_ALL, "");
	wcout.imbue(locale(""));

	if (PSFExtHandler_GetVersion() < PSFEXTHANDLER_CURRENT_VERSION
		|| PSFExtHandler_GetVersion() >> 22 != PSFEXTHANDLER_MAJOR_VERSION)
	{
		SetLastError(ERROR_NOT_SUPPORTED);
		ShowErrorMessage();
	}

	int Start = 1;
	if (argc > 1 && CompareStrings(argv[1], { L"/Lang" }, { 0 }) == 0)
	{
		Start = 2;

		PCWSTR pLang = GetSubstringFromArgString(argv[1], 5);
		if (!pLang || pLang == reinterpret_cast<PCWSTR>(-1))
			pLang = L"en";

		switch (CompareStrings(pLang, { L"zh-Hans",L"en" }, {}))
		{
		case 0:
			if (GetThreadUILanguage() >> 10 != LANG_CHINESE_SIMPLIFIED)
				SetThreadUILanguage(MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED));
			break;
		default:
			if (GetThreadUILanguage() >> 10 != LANG_ENGLISH)
				SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			break;
		}
	}

	out << '\n' << GetString(Program) << '\n' << "PSF Extraction Handler API " << GetString(Version) << L": " << PSFExtHandler_GetVersionString() << '\n' << '\n';

	if (argc == 1 || Start == 2 && argc == 2)
	{
		PrintHelp::ProgramHelp();
		return 0;
	}

	if (argv[Start][0] != '/')
		Error::InvalidCmdline(argv[Start]);

	int (*ArgProc)(int argc, wchar_t** argv) = nullptr;
	switch (CompareStrings(argv[Start] + 1,
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
		if (argc == 2 || argc == 3 && Start == 2)
			PrintHelp::ProgramHelp();
		break;
	case 3:
		ArgProc = ListArg;
		break;
	case 4:
		ArgProc = ExtractArg;
		break;
	case 5:
		ArgProc = ExpandArg;
		break;
	default:
		Error::MissingRequiredArg(Operation_Commands, false);
	}

	return ArgProc ? ArgProc(argc - Start, argv + Start) : 0;
}
