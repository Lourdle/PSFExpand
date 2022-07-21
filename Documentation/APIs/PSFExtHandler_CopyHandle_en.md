# PSFExtHandler_CopyHandle
Copies the PSF handle for use by another thread.
````c
PSFEXTRACTIONHANDLER_API
HPSF
PSFExtHandler_CopyHandle(
    _In_        HPSF hPSF
);
````
### Parameters
`[in] hPSF`  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md) or [PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_en.md) or PSFExtHandler_CopyHandle. Closing a handle to a closed or other object is undefined behavior.  
### Return Value
Returns a handle that can be used by other threads.
### Remark
Copies the handle to allow multiple threads to operate on the object concurrently. Calling this function increments the object reference count by one. Unneeded handles should be closed promptly, and the PSF object will not be destroyed until all handles are closed.
