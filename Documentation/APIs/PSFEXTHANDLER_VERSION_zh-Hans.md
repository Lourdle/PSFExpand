# PSFEXTHANDLER_VERSION
PSF Extraction Handler API 的版本信息。

示例（版本 1.1.1）：
````c
#define PSFEXTHANDLER_MAJOR_VERSION                                 ((DWORD)1)
#define PSFEXTHANDLER_MINOR_VERSION                                 ((DWORD)1)
#define PSFEXTHANDLER_PATCH_VERSION                                 ((DWORD)1)

#define PSFEXTHANDLER_CURRENT_VERSION                               (PSFEXTHANDLER_MAJOR_VERSION << 22\
                                                                    | PSFEXTHANDLER_MINOR_VERSION << 12\
                                                                    | PSFEXTHANDLER_PATCH_VERSION << 2)
````
>PSFEXTHANDLER_MAJOR_VERSION  
>PSF Extraction Handler API 主要版本。

>PSFEXTHANDLER_MINOR_VERSION  
>PSF Extraction Handler API 次要版本。

>PSFEXTHANDLER_PATCH_VERSION  
>PSF Extraction Handler API 修补程序版本。

>PSFEXTHANDLER_CURRENT_VERSION  
>PSF Extraction Handler API 当前版本。这是 32 位数字，其前 10 位包含主要版本，接下来的 10 位包含次要版本，然后的 10 位包含修补程序版本，保留 2 位。

### 备注
主要版本、次要版本、修补程序版本最大值不超过 1023。
