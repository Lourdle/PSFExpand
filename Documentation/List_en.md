## PSFExpand.exe /List Operation Command
Lists file information in the PSF from the specified XML file.
### Options
- XmlFile specifies the path of the XML file on the filesystem
- DisplayDetail shows the details of each file, including size (in bytes) and file type in PSF (PA30, PA19, raw file)
- Filename specifies one or more file names to be listed. Wildcards can list multiple matching file names.
##### XmlFile Option
Required option. The usage method is to append `/XmlFile:<file path>` to the command line. Can be case insensitive.
##### DisplayDetail Option
Optional option. Append `/DisplayDetail` to the command line to enable.
##### Filename Options
Optional option. Append the filename directly (without the slash "/") to the command line. Multiple files can be specified, case-insensitive. The program will list all matching files.
### Examples:
- Displays details of all files in PSF, whose XML is located at D:\express.psf.cix.xml
>PSFExpand.exe /List /DisplayDetail /XmlFile:D:\express.psf.cix.xml

- List all DLL files in PSF whose XML is located at D:\express.psf.cix.xml.
>PSFExpand.exe /List /XmlFile:D:\express.psf.cix.xml \*.dll