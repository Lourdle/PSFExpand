# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO
CAB 文件展开函数报告进度信息。
````c
typedef union _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO
{
    const PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS* pProgressInfo;
    PHANDLE phFile;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO;
````
### 成员
##### pProgressInfo
指向进度信息结构的指针。
##### phFile
指向文件句柄的指针。
### 备注
可根据 pProgressInfo 获取进度信息，它的第一个成员为文件句柄。  
可通过第二个成员提供文件句柄。  
有关进度信息的更多信息，参阅[PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md)。
