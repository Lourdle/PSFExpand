# PSFExtHandler_OpenFile
Read the XML file and open the PSF.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HPSF
PSFExtHandler_OpenFile(
    _In_opt_    PCWSTR pPSFFile,
    _In_        PCWSTR pXmlFile,
	_Reserved_  PDWORD Reserved,
	_In_        WORD wFlags
);
````
### Parameters
`[in, optional] pPSFFile`  
Optional. A PSF filename string. If NULL, the returned PSF handle can only be used for file information query, and cannot extract files.

`[in] pXmlFile`  
A string for the XML filename.

`Reserved`  
This parameter is reserved, generally NULL.

`[in] wFlags`  
File open flags, see [PSFEXTHANDLER_OPEN_FLAG](PSFEXTHANDLER_OPEN_FLAG_en.md).
### Return Value
Returns the PSF handle on success, NULL on failure.
### Remarks
If an error occurs, you can query LastError.
