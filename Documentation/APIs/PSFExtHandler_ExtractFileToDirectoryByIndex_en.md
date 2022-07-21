# PSFExtHandler_ExtractFileToDirectoryByIndex
Extract files to the specified directory according to the file index.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectoryByIndex(
    _In_        HPSF hPSF,
    _In_        DWORD dwIndex,
    _In_opt_    PCWSTR pOutDir,
    _In_opt_    PCWSTR pOutFileName,
    _In_        DWORD dwExtractionFlags
);
````
### Parameters
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md) or [PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_en.md).

`[in] pTargetFileName`  
The target file index. The order of the file indexes is the same as the order of the files within the XML file. The index starts with `0` and has a maximum of `number of files -1`. To get the number of files, see [PSFExtHandler_GetFileCount](PSFExtHandler_GetFileCount_en.md).

`[in, optional] pOutDir`  
Optional. Relative or absolute path to the folder where the output files are stored. If NULL, output the file to the current processing directory of the program.

`[in, optional] pOutFile`  
Optional. The output file names are stored in the folder. If NULL, save the file to the storage folder with the original name.

`[in] dwExtractionFlags`  
For information on extraction flags, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md).
### Return Value
Returns TRUE on success, FALSE on failure.
### Remarks
If the index value is invalid, return FALSE and set LastError to ERROR_INVALID_PARAMETER.  
This function cannot use PSFEXTHANDLER_EXTRACT_WRITE_DATA_TO_HANDLE.  
When dwExtractionFlags provides the flag PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS | PSFEXTHANDLER_EXTRACT_FLAG_VERIFY, the file verification operation fails (that is, the hash of the extracted file is not successfully obtained). If PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS is provided and the PSFEXTHANDLER_EXTRACT_FLAG_VERIFY flag is not provided, then PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS is ignored. If PSFEXTHANDLER_EXTRACT_FLAG_VERIFY is provided without the PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS flag, a verification operation failure will return FALSE and LastError will be set.  
Depending on the extraction flags provided, the function returns TRUE but LastError may not be ERROR_SUCCESS, see [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_en.md) for details.
