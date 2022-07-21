# PSFExtHandler_GetLongestNameLength
获取最长的文件名长度。
````c
PSFEXTRACTIONHANDLER_API
SHORT
PSFExtHandler_GetLongestFileNameLength(
    _In_        HPSF hPSF
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)或[PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_zh-Hans.md)的返回值。
### 返回值
返回最长的文件名长度。若句柄无效，返回 0。
