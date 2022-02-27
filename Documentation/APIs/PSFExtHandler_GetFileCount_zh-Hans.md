# PSFExtHandler_GetFileCount
取得文件的数目。
````c
PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetFileCount(
    _In_        HPSF hPSF,
);
````
### 参数
hPSF\[in\]  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)的返回值。
### 返回值
返回文件的数目。若句柄无效，返回 0。
