# PSFExtHandler_CopyHandle
拷贝 PSF 句柄以供另一个线程使用。
````c
PSFEXTRACTIONHANDLER_API
HPSF
PSFExtHandler_CopyHandle(
    _In_        HPSF hPSF
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)或 PSFExtHandler_CopyHandle 的返回值。拷贝一个已经关闭的或其他对象的句柄，是未定义的行为。  
### 返回值
返回一个可供其他线程使用的句柄。
### 备注
复制句柄以便多线程并发操作对象。调用此函数会使对象引用计数自加一。不需要的句柄应及时关闭，且只有所有句柄关闭后 PSF 对象才会销毁。
