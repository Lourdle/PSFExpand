# PSFExtHandler_ClosePSF
Closes opened PSF handle.
````c
PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
    _In_        HPSF hPSF
);
````
### Parameters
hPSF\[in\]  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md). Closing a handle to a closed or other object is undefined behavior.  
### Return Value
This function has no return value.
### Remarks
This function will free the memory used to save the PSF, and close the open PSF file at the same time, will set LastError.  
If the parameter is `NULL` or `INVALID_HANDLE_VALUE` no exception will be raised and LastError will be set to `ERROR_INVALID_HANDLE`.  
Since the PSF class of the PSF Extraction Handler does not have locks, the behavior of multiple threads concurrently calling this function and the threads operating through this PSF handle is undefined.
