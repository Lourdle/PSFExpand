# PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
The CAB file expansion state.
````c
typedef enum _PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE
{
    PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE,
    PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE
}PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE;
````
### Constants
|Constant                                                    |Value |Description                                |
|------------------------------------------------------------|------|-------------------------------------------|
|PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE        |0     |File to be written                         |
|PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE        |1     |Completed file writing, will close handle  |
