# PSFEXTHANDLER_EXTRACT_FLAG
Identifier for file extraction or expansion. Apply multiple identity via bitwise `OR`(`|`).
````c
#define PSFEXTHANDLER_EXTRACT_FLAG_VERIFY								0x0001
#define PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS							0x0002
#define PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS						0x0004
#define PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD						0x0008
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA						0x0010
#define PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS		0x0020
#define PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE					0x0040
#define PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT					0x0080
#define PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD\
																		0x0100
#define PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY		0x0200
````
>PSFEXTHANDLER_EXTRACT_FLAG_VERIFY
> Verify that the data is intact before writing to the file.

>PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS
> Skip if the file already exists when writing the file.

>PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS
> If the file exists when writing the file, the function fails.

>PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD
> For the PSFExtHandler_Expand function, use a single thread when expanding the file. Other file extraction functions are themselves single-threaded extractions and ignore this flag.

>PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA
> Incomplete verification file is required to write file.

>PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS
> Continue function execution when some operation fails but the error is non-fatal.

>PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE
>Write the file to a provided handle instead of the specified file.

>PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT
>Keep writing files in PSF file format.

>PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD
> For the PSFExtHandler_Expand function, it is allowed not to call the main thread (the thread that called PSFExtHandler_Expand) when calling the progress reporting function during file expansion. Other extraction functions do not accept this flag.

>PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
> For the PSFExtHandler_Expand function, the progress reporting function is called synchronously during file expansion. Other functions do not accept this flag.

### Remarks
1. >PSF Extraction Handler All extraction functions that write files and find files with the same name will overwrite the files by default unless the corresponding identifier is specified. PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS and PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS flag conflict, do not accept both.

2. >Specify that the PSFEXTHANDLER_EXTRACT_FLAG_WRITE_BAD_DATA flag should also be matched with PSFEXTHANDLER_EXTRACT_FLAG_VERIFY. When the two exist at the same time, if the file is damaged, it will continue, and if successful, it will return TRUE, and will set LastError to ERROR_INVALID_DATA.

3. >Use the PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE flag, a handle should be provided in the output file name parameter, and the provided handle must be guaranteed to be writable, and an IO device handle (such as a file, pipe, etc.) can be provided here. Only the PSFExtHandler_ExtractFileByIndex and PSFExtHandler_ExtractFile functions are available.
   >> Example:
   >>BOOL ret = PSFExtHandler_ExtractFileByIndex(hPSF, 0, (PCWSTR)hFile, PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE);

4. > Specifying PSFEXTHANDLER_EXTRACT_FLAG_KEEP_ORIGINAL_FORMAT will not process the data. For unsupported file types, the file can only be written with this flag.

5. >- If the PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD flag is specified the PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD and PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY flags are ignored.
   >- After specifying PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD | PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY, each thread will wait for the progress report function to return before calling the progress report function after the function returns. PSFExtHandler_Expand will ensure that only one thread will call the progress reporting function at the same time.
   >- Be careful to only specify PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY, so each thread will wait for the progress report function to return before calling the progress report function after completing a file extraction. Multiple threads will send progress information messages to the main thread through the message queue. When the main thread's thread report function Continue only after returning. However, it takes a long time to send messages synchronously in this way, and the main reason for limiting the file expansion speed is the read and write speed of the hard disk rather than the single-core computing speed of the CPU. Not as fast as a single thread.
   >- Do not specify the PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY and PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD flags, each thread will add a progress information message to the message queue after completing the file extraction operation, and then continue to extract the next file. The main thread will call the progress report function. After the function returns, the memory space occupied by the message will be released, and the next message will be processed.