# PSFExpand & PSFExtractionHandler Changelog

## PSFExpand
- 2.0.1.0
1. Adapt to API version 2.0.1, support setting file time.
2. Deprecated expanding the CAB file by calling Expand.exe and use the PSF Extraction Handler Cabinet utility function to improve the speed.
3. Fixed the problem that an error occurs when the file is expanded to the root directory.
4. Optimized the progress reporting mechanism to improve the speed.
5. Deprecated the "SafeRead" option.
6. Uses Shlwapi to match file wildcards.
7. Added the function of detecting whether the API version is supported at startup.
8. Other unlisted detail fixes.

- 1.1.1.0
1. Upgraded the default API version to 1.1.1.
2. Added "SafeRead" option.
3. Fixed several documentation errors.
4. Other unlisted detail fixes.

- 1.0.2.0
1. Fixed the problem of wrong file name when selecting PSF file.
2. Added the function of displaying related files before file expansion.
3. Optimized the file processing mechanism and support long file names.
4. Upgraded the default API version to 1.0.2.
5. Other unlisted detail fixes.

- 1.0.1.1
1. Fixed the problem that the filter fails to process '?'.
2. Added display language active selection feature.
3. Added CAB file expansion and automatic selection of PSF file and XML file.
4. Other unlisted detail fixes.


- 1.0.1.0
First release.


## PSFExtractionHandler
- 2.0.1
1. Optimized the file processing mechanism.
2. Added Cabinet utility function.
3. Deprecated multi-threaded reading of XML.
4. Added support for file time setting.
5. Functions that accept unsupported flags are automatically ignored instead of failing.
6. Other unlisted detail fixes.

Note: Earlier API versions will no longer be supported starting with this release. The behavior of unadapted programs calling some functions of the API after this version is undefined.

- 1.1.1
1. Canceled file write-through to improve file writing speed.
2. Fixed the problem that single thread does not expand the entire file.
3. Supports reading XML files with a single thread to avoid program failures caused by small probability conflict exceptions.
4. Other unlisted detail fixes.

- 1.0.2
1. Optimized the file processing mechanism and support long file names.
2. Added the feature of PSF file format validity check.
3. Optimize parallel multi-thread scheduling.
4. Other unlisted detail fixes.

- 1.0.1
First release.
