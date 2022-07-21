# PSFExtHandler_ExtractFileToDirectory
根据一个提供的文件名提取文件到指定目录。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
BOOL
PSFExtHandler_ExtractFileToDirectory(
    _In_        HPSF hPSF,
    _In_        PCWSTR pTargetFileName,
    _In_opt_    PCWSTR pOutDir,
    _In_opt_    PCWSTR pOutFileName,
    _In_        WORD wExtractionFlags
);
````
### 参数
`[in] hPSF`  
一个有效的 PSF 句柄。句柄必须为[PSFExtHandler_OpenFile](PSFExtHandler_OpenFile_zh-Hans.md)或[PSFExtHandler_OpenFileEx](PSFExtHandler_OpenFileEx_zh-Hans.md)或[PSFExtHandler_CopyHandle](PSFExtHandler_CopyHandle_zh-Hans.md)的返回值。

`[in] pTargetFileName`  
目标文件名。提供一个在 PSF 文件内欲提取的完整文件名。不区分大小写。

`[in, optional] pOutDir`  
可选。输出文件存储的文件夹的相对路径或绝对路径。如果为 NULL，将文件输出到程序当前的处理目录下。

`[in, optional] pOutFile`  
可选。输出文件名称存储在文件夹。如果为 NULL，将文件保持原来命名存放到存储文件夹。

`[in] wExtractionFlags`  
提取标识。有关提取标识的信息，参阅[PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_zh-Hans.md)。
### 返回值
成功返回 TRUE，失败返回 FALSE。
### 备注
此函数不可使用 PSFEXTHANDLER_EXTRACT_WRITE_DATA_TO_HANDLE。  
当 wExtractionFlags 提供了 PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS | PSFEXTHANDLER_EXTRACT_FLAG_VERIFY 标识，文件校验操作失败（即未能成功得到提取文件的 Hash）提取任然会继续，完成后如果文件正常提取会返回 TRUE，会设置 LastError。若提供 PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS 又未提供 PSFEXTHANDLER_EXTRACT_FLAG_VERIFY 标识，则忽略 PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS。若提供 PSFEXTHANDLER_EXTRACT_FLAG_VERIFY 而不提供 PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS 表示，验证操作失败将返回 FALSE，会设置 LastError。  
根据提供的提取标识，函数返回 TRUE 但 LastError 也可能不为 ERROR_SUCCESS，详情参阅 [PSFEXTHANDLER_EXTRACT_FLAG](PSFEXTHANDLER_EXTRACT_FLAG_zh-Hans.md)。
