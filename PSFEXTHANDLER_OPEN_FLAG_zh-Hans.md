#PSFEXTHANDLER_OPEN_FLAG
用于打开文件的标识。通过按位`或`(OR)`|`应用多个标识。
````c
#define PSFEXTHANDLER_OPEN_FLAG_SINGLE_THREAD                           ((WORD)0x0001)
````
>PSFEXTHANDLER_OPEN_FLAG_SINGLE_THREAD
>读取 XML 文件时使用单线程以避免因冲突引发的异常导致程序失败。
