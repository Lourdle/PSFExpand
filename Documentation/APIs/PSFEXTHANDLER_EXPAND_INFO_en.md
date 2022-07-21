# PSFEXTHANDLER_EXPAND_INFO
The file expansion progress function provides file information.
````c
typedef struct _PSFEXTHANDLER_EXPAND_INFO
{
    DWORD       dwIndex;
    PCWSTR      pFileName;
    DWORD       dwCurrentFileSize;
    DWORD       dwCompletedBytes;
    DWORD       dwTotalBytes;
    ULONG       ulCompletedFileCount;
    ULONG       ulTotalFileCount;
}PSFEXTHANDLER_EXPAND_INFO;
````
### Members
##### dwIndex
The index value of this file.
##### pFileName
File name.
##### dwCurrentFileSize
The size of the file contained in the PSF file (if the format is not RAW, the actual size of the expansion is slightly larger than the size in the PSF file).
##### dwCompletedBytes
Completed data bytes.
##### dwTotalBytes
The total bytes of the file contained within the PSF.
##### ulCompletedFileCount
The number of files completed.
##### ulTotalFileCount
Total number of files.
