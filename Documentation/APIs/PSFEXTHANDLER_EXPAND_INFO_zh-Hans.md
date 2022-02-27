# PSFEXTHANDLER_EXPAND_INFO
文件展开进度函数提供文件信息。
````c
typedef struct _PSFEXTHANDLER_EXPAND_INFO
{
    DWORD       dwIndex;
    PCWSTR      pFileName;
    DWORD       dwCurrentFileSize;
    DWORD       dwCompletedBytes;
    DWORD       dwTotalBytes;
    DWORD       dwCompletedFileCount;
    DWORD       dwTotalFileCount;
}PSFEXTHANDLER_EXPAND_INFO;
````
### 成员
##### dwIndex
此文件的索引值。
##### pFileName
文件名。
##### dwCurrentFileSize
文件在 PSF 文件中包含的大小（如果格式不是 RAW，则展开的实际大小偏大于在 PSF 文件中的大小）。
##### dwCompletedBytes
已完成的数据字节。
##### dwTotalBytes
PSF 内包含文件的总字节。
##### dwCompletedFileCount
已完成的文件数。
##### dwTotalFileCount
总文件数。
