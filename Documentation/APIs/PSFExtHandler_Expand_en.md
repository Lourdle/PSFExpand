# PSFExtHandler_Expand
Expands all files in PSF.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExpandPSF(
    _In_        HPSF hPSF,
    _In_opt_    PCWSTR pOutDir,
    _In_        WORD ExtractionFlags,
    _In_opt_    PSFEXTHANDLER_PROGRESS_PROC pfProgress,
    _In_opt_    PVOID pvUserData
);
````
### Parameters
hPSF\[in\]  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md).

pOutDir\[in, optional\]  
Optional. Relative or absolute path to the folder where the output files are stored. If it is NULL, the expansion file will be stored in the current processing directory of the program.

ExtractionFlags\[in\]  
For information on extraction flags, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md).

pfProgress\[in, optional\]  
Optional. A defined return value type BOOL parameter is a constant pointer to the [PSFEXTHANDLER_EXPAND_INFO](PSFEXTHANDLER_EXPAND_INFO_en.md) structure and a function pointer to a user-defined void pointer.  
A user-defined function with the C calling convention of BOOL(const [PSFEXTHANDLER_EXPAND_INFO](PSFEXTHANDLER_EXPAND_INFO_en.md)\*, PVOID), which will call the provided function once for each file during file expansion to report progress. The return value of the function should not be FALSE. If it returns FALSE, it is considered that the user canceled the operation and tried to stop, and set LastError to ERROR_CANCELLED.  
If NULL, it is assumed that the unwinding process does not need to know the progress.

pvUserData  
Optional. User-defined custom data provided to progress reporting functions.
### Return Value
Returns TRUE on success, FALSE on failure.
### Remarks
When ExtractionFlags provides the PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS flag, when one of the file extraction operations fails, the extension will continue, and will return FALSE after completion.  
Depending on the extraction flags provided, the function returns TRUE but LastError may not be ERROR_SUCCESS, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md) for details.
