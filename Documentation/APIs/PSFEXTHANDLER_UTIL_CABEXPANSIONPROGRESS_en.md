# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
CAB file expansion progress information.
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
### Members
##### hFile
A handle to the file.
##### ulComplitedFiles
The number of files completed.
##### ulTotalFiles
Total number of files.
##### pCurrentFile
The current filename.
##### ulCurrentFileWrittenSize
The size of the current file.
##### FileTime
The file time.
