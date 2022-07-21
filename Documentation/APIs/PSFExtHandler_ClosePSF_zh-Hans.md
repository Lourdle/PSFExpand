# PSFExtHandler_ClosePSF
关闭 PSF 句柄。
````c
PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
    _In_        HPSF hPSF
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)或[PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_zh-Hans.md)的返回值。关闭一个已经关闭的或其他对象的句柄，是未定义的行为。
### 返回值
此函数没有返回值。
### 备注
此函数将释放相应的内存，同时让 PSF 对象的引用计数减一。当引用计数归零时，会销毁对象，关闭 PSF 文件并释放内存。会设置 LastError。  
如果参数为`NULL`或`INVALID_HANDLE_VALUE`不会引发异常，会设置 LastError 为`ERROR_INVALID_HANDLE`。  
