# PSFExtHandler_ClosePSF
Closes a PSF handle.
````c
PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
    _In_        HPSF hPSF
);
````
### Parameters
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md) or [PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_en.md). Closing a handle to a closed or other object is undefined behavior.  
### Return Value
This function has no return value.
### Remarks
This function will free the corresponding memory and decrement the reference count of the PSF object by one. When the reference count reaches zero, the object is to be destroyed, the PSF file is to be closed, and memory is to be freed. LastError will be set.  
If the parameter is `NULL` or `INVALID_HANDLE_VALUE` does not throw an exception, sets LastError to `ERROR_INVALID_HANDLE`.
