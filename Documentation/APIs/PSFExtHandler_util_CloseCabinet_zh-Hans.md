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
一个有效的 Cabinet 句柄。句柄必须为[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)或[PSFExtHandler_util_CopyHandle](PSFExtHandler_util_CopyHandle_zh-Hans.md)的返回值。
### 返回值
此函数没有返回值。
### 备注
此函数将释放相应的内存，同时让 Cabinet 对象的引用计数减一。当引用计数归零时，会销毁对象，关闭 Cab 文件并释放内存。会设置 LastError。  
