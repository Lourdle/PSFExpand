# PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
CAB 文件展开状态。
````c
typedef enum _PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
{
	State_WriteFile,
	State_CloseFile
}PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE;
````
### 常量
|常量                   |值 |说明                            |
|-----------------------|---|--------------------------------|
|State_WriteFile        |0  |即将写文件                      |
|State_CloseFile        |1  |已完成文件写入，即将关闭句柄    |
