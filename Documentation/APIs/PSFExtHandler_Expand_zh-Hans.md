# PSFExtHandler_Expand
扩展 PSF 的所有文件。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExpandPSF(
    _In_        HPSF hPSF,
    _In_opt_    PCWSTR pOutDir,
    _In_        DWORD dwExtractionFlags,
    _In_opt_    PSFEXTHANDLER_PROGRESS_PROC pfProgressCallback,
    _In_opt_    PVOID pvUserData
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)或[PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_zh-Hans.md)的返回值。

`[in, optional] pOutDir`  
可选。输出文件存储的文件夹的相对路径或绝对路径。如果为 NULL，将展开文件存放到程序当前的处理目录下。

`[in] dwExtractionFlags`
用于文件展开的提取标识。有关提取标识的信息，参阅[PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_zh-Hans.md)。

`[in, optional] pfProgressCallback`  
可选。一个定义的返回值类型为 BOOL 参数为[PSFEXTHANDLER_EXPAND_INFO](PSFEXTHANDLER_EXPAND_INFO_zh-Hans.md)结构的常量指针和用户定义的无类型指针的函数指针。  
用户定义一个 BOOL(const [PSFEXTHANDLER_EXPAND_INFO](PSFEXTHANDLER_EXPAND_INFO_zh-Hans.md)\*, PVOID) 的 C 调用约定的函数，在文件扩展过程中完成每个文件都会调用一次提供的函数报告进度。函数的返回值不应为 FALSE，若返回 FALSE 则认为用户取消了操作并尝试停止，设置 LastError 为 ERROR_CANCELLED。  
如果为 NULL，则认为展开过程不需要知道进度。  
`[in, optional] pvUserData`  
可选。用户定义的提供给进度报告函数的自定义数据。
### 返回值
成功返回 TRUE，失败返回 FALSE。
### 备注
当 dwExtractionFlags 提供了 PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS 标识，当其中一个文件提取操作失败扩展任然会继续，完成后会返回 FALSE。  
根据提供的提取标识，函数返回 TRUE 但 LastError 也可能不为 ERROR_SUCCESS，详情参阅 [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_zh-Hans.md)。
