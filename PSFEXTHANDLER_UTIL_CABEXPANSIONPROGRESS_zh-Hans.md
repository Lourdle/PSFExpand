# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
CAB 文件展开进度函数提供文件信息。
````c
typedef struct _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
{
	WORD wComplitedFiles;
	WORD wTotalFiles;
	PCSTR pCurrentFile;
	WORD wCurrentFileWrittenSize;
	FILETIME FileTime;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS;
````
### 成员
##### wComplitedFiles
已完成的文件数目。
##### wTotalFiles
总文件数。
##### pCurrentFile
当前文件名。
##### wCurrentFileWrittenSize
当前文件已写入的大小。
##### FileTime
文件时间。
