# PSFExtHandler_util_OpenCabinet
Open a CAB file.
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HANDLE PSFExtHandler_util_OpenCabinet(
    _In_        PCWSTR pCabFile
);
````
### Parameters
`[in] pCabFile`  
A string for the CAB filename.
### Return Value
Returns the Cabinet handle on success, NULL on failure.
### Remark
If an error occurs, you can query LastError.
