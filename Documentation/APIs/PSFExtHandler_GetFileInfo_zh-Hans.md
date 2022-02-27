# PSFExtHandler_GetFileInfo
根据索引获取文件信息。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_GetFileInfo(
    _In_        HPSF hPSF,
    _In_        DWORD dwIndex,
    _Out_writes_bytes_to_opt_(*pcbData, *pcbData) PWSTR pszFileName,
    _When_(pszFileName == NULL, _Out_opt_) _When_(pszFileName != NULL, _Inout_opt_) PDWORD pcbData,
    _Out_opt_   PDWORD pdwFileSize,
    _Out_opt_   PSFEXTHANDLER_FILE_TYPE* Type
);
````
### 参数
hPSF\[in\]  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)的返回值。

dwIndex\[in\]  
目标文件索引。文件索引的顺序与 XML 文件内文件的顺序一致。索引以`0`开始，最大为`文件数 -1`。要获取文件数，参阅[PSFExtHandler_GetFileCount](PSFExtHandler_GetFileCount_zh-Hans.md)。

pszFileName\[out, optional\]  
可选。指向接收文件名的缓冲区指针。

pcbData\[in, out, optional\]  
可选。仅在 pszFileName 为 NULL 时，此参数才能为 NULL。  
参数接收文件名包括空字符的大小，单位字节。  
当文件名缓冲区不够大，将返回 FALSE，设置 LastError 为 ERROR_MORE_DATA。  
若 pszFileName 为 NULL，pcbData 不为 NULL，在接收数据大小后返回 TRUE，设置 LastError 为 ERROR_SUCCESS。

pdwFileSize\[out, optional\]  
可选。接收文件的大小，单位字节。

Type\[out, optional\]  
可选，接收该文件的类型。有关类型的信息，参阅[PSFEXTHANDLER_FILE_TYPE](PSFEXTHANDLER_FILE_TYPE_zh-Hans.md)。  
### 返回值
成功返回 TRUE，失败返回 FALSE。
### 备注
如果索引值不合法，返回 FALSE，设置 LastError 为 ERROR_INVALID_PARAMETER。
