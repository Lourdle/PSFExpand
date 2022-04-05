# PSFExtHandler_OpenFile
读取 XML 文件，并打开 PSF。
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HPSF
PSFExtHandler_OpenFileEx(
	_In_opt_    PCWSTR pPSFFile,
	_In_        PCWSTR pXmlFile,
	_Reserved_  PDWORD Reserved,
	_In_        WORD wFlags
);
````
### 参数
`[in, optional] pPSFFile`  
可选。一个 PSF 文件名字符串。如果为 NULL，则返回的 PSF 句柄只能用于文件信息查询，无法提取文件。

`[in] pXmlFile`  
XML 文件名的字符串。

`Reserved`  
保留，一般为 NULL。

`[in] wFlags`  
文件打开标识，参阅[PSFEXTHANDLER_OPEN_FLAG](PSFEXTHANDLER_OPEN_FLAG_zh-Hans.md)。
### 返回值
成功返回 PSF 句柄，失败返回 NULL。
### 备注
出现错误可查询 LastError。
