# PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
The CAB file expansion state.
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
|Constant        |Value |Description                                |
|----------------|------|-------------------------------------------|
|State_WriteFile |0     |File to be written                         |
|State_CloseFile |1     |Completed file writing, will close handle  |
