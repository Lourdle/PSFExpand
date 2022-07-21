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
### 参数
`[in] hCabinet`  
一个有效的 Cabinet 句柄。句柄必须为[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)或 PSFExtHandler_util_CabinetCopyHandle 的返回值。拷贝一个已经关闭的或其他对象的句柄，是未定义的行为。  
### 返回值
返回一个可供其他线程使用的句柄。
### 备注
复制句柄以便多线程并发操作对象。调用此函数会使对象引用计数自加一。不需要的句柄应及时关闭，且只有所有句柄关闭后 Cabinet 对象才会销毁。
