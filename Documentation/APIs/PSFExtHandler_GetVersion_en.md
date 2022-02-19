# PSFExtHandler_GetVersion
Get the PSF Extraction Handler API version.
````c
PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetVersion();
````
### Parameters
None.
### Return Value
Returns a 32-bit number with the first 10 bits containing the major version, the next 10 bits containing the minor version, then the next 10 bits containing the patch version, 2 bits reserved.
### Remarks
Major, minor and patch versions are defined in the header file, see [PSFEXTHANDLER_VERSION](PSFEXTHANDLER_VERSION_en.md).  
This function can get the runtime version, that is, returns the value defined by the compile-time header file (WIMLIB_MAJOR_VERSION << 22) | (WIMLIB_MINOR_VERSION << 12) | (WIMLIB_PATCH_VERSION << 2).
