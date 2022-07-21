# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
CAB 文件展开进度信息。
````c
typedef struct _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
{
    HANDLE hFile
    ULONG ulComplitedFiles;
    ULONG ulTotalFiles;
    PCSTR pCurrentFile;
    ULONG ulSize;
    FILETIME FileTime;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS;
````
### 成员
##### hFile
文件句柄。
##### ulComplitedFiles
已完成的文件数目。
##### ulTotalFiles
总文件数。
##### pCurrentFile
当前文件名。
##### ulSize
当前文件大小。
##### FileTime
文件时间。
