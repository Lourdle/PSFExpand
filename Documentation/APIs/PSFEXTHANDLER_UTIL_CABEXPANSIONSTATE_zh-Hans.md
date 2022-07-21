# PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
CAB 文件展开状态。
````c
typedef enum _PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
{
    PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE,
    PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE
}PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE;
````
### 常量
|常量                                                        |值 |说明                            |
|------------------------------------------------------------|---|--------------------------------|
|PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE        |0  |即将写文件                      |
|PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE        |1  |已完成文件写入，即将关闭句柄    |
