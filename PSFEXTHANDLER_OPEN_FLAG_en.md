#PSFEXTHANDLER_OPEN_FLAG_en
Flags for opening files. Apply multiple identity via bitwise `OR`(`|`).
````c
#define PSFEXTHANDLER_OPEN_FLAG_SINGLE_THREAD                           ((WORD)0x0001)
````
>PSFEXTHANDLER_OPEN_FLAG_SINGLE_THREAD
>Use a single thread when reading XML files to avoid program failure due to the conflicting exception.
