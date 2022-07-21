# PSFExtHandler_util_ExpandCabinet
Expand files in CAB.
````c
PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_util_ExpandCabinet(
    _In_        HANDLE hCabinet,
    _In_        PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfnProgressCallback,
    _In_opt_    PVOID pvUserData
);
````
### Parameters
`[in] hCab`  
A valid Cabinet handle. The handle must be the return value of [PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_en.md) or [PSFExtHandler_util_CabinetCopyHandle](PSFExtHandler_util_CabinetCopyHandle_en.md).

`[in, optional] pfProgressCallback`  
Optional. A defined return value is an enumeration with BOOL parameters [PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_en.md), a constant pointer to the [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md) structure, a pointer to a constant pointer to the handle and a user-defined untyped pointer pointer.

Define a BOOL([PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_en.md), [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_en.md)\*, PVOID) The function of the C calling convention will be called once in the file extension process. The provided function reports progress.  
The first parameter provides the current state. A file handle can be provided through a pointer to handle when the state is `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE`. If the handle pointed to by the pointer is NULL (the default), a file is created in the program's current directory with the default filename. Otherwise write data to the provided handle. If set to `INVALID_HANDLE_VALUE` it is assumed that the user needs to skip this file. When the state is `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE`, this callback function is responsible for closing the handle if the handle is provided, and the function will not set the file time; if the handle pointed to by the pointer is `NULL`, the function will automatically set the file time.  
The function should return TRUE. If it returns FALSE it is considered that the user wants to cancel the operation, this function will cancel the operation and set LastError to `ERROR_CANCELLED`.  
If NULL, it is assumed that the unwinding process does not need to know the progress.  

`[in, optional] pvUserData`  
Optional. User-defined custom data provided to progress reporting functions.
### Return Value
Returns TRUE on success, FALSE on failure.
### Remarks
If an error occurs, you can query LastError.
