# PSFExtHandler_OpenFile
Read the XML file and open the PSF.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HPSF
PSFExtHandler_OpenFileEx(
    _In_opt_    PCWSTR pPSFFile,
    _In_        PCWSTR pXmlFile,
    _Reserved_  PVOID Reserved,
    _In_        DWORD dwFlags
);
````
### Parameters
`[in, optional] pPSFFile`  
Optional. A PSF filename string. If NULL, the returned PSF handle can only be used for file information query, and cannot extract files.

`[in] pXmlFile`  
A string for the XML filename.

`Reserved`  
This parameter is reserved, must be NULL.

`[in] dwFlags`  
File open flags, deprecated.
### Return Value
Returns the PSF handle on success, NULL on failure.
### Remark
If an error occurs, you can query LastError.
