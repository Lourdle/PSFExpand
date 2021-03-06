# PSF Extraction Handler API 中文文档
这是 PSF Extraction Handler API 的中文文档。这些 APIs 的实现不算特别复杂，可能代码还写的不好。毕竟作者不是专业的 C++ 程序员，编程习惯比较随便。
### 使用 PSF Extraction Handler API
- 直接使用 SDK 而不编译代码。可以在 [Releases 页面](https://github.com/Lourdle/PSFExpand/releases) 直接下载 SDK。
- 编译源代码。克隆或下载 PSFExpand 源代码，由于 PSFExpand 与 PSFExtractionHandler 在同一解决方案，可以提出 PSFExtractionHandler 项目单独生成或添加到所需解决方案中。

在源程序中包括`PSFExtractionHandler.h`头文件，导入 PSFExtractionHandler.lib 使程序与 PSFExtractionHandler.dll 链接。  
当然，也可以通过 LoadLibrary 函数在运行时加载 DLL，使用 GetProcAddress 函数获取函数地址。  
注意：如果在非 C/C++ 语言中调用函数，需要将调用约定为`cdecl`。  

如果不想让程序依赖于一个 DLL 可以选择把 PSFExtractionHandler 编译成静态库，或者直接把源文件添加到项目中。  
### PSF Extraction Handler 基本处理概念
- PSF 的信息是封装在不透明的 PSF 类中，程序使用`PSF 句柄（hPSF）`（其实就是 PSF 类的指针）进行操作。有关 PSF 类的定义，请参阅 PSFExtractionHandler 项目中的 PSFExtHandlerFrame.h。
- 字符串应使用`宽字符串（wchat_t*）`。因为 Windows 中多字节串具有一些局限性（如不支持 NTFS 最多支持的 32, 767 宽字符的名称长度，详情参阅[最大路径长度限制](https://docs.microsoft.com/zh-cn/windows/win32/fileio/maximum-file-path-limitation)）。PSF Extraction Handler 内部的文件操作都是先将其转换为`\\?\`前缀的绝对路径，然后再打开文件，以支持 32, 767 宽字符的长文件名。
- 所有发生的错误都可以通过 GetLastError 获得，其值与 Windows 的错误值相同，有关错误码的信息，参阅[调试系统错误代码](https://docs.microsoft.com/zh-cn/windows/win32/debug/system-error-codes)。
### PSF Extraction Handler 参考
##### PSF Extraction Handler 函数
- [PSFExtHandler_ClosePSF](APIs/PSFExtHandler_ClosePSF_zh-Hans.md)
- [PSFExtHandler_CopyHandle](APIs/PSFExtHandler_CopyHandle_zh-Hans.md)
- [PSFExtHandler_Expand](APIs/PSFExtHandler_Expand_zh-Hans.md)
- [PSFExtHandler_ExtractFile](APIs/PSFExtHandler_ExtractFile_zh-Hans.md)
- [PSFExtHandler_ExtractFileByIndex](APIs/PSFExtHandler_ExtractFileByIndex_zh-Hans.md)
- [PSFExtHandler_ExtractFileToDirectory](APIs/PSFExtHandler_ExtractFileToDirectory_zh-Hans.md)
- [PSFExtHandler_ExtractFileToDirectoryByIndex](APIs/PSFExtHandler_ExtractFileToDirectoryByIndex_zh-Hans.md)
- [PSFExtHandler_GetFileCount](APIs/PSFExtHandler_GetFileCount_zh-Hans.md)
- [PSFExtHandler_GetFileInfo](APIs/PSFExtHandler_GetFileInfo_zh-Hans.md)
- [PSFExtHandler_GetLongestFileNameLength](APIs/PSFExtHandler_GetLongestFileNameLength_zh-Hans.md)
- [PSFExtHandler_GetVersion](APIs/PSFExtHandler_GetVersion_zh-Hans.md)
- [PSFExtHandler_GetVersionString](APIs/PSFExtHandler_GetVersionString_zh-Hans.md)
- [PSFExtHandler_OpenFile](APIs/PSFExtHandler_OpenFile_zh-Hans.md)
- [PSFExtHandler_OpenFileEx](APIs/PSFExtHandler_OpenFileEx_zh-Hans.md)
- [PSFExtHandler_util_CloseCabinet](APIs/PSFExtHandler_util_CloseCabinet_zh-Hans.md)
- [PSFExtHandler_util_CabinetCopyHandle](APIs/PSFExtHandler_util_CabinetCopyHandle_zh-Hans.md)
- [PSFExtHandler_util_ExpandCabinet](APIs/PSFExtHandler_util_ExpandCabinet_zh-Hans.md)
- [PSFExtHandler_util_OpenCabinet](APIs/PSFExtHandler_util_OpenCabinet_zh-Hans.md)
##### PSF Extraction Handler 枚举
- [PSFEXTHANDLER_FILE_TYPE](APIs/PSFEXTHANDLER_FILE_TYPE_zh-Hans.md)
- [PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](APIs/PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md)
##### PSF Extraction Handler 结构
- [PSFEXTHANDLER_EXPAND_INFO](APIs/PSFEXTHANDLER_EXPAND_INFO_zh-Hans.md)
- [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](APIs/PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md)
##### PSF Extraction Handler 联合
- [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](APIs/PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md)
##### PSF Extraction Handler 常量
- [PSFEXTHANDLER_EXTRACT_FLAG](APIs/PSFEXTHANDLER_EXTRACT_FLAG_zh-Hans.md)
- [PSFEXTHANDLER_OPEN_FLAG](APIs/PSFEXTHANDLER_OPEN_FLAG_zh-Hans.md)
- [PSFEXTHANDLER_VERSION](APIs/PSFEXTHANDLER_VERSION_zh-Hans.md)
### PSF Extraction Handler 线程安全
不管是 PSF 对象还是 Cabinet 对象，内部都有全局活动事件，文件活动事件和句柄活动事件。  
当调用查询文件信息的相关函数时，会直接进行相关操作并返回。若执行文件提取的函数，会设置活动事件使得别的线程调用这类函数且使用同一句柄时会等待其完成。文件活动事件用于多线程异步读取文件。全局活动事件在拷贝句柄或关闭句柄时内部访问同一资源使用。  
若程序要多线程使用提取函数使用同一对象，可先拷贝该对象的任意句柄，然后调用相关的函数并使用拷贝函数返回的句柄。这样就能安全地操作相应的对象，不用的句柄应及时关闭。
