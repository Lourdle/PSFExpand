#ifndef PSFEXTRACTIONHANDLER_H
#define PSFEXTRACTIONHANDLER_H

#define PSFEXTRACTIONHANDLER_API __declspec(dllimport)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct PSF PSF, * HPSF;

#define PSFEXTHANDLER_MAJOR_VERSION									((DWORD)2)
#define PSFEXTHANDLER_MINOR_VERSION									((DWORD)0)
#define PSFEXTHANDLER_PATCH_VERSION									((DWORD)1)

#define PSFEXTHANDLER_CURRENT_VERSION								(PSFEXTHANDLER_MAJOR_VERSION << 22\
																	| PSFEXTHANDLER_MINOR_VERSION << 12\
																	| PSFEXTHANDLER_PATCH_VERSION << 2)

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
DWORD
PSFExtHandler_GetVersion();

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
PCWSTR
PSFExtHandler_GetVersionString();

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HPSF
PSFExtHandler_OpenFile(
	_In_opt_	PCWSTR pPSFFile,
	_In_		PCWSTR pXmlFile
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HPSF
PSFExtHandler_OpenFileEx(
	_In_opt_	PCWSTR pPSFFile,
	_In_		PCWSTR pXmlFile,
	_Reserved_	PVOID Reserved,
	_In_		DWORD wFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
SHORT
PSFExtHandler_GetLongestFileNameLength(
	_In_		HPSF hPSF
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
DWORD
PSFExtHandler_GetFileCount(
	_In_		HPSF hPSF
);

typedef enum _PSFEXTHANDLER_FILE_TYPE
{
	PSFEXTHANDLER_FILE_TYPE_RAW,
	PSFEXTHANDLER_FILE_TYPE_PA19,
	PSFEXTHANDLER_FILE_TYPE_PA30,
	PSFEXTHANDLER_FILE_TYPE_UNKNOWN
}PSFEXTHANDLER_FILE_TYPE;

PSFEXTRACTIONHANDLER_API
BOOL
PSFExtHandler_GetFileInfo(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PWSTR pszFileName,
	_When_(pszFileName == NULL, _Out_opt_) _When_(pszFileName != NULL, _Inout_opt_) PDWORD pcbData,
	_Out_opt_	PDWORD pdwFileSize,
	_Out_opt_	PFILETIME pFileTime,
	_Out_opt_	PSFEXTHANDLER_FILE_TYPE* Type,
	_Reserved_	PVOID Reserved
);


#define PSFEXTHANDLER_EXTRACT_FLAG_VERIFY								((DWORD)0x0001)
#define PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS							((DWORD)0x0002)
#define PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS						((DWORD)0x0004)
#define PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD						((DWORD)0x0008)
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA						((DWORD)0x0010)
#define PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS		((DWORD)0x0020)
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE					((DWORD)0x0040)
#define PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT					((DWORD)0x0080)
#define PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD\
																		((DWORD)0x0100)
#define PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY		((DWORD)0x0200)
#define PSFEXTHANDLER_EXTRACT_FLAG_DO_NOT_SET_FILE_TIME					((DWORD)0x0400)

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFile(
	_In_		HPSF hPSF,
	_In_		PCWSTR pTargetFileName,
	_In_opt_	PCWSTR pOutFile,
	_In_		DWORD dwExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileByIndex(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_In_opt_	PCWSTR pOutFile,
	_In_		DWORD dwExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectory(
	_In_		HPSF hPSF,
	_In_		PCWSTR pTargetFileName,
	_In_opt_	PCWSTR pOutDir,
	_In_opt_	PCWSTR pOutFileName,
	_In_		DWORD dwExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectoryByIndex(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_In_opt_	PCWSTR pOutDir,
	_In_opt_	PCWSTR pOutFileName,
	_In_		DWORD dwExtractionFlags
);

typedef struct _PSFEXTHANDLER_EXPAND_INFO
{
	DWORD		dwIndex;
	PCWSTR		pFileName;
	DWORD		dwCurrentFileSize;
	DWORD		dwCompletedBytes;
	DWORD		dwTotalBytes;
	ULONG		ulCompletedFileCount;
	ULONG		ulTotalFileCount;
}PSFEXTHANDLER_EXPAND_INFO;

typedef BOOL(*PSFEXTHANDLER_PROGRESS_PROC)(const PSFEXTHANDLER_EXPAND_INFO*, PVOID);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExpandPSF(
	_In_		HPSF hPSF,
	_In_opt_	PCWSTR pOutDir,
	_In_		DWORD dwExtractionFlags,
	_In_opt_	PSFEXTHANDLER_PROGRESS_PROC pfProgressCallback,
	_In_opt_	PVOID pvUserData
);

PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
	_In_		HPSF hPSF
);

PSFEXTRACTIONHANDLER_API
HPSF
PSFExtHandler_CopyHandle(
	_In_		HPSF hPSF
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HANDLE
PSFExtHandler_util_OpenCabinet(
	_In_		PCWSTR pCabFile
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
WORD
PSFExtHandler_util_CabinetGetFileCount(
	_In_		HANDLE hCabinet);

typedef enum _PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
{
	PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE,
	PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE
}PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE;

typedef struct _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
{
	HANDLE hFile;
	ULONG ulComplitedFiles;
	ULONG ulTotalFiles;
	PCSTR pCurrentFile;
	ULONG ulSize;
	FILETIME FileTime;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS;

typedef union _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO
{
	const PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS* pProgressInfo;
	PHANDLE phFile;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO;

typedef VOID(*PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK)(PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE, PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO, PVOID);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_util_ExpandCabinet(
	_In_		HANDLE hCabinet,
	_In_		PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfnProgressCallback,
	_In_opt_	PVOID pvUserData
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HANDLE
PSFExtHandler_util_CabinetCopyHandle(
	_In_		HANDLE hCabinet
);

PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_util_CloseCabinet(
	_In_		HANDLE hCabinet
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !PSFEXTRACTIONHANDLER_H
