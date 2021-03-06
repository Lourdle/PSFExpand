# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO
The CAB file expansion function reports progress information.
````c
typedef union _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO
{
    const PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS* pProgressInfo;
    PHANDLE phFile;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO;
````
### Members
##### pProgressInfo
A pointer to progress information structu.
##### phFile
A pointer to a handle to the file.
### Remarks
Progress information can be obtained according to pProgressInfo, whose first member is the file handle.  
A file handle can be provided through the second member.  
For more information on progress messages, see [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_en.md).
