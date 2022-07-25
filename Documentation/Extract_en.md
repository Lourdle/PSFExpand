## PSFExpand.exe /Extract operation command
Extract the file specified in the PSF
### Options
- XmlFile specifies the path of the XML file on the filesystem
- PsfFile specifies the path of the PSF file on the filesystem
- Filename specifies one or more file names to be extracted. Wildcards can list multiple matching file names.
- OutDir specifies the directory where the extracted files are stored
- Verify to verify that the file data is intact
- Verbose verbose mode, prints the filename in the console window every time a file is extracted
##### XmlFile Option
Required option. The usage method is to append `/XmlFile:<file path>` to the command line. Can be case insensitive.
##### PsfFile Option
Required option. The usage method is to append `/PsfFile:<file path>` to the command line. Can be case insensitive.
##### File Options
Required option. Append the filename directly (without the slash "/") to the command line. Multiple files can be specified, case-insensitive. The program will extract all matching files.
##### OutDir Option
Required option. To use it, append `/OutDir:<folder path>` to the command line. Can be case insensitive.
##### Verify Option
Optional option. Append the `/Verify` option to the program to perform a hash check before the data is written to the file. If the file data is corrupted, it will give a `data invalid` error message.
##### Verbose Option
Optional option. Appendi the `/Verbose` option to the program will output the filename to the console after successfully writing a file.
### Examples:
- Extract and verify the historycix.cab file in D:\Windows10.0-KB0000000-x64.psf from D:\Windows10.0-KB0000000-x64\express.psf.cix.xml that saves the file information and verify it Errors are stored in D:\Windows10.0-KB0000000-x64.
>PSFExpand.exe /Extract /PsfFile:D:\Windows10.0-KB0000000-x64.psf historycix.cab /Verify /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /OutDir:D:\Windows10.0-KB0000000-x64

- Extract all files in D:\Windows10.0-KB0000000-x64.psf from D:\Windows10.0-KB0000000-x64\express.psf.cix.xml to save the file information and store them in D:\Windows10 .0-KB0000000-x64, display each extracted file during extraction (it is not recommended to extract all files, it is recommended to use ['Expand' operation command](Expand_en.md), unless you want to display each extracted file).
>PSFExpand.exe /Extract /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml * /PsfFile:D:\Windows10.0-KB0000000-x64.psf /OutDir:D:\Windows10.0-KB0000000-x64 /Verify