# PSF File Expansion Utility
Portions of the English readme and documentation may be machine translated.  
- English(Current)
- [简体中文](README_zh-Hans.md)

Lists, extracts, expands Windows PSF updates.
### Instructions
The use of PSF files is inseparable from the XML that describes the information of the PSF files, generally express.psf.cix.xml in the corresponding CAB file. You can use Expand.exe to first expand the CAB file corresponding to the PSF. Then use PSFExpand.exe to expand the PSF.
### PSFExpand provides operation commands
- [List](Documentation/List_en.md)
- [Extract](Documentation/Extract_en.md)
- [Expand](Documentation/Expand_en.md)
### API
See [PSF Extraction Handler API](Documentation/APIs_en.md) documentation.
### Operating Conditions
The operating system must be Windows Vista or later. Because the file extraction function of the PSF Extraction Handler API calls MSDelta functions.
### Building
The platform toolset for both projects is v143, which means Visual Studio 2022 or newer is required. The platform toolset version can also be manually modified to support earlier versions of Visual Studio.  
The Windows SDK needs to be installed, not necessarily the latest version.
