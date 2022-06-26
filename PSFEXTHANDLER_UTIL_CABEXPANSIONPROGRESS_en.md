# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
The CAB file expansion progress function provides file information.
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
### Members
##### wComplitedFiles
The number of files completed.
##### wTotalFiles
Total number of files.
##### pCurrentFile
The current filename.
##### wCurrentFileWrittenSize
The size of the current file that has been written.
##### FileTime
The file time.
