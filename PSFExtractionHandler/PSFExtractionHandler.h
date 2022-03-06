#ifndef PSFEXTRACTIONHANDLER_H
#define PSFEXTRACTIONHANDLER_H

#define PSFEXTRACTIONHANDLER_API __declspec(dllimport)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct PSF PSF, * HPSF;

#define PSFEXTHANDLER_MAJOR_VERSION									1
#define PSFEXTHANDLER_MINOR_VERSION									0
#define PSFEXTHANDLER_PATCH_VERSION									2

PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetVersion();

PSFEXTRACTIONHANDLER_API
PCWSTR
PSFExtHandler_GetVersionString();

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HPSF
PSFExtHandler_OpenFile(
	_In_opt_	PCWSTR pPSFFile,
	_In_		PCWSTR pXmlFile
);

PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetLongestFileNameLength(
	_In_		HPSF hPSF
);

PSFEXTRACTIONHANDLER_API
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
_Must_inspect_result_
BOOL
PSFExtHandler_GetFileInfo(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PWSTR pszFileName,
	_When_(pszFileName == NULL, _Out_opt_) _When_(pszFileName != NULL, _Inout_opt_) PDWORD pcbData,
	_Out_opt_	PDWORD pdwFileSize,
	_Out_opt_	PSFEXTHANDLER_FILE_TYPE* Type
);


#define PSFEXTHANDLER_EXTRACT_FLAG_VERIFY								0x0001
#define PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS							0x0002
#define PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS						0x0004
#define PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD						0x0008
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA						0x0010
#define PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS		0x0020
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE					0x0040
#define PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT					0x0080
#define PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD\
																		0x0100
#define PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY		0x0200

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFile(
	_In_		HPSF hPSF,
	_In_		PCWSTR pTargetFileName,
	_In_opt_	PCWSTR pOutFile,
	_In_		WORD ExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileByIndex(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_In_opt_	PCWSTR pOutFile,
	_In_		WORD ExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectory(
	_In_		HPSF hPSF,
	_In_		PCWSTR pTargetFileName,
	_In_opt_	PCWSTR pOutDir,
	_In_opt_	PCWSTR pOutFileName,
	_In_		WORD ExtractionFlags
);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectoryByIndex(
	_In_		HPSF hPSF,
	_In_		DWORD dwIndex,
	_In_opt_	PCWSTR pOutDir,
	_In_opt_	PCWSTR pOutFileName,
	_In_		WORD ExtractionFlags
);

typedef struct _PSFEXTHANDLER_EXPAND_INFO
{
	DWORD		dwIndex;
	PCWSTR		pFileName;
	DWORD		dwCurrentFileSize;
	DWORD		dwCompletedBytes;
	DWORD		dwTotalBytes;
	DWORD		dwCompletedFileCount;
	DWORD		dwTotalFileCount;
}PSFEXTHANDLER_EXPAND_INFO;

typedef BOOL(*PSFEXTHANDLER_PROGRESS_PROC)(const PSFEXTHANDLER_EXPAND_INFO*, PVOID);

PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExpandPSF(
	_In_		HPSF hPSF,
	_In_opt_	PCWSTR pOutDir,
	_In_		WORD ExtractionFlags,
	_In_opt_	PSFEXTHANDLER_PROGRESS_PROC pfProgress,
	_In_opt_	PVOID pvUserData
);

PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
	_In_		HPSF hPSF
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !PSFEXTRACTIONHANDLER_H
