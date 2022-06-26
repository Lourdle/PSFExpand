# PSF File Expansion Utility
Some contents of English readme file, documentation and changelog may be machine translated.
- English (Current)
- [简体中文](README_zh-Hans.md)

Lists, extracts, expands Windows PSF updates.
### Instructions
The use of PSF files is inseparable from the XML that describes the information of the PSF files, generally express.psf.cix.xml in the corresponding CAB file.  
The CAB file can be expanded manually and then the PSF file can be expanded using this tool.  
It is recommended to use this tool to expand CAB files and PSF files, because running Expand.exe outputs too much file name information in the console, which seriously affects the speed. This tool will call the Cabinet API to expand the CAB file, which greatly improves the speed.
### PSFExpand Languages
The application is available in Simplified Chinese and English, and the appropriate language will be selected at runtime under normal circumstances. The language can be specified explicitly using the `/Lang` option.  
Specify the /Lang option to select the display language. To use it, add "/Lang:\<Language>" to the command line immediately following the application name.

Supported languages:
- en English
- zh-Hans 简体中文

Example:
- Use Simplified Chinese as the display language and print help.
>PSFExpand.exe /Lang:zh-Hans /?

If an unsupported language is specified, the program will use English as the display language.
### PSFExpand provides operation commands.
- [List](Documentation/List_en.md)
- [Extract](Documentation/Extract_en.md)
- [Expand](Documentation/Expand_en.md)
### API
See [PSF Extraction Handler API](Documentation/APIs_en.md) documentation.
### Program Operating Conditions
The operating system must be Windows Vista or later. Because the file extraction function of the PSF Extraction Handler API calls MSDelta functions.
### Building
The platform toolset for both projects is v143, which means Visual Studio 2022 or newer is required. The platform toolset version can also be manually modified to support earlier versions of Visual Studio.  
The Windows SDK needs to be installed, not necessarily the latest version.
### Changelog
See [CHANGELOG.md](CHANGELOG.md).
