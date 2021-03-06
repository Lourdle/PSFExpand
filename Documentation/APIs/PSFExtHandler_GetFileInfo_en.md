# PSFExtHandler_GetFileInfo
Get file information based on index.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_GetFileInfo(
    _In_        HPSF hPSF,
    _In_        DWORD dwIndex,
    _Out_writes_bytes_to_opt_(*pcbData, *pcbData) PWSTR pszFileName,
    _When_(pszFileName == NULL, _Out_opt_) _When_(pszFileName != NULL, _Inout_opt_) PDWORD pcbData,
    _Out_opt_   PDWORD pdwFileSize,
    _Out_opt_   PFILETIME pFileTime,
    _Out_opt_   PSFEXTHANDLER_FILE_TYPE* Type,
    _Reserved_  PVOID Reserved
);
````
### Parameters
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md) or [PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_en.md).

`[in] dwIndex`  
The target file index. The order of the file indexes is the same as the order of the files within the XML file. The index starts with `0` and has a maximum of `number of files -1`. To get the number of files, see [PSFExtHandler_GetFileCount](PSFExtHandler_GetFileCount_en.md).

`[out, optional] pszFileName`  
Optional. Pointer to buffer to receive filename.

`[in, out, optional] pcbData`  
Optional. This parameter can be NULL only if pszFileName is NULL.  
The parameter receives the size of the file name including null characters, in bytes.  
When the filename buffer is not large enough, it will return FALSE and set LastError to ERROR_MORE_DATA.  
If pszFileName is NULL and pcbData is not NULL, return TRUE after receiving the data size, and set LastError to ERROR_SUCCESS.

`[out, optional] pdwFileSize`  
Optional. Receive the size of this file, in bytes.

`[out, optional] pFileTime`
Optional. Receive the file time of this file.

`[out, optional] Type`  
Optional. The type of the file to receive. For information on types, see [PSFEXTHANDLER_FILE_TYPE](PSFEXTHANDLER_FILE_TYPE_en.md).  

`Reserved`  
This parameter is reserved, must be NULL.
### Return Value
Returns TRUE on success, FALSE on failure.
### Remark
If the index value is invalid, return FALSE and set LastError to ERROR_INVALID_PARAMETER.
