# PSFExtHandler_util_ExpandCabinet
````c
PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_util_ExpandCabinet(
    _In_        HANDLE hCabinet,
    _In_        PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfnProgressCallback,
    _In_opt_    PVOID pvUserData
);
````
### 参数
`[in] hCab`  
一个有效的 Cabinet 句柄。句柄必须一个有效的 Cabinet 句柄。句柄必须为[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)或[PSFExtHandler_util_CopyHandle](PSFExtHandler_util_CopyHandle_zh-Hans.md)的返回值。

`[in, optional] pfProgressCallback`  
可选。一个定义的返回值为 BOOL 参数为[PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md)枚举，[PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md)结构的常量指针，指向句柄的指针和用户定义的无类型指针的函数指针。

定义一个 BOOL([PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md), [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md)\*, PVOID) 的 C 调用约定的函数，在文件扩展过程中完成每个文件都会调用一次提供的函数报告进度。  
第一个参数提供当前的状态。当状态为 `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE` 时可通过指向句柄的指针提供文件句柄。若指针指向的句柄为 NULL（默认），则以默认文件名在程序的当前目录创建文件。否则将数据写入提供的句柄。若设置为 `INVALID_HANDLE_VALUE` 则认为用户需要跳过此文件。当状态为 `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE`，在提供了句柄的情况下此回调函数需负责关闭句柄，函数不会设置文件时间；若指针指向的句柄为 `NULL`，函数会自动设置文件时间。  
函数应该返回 TRUE。若返回 FALSE 则认为用户要取消操作，此函数将取消操作并设置 LastError 为 `ERROR_CANCELLED`。
如果为 NULL，则认为展开过程不需要知道进度。  

`[in, optional] pvUserData`  
可选。用户定义的提供给进度报告函数的自定义数据。

### 返回值
成功返回 TRUE，失败返回 FALSE。
### 备注
出现错误可查询 LastError。
