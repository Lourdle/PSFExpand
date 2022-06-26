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
A valid Cabinet handle. The handle must be the return value of [PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_en.md).

`[in, optional] pfProgressCallback`  
Optional. A defined parameter with no return value is the [PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md) enumeration, the [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md) structure's constant pointer, a pointer to a handle, and a function pointer to a user-defined untyped pointer.  
The user defines a VOID ([PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md), const [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md)\*, PVOID) The function of the C calling convention is completed during the file extension process. Once provided function reports progress.  
The first parameter provides the current state. When the state is State_WriteFile, the file handle can be written by modifying the pointer data to the handle. If the handle pointed to by the pointer is NULL (the default), a file is created in the program's current directory with the default filename. Otherwise write data to the provided handle. If set to INVALID_HANDLE_VALUE, it is considered that the user canceled the operation and tried to stop, and set LastError to ERROR_CANCELLED. When the state is State_Close, if the handle is provided, it is responsible for closing the handle, and the function will not set the file time; otherwise, the handle pointed to by the pointer is NULL, and the function will automatically set the file time.  
If NULL, it is assumed that the unwinding process does not need to know the progress.

`[in, optional] pvUserData`  
Optional. User-defined custom data provided to progress reporting functions.
### Return Value
Returns TRUE on success, FALSE on failure.
### Remarks
If an error occurs, you can query LastError.
