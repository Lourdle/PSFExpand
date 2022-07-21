# PSFExtHandler_util_CabinetCopyHandle
拷贝 Cabinet 句柄以供另一个线程使用。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HANDLE
PSFExtHandler_util_CabinetCopyHandle(
    _In_        HANDLE hCabinet
);
````
### Parameters
`[in] hCabinet`  
A valid handle to the Cabinet. The handle must be the return value of [PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_en.md) or PSFExtHandler_util_CabinetCopyHandle. Copying a handle to a closed or other object is undefined behavior.
### Return Value
Returns a handle that can be used by other threads.
### Remark
Copies the handle to allow multiple threads to operate on the object concurrently. Calling this function increments the object reference count by one. Unneeded handles should be closed promptly, and the Cabinet object will not be destroyed until all handles are closed.
