# PSFExtHandler_OpenFile
Read the XML file and open the PSF.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HPSF
PSFExtHandler_OpenFile(
    _In_opt_    PCWSTR pPSFFile,
    _In_        PCWSTR pXmlFile
);
````
### Parameters
pPSFFile\[in, optional\]  
Optional. A PSF filename string. If NULL, the returned PSF handle can only be used for file information query, and cannot extract files.

pXmlFile\[in\]  
A string for the XML filename.
### Return Value
Returns the PSF handle on success, NULL on failure.
### Remarks
If an error occurs, you can query LastError.
