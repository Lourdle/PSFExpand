# PSFExtHandler_util_CloseCabinet
关闭打开的 Cabinet 句柄。
````c
PSFEXTRACTIONHANDLER_API
VOID PSFExtHandler_util_CloseCabinet(
    _In_        HANDLE hCabinet
);
````
### 参数
`[in] hCab`  
一个有效的 Cabinet 句柄。句柄必须为[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)的返回值。
### 返回值
此函数没有返回值。
