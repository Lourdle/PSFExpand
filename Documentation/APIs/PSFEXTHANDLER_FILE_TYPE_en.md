# PSFEXTHANDLER_FILE_TYPE
The format in which the file is contained within the PSF file.
````c
typedef enum _PSFEXTHANDLER_FILE_TYPE
{
	PSFEXTHANDLER_FILE_TYPE_RAW,
	PSFEXTHANDLER_FILE_TYPE_PA19,
	PSFEXTHANDLER_FILE_TYPE_PA30,
	PSFEXTHANDLER_FILE_TYPE_UNKNOWN
}PSFEXTHANDLER_FILE_TYPE;
````
### Constants
|Constant                       |Value |Description                                                        |
|-------------------------------|------|-------------------------------------------------------------------|
|PSFEXTHANDLER_FILE_TYPE_RAW    |0     |Include in PSF directly in raw format                              |
|PSFEXTHANDLER_FILE_TYPE_PA19   |1     |Contained in PSF in PA19 format, will call PatchAPI for processing |
|PSFEXTHANDLER_FILE_TYPE_PA30   |2     |Contained in PSF in PA30 format, will call MSDelta for processing  |
|PSFEXTHANDLER_FILE_TYPE_UNKNOW |3     |Unknown format contained in PSF                                    |