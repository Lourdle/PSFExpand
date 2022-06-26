# PSFExtHandler_ExtractFile
Extract files based on a provided filename.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFile(
    _In_        HPSF hPSF,
    _In_        PCWSTR pTargetFileName,
    _In_opt_    PCWSTR pOutFile,
    _In_        DWORD dwExtractionFlags
);
````
### Parameters
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md).

`[in] pTargetFileName`  
Target filename. Provide a full filename within the PSF file to extract. not case sensitive.

`[in, optional] pOutFile`  
Optional. Relative or absolute path to the output file name. If NULL, keep the original name of the file and store it in the current processing directory of the program.

`[in] dwExtractionFlags`  
For information on extraction flags, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md).
### Return Value
Returns TRUE on success, FALSE on failure.
### Remarks
When dwExtractionFlags provides the flag PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS | PSFEXTHANDLER_EXTRACT_FLAG_VERIFY, the file verification operation fails (that is, the hash of the extracted file is not successfully obtained). If PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS is provided and the PSFEXTHANDLER_EXTRACT_FLAG_VERIFY flag is not provided, then PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS is ignored. If PSFEXTHANDLER_EXTRACT_FLAG_VERIFY is provided without the PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS flag, a verification operation failure will return FALSE and LastError will be set.  
Depending on the extraction flags provided, the function returns TRUE but LastError may not be ERROR_SUCCESS, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md) for details.
