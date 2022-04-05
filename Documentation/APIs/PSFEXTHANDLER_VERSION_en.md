# PSFEXTHANDLER_VERSION
Version information for the PSF Extraction Handler API.

Example (Version 1.1.1):
````c
#define PSFEXTHANDLER_MAJOR_VERSION                                 ((DWORD)1)
#define PSFEXTHANDLER_MINOR_VERSION                                 ((DWORD)1)
#define PSFEXTHANDLER_PATCH_VERSION                                 ((DWORD)1)

#define PSFEXTHANDLER_CURRENT_VERSION                               (PSFEXTHANDLER_MAJOR_VERSION << 22\
                                                                    | PSFEXTHANDLER_MINOR_VERSION << 12\
                                                                    | PSFEXTHANDLER_PATCH_VERSION << 2)
````
>PSFEXTHANDLER_MAJOR_VERSION
>PSF Extraction Handler API major version.

>PSFEXTHANDLER_MINOR_VERSION
>PSF Extraction Handler API minor version.

>PSFEXTHANDLER_PATCH_VERSION
>PSF Extraction Handler API patch version.

>PSFEXTHANDLER_CURRENT_VERSION
>PSF Extraction Handler API current version. It is a 32-bit number with the first 10 bits containing the major version, the next 10 bits containing the minor version, then the next 10 bits containing the patch version, 2 bits reserved.

### Remark
Major version, minor version, patch version maximum is 1023.
