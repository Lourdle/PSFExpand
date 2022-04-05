# PSF Extraction Handler API
This is the documentation for the PSF Extraction Handler API. If there is an application that developers want to do PSF file extension and do not want to do the PSF extraction function themselves, then they can consider using PSF Extraction Handler, although the implementation of these APIs is not particularly complicated, there will also be developers complaining about PSF Extraction Handler code writing bad. After all, the author is not a professional C++ programmer, and the programming habits are relatively casual (for example, do not like to write comments, variable names do not follow the general specifications, etc.), please forgive me for any shortcomings.
### Using the PSF Extraction Handler API
- Use the SDK directly without compiling the code. You can download the SDK directly from the [Releases page](https://github.com/Lourdle/PSFExpand/releases).
- Compile the source code. Clone or download the source code of PSFExpand, since PSFExpand and PSFExtractionHandler are in the same solution, you can propose that the PSFExtractionHandler project be generated separately or add to the desired solution.

Include the `PSFExtractionHandler.h` header file in the source program and import PSFExtractionHandler.lib to link the program with PSFExtractionHandler.dll.  
Of course, you can also load the DLL at runtime through the LoadLibrary function, and use the GetProcAddress function to get the function address.  
Note: If calling a function in a language other than C/C++, you need to change the calling convention to `cdecl`.  

If you don't want the program to depend on a DLL, you can choose to compile the PSFExtractionHandler into a static library, or add the source files directly to the project.
### PSF Extraction Handler Basic Processing Concepts
- The information of the PSF is encapsulated in the opaque PSF class, and the program uses the`PSF handle (hPSF)` (actually the pointer to the PSF class) to operate. For definitions of the PSF classes, see PSFExtHandlerFrame.h in the PSFExtractionHandler project.
- Strings should use `wide character strings (wchat_t*)`. Because multibyte strings in Windows have some limitations (such as not supporting the name length of up to 32,767 wide characters supported by NTFS, see [Maximum Path Length Limitation](https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation)). All file operations inside the PSF Extraction Handler convert it to an absolute path prefixed with `\\?\` before opening the file to support long filenames of 32,767 wide characters.
- All errors that occur can be obtained through GetLastError, the value of which is the same as that of Windows, for information on error codes, see [Debug system error codes](https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes).
### PSF Extraction Handler Reference
##### PSF Extraction Handler Functions
- [PSFExtHandler_ClosePSF](APIs/PSFExtHandler_ClosePSF_en.md)
- [PSFExtHandler_Expand](APIs/PSFExtHandler_Expand_en.md)
- [PSFExtHandler_ExtractFile](APIs/PSFExtHandler_ExtractFile_en.md)
- [PSFExtHandler_ExtractFileByIndex](APIs/PSFExtHandler_ExtractFileByIndex_en.md)
- [PSFExtHandler_ExtractFileToDirectory](APIs/PSFExtHandler_ExtractFileToDirectory_en.md)
- [PSFExtHandler_ExtractFileToDirectoryByIndex](APIs/PSFExtHandler_ExtractFileToDirectoryByIndex_en.md)
- [PSFExtHandler_GetFileCount](APIs/PSFExtHandler_GetFileCount_en.md)
- [PSFExtHandler_GetFileInfo](APIs/PSFExtHandler_GetFileInfo_en.md)
- [PSFExtHandler_GetLongestNameLength](APIs/PSFExtHandler_GetLongestNameLength_en.md)
- [PSFExtHandler_GetVersion](APIs/PSFExtHandler_GetVersion_en.md)
- [PSFExtHandler_GetVersionString](APIs/PSFExtHandler_GetVersionString_en.md)
- [PSFExtHandler_OpenFile](APIs/PSFExtHandler_OpenFile_en.md)
- [PSFExtHandler_OpenFileEx](APIs/PSFExtHandler_OpenFileEx_en.md)
##### PSF Extraction Handler Enumeration
- [PSFEXTHANDLER_FILE_TYPE](APIs/PSFEXTHANDLER_FILE_TYPE_en.md)
##### PSF Extraction Handler Structure
- [PSFEXTHANDLER_EXPAND_INFO](APIs/PSFEXTHANDLER_EXPAND_INFO_en.md)
##### PSF Extraction Handler Constants
- [PSFEXTHANDLER_EXTRACT_FLAG](APIs/PSFEXTHANDLER_EXTRACT_FLAG_en.md)
- [PSFEXTHANDLER_OPEN_FLAG](APIs/PSFEXTHANDLER_OPEN_FLAG_en.md)
- [PSFEXTHANDLER_VERSION](APIs/PSFEXTHANDLER_VERSION_en.md)
