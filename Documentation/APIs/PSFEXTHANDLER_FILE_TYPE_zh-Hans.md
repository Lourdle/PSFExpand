# PSFEXTHANDLER_FILE_TYPE
文件在 PSF 文件内被包含的格式。
````c
typedef enum _PSFEXTHANDLER_FILE_TYPE
{
    PSFEXTHANDLER_FILE_TYPE_RAW,
    PSFEXTHANDLER_FILE_TYPE_PA19,
    PSFEXTHANDLER_FILE_TYPE_PA30,
    PSFEXTHANDLER_FILE_TYPE_UNKNOWN
}PSFEXTHANDLER_FILE_TYPE;
````
### 常量
|常量                                 |值 |说明                                                   |
|-------------------------------------|---|-------------------------------------------------------|
|PSFEXTHANDLER_FILE_TYPE_RAW          |0  |在 PSF 直接以原始格式包含                              |
|PSFEXTHANDLER_FILE_TYPE_PA19         |1  |在 PSF 中以 PA19 格式包含，将调用 PatchAPI 进行处理    |
|PSFEXTHANDLER_FILE_TYPE_PA30         |2  |在 PSF 中以 PA30 格式包含，将调用 MSDelta 进行处理     |
|PSFEXTHANDLER_FILE_TYPE_UNKNOW       |3  |在 PSF 中包含的格式未知                                |
