# PSFExtHandler_util_CloseCabinet
Closes opened Cabinet handle.
````c
PSFEXTRACTIONHANDLER_API
VOID PSFExtHandler_util_CloseCabinet(
    _In_        HANDLE hCabinet
);
````
### Parameters
`[in] hCab`  
A valid Cabinet handle. The handle must be the return value of [PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_en.md).
### Return Value
This function has no return value.
### Remark
This function will free the corresponding memory and decrement the reference count of the Cabinet object by one. When the reference count reaches zero, the object is to be destroyed, the Cab file is to be closed, and memory is to be freed. LastError will be set.  
