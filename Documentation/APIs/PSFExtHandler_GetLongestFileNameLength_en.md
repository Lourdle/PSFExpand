# PSFExtHandler_GetLongestNameLength
Get the longest filename length.
````c
PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetLongestFileNameLength(
	_In_		HPSF hPSF
);
````
### Parameters
hPSF\[in\]  
A valid PSF handle. The handle must be the return value of [PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_en.md).  
### Return Value  
Returns the longest filename length. Returns 0 if the handle is invalid.
