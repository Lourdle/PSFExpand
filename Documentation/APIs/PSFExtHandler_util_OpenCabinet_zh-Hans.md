# PSFExtHandler_util_OpenCabinet
打开一个 CAB 文件。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HANDLE PSFExtHandler_util_OpenCabinet(
    _In_        PCWSTR pCabFile
);
````
### 参数
`[in] pCabFile`  
CAB 文件名的字符串。
### 返回值
成功返回 Cabinet 句柄，失败返回 NULL。
### 备注
出现错误可查询 LastError。
