# PSFExtHandler_GetFileCount
Get the number of files.
````c
PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetFileCount(
    _In_        HPSF hPSF
);
````
### Parameter
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md) or [PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_en.md).
### Return Value
Returns the number of files. Returns 0 if the handle is invalid.
