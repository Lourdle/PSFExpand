# PSFExtHandler_GetVersion
获取 PSF Extraction Handler API 版本
````c
PSFEXTRACTIONHANDLER_API
DWORD
PSFExtHandler_GetVersion();
````
### 参数
无。
### 返回值
返回 32 位数字，其前 10 位包含主要版本，接下来的 10 位包含次要版本，然后的 10 位包含修补程序版本，保留 2 位。
### 备注
主要、次要、修补版本在头文件有定义，参阅[PSFEXTHANDLER_VERSION](PSFEXTHANDLER_VERSION_zh-Hans.md)。  
此函数可以获得运行时的版本，也就是说，返回编译时头文件定义的值 (WIMLIB_MAJOR_VERSION << 22) | (WIMLIB_MINOR_VERSION << 12) | (WIMLIB_PATCH_VERSION << 2)。
