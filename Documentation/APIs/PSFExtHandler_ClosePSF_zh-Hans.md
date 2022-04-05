# PSFExtHandler_ClosePSF
关闭打开的 PSF 句柄。
````c
PSFEXTRACTIONHANDLER_API
VOID
PSFExtHandler_ClosePSF(
    _In_        HPSF hPSF
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)的返回值。关闭一个已经关闭的或其他对象的句柄，是未定义的行为。
### 返回值
此函数没有返回值。
### 备注
此函数将释放保存 PSF 所用的内存，同时关闭已经打开的 PSF 文件，会设置 LastError。  
如果参数为`NULL`或`INVALID_HANDLE_VALUE`不会引发异常，会设置 LastError 为`ERROR_INVALID_HANDLE`。  
由于 PSF Extraction Handler 的 PSF 类没有锁，对多线程并发调用此函数同时还有线程通过此 PSF 句柄进行操作，其行为是未定义的。
