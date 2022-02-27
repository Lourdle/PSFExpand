## PSFExpand.exe /List Operation Command
Lists file information in the PSF from the specified XML file.
### Options
- XmlFile specifies the path of the XML file on the filesystem
- DisplayDetail shows the details of each file, including size (in bytes) and file type in PSF (PA30, PA19, raw file)
- Screen {condition} filters out files that meet specified criteria, either full filenames or filenames with file wildcards (\*, ?)
##### XmlFile Options
Required option. The usage method is to add `/XmlFile:<file path>` to the command line. Can be case-insensitive, does not support files with long path lengths.
##### DisplayDetail Options
Optional option. Add `/DisplayDetail` to the command line to enable.
##### Screen Options
Optional option. Add `/Screen` and add the criteria for the files you want to list in the latter parameter. After completion, the number of matching files will be displayed. If this option is used, it must be placed after all options, as all command line arguments after /Screen become filters. A filter is a string containing conditions that need to list files. Do not add a slash at the beginning of the string. It is not case sensitive. Use forward slashes `/` or backslashes `\\` for slashes in the path. is legal. For example, "/Screen \*.dll" means all files with dll extension; "/Screen historycix.cab" specifically refers to the historycix.cab file; if you add other options after "/Screen", such as "/Screen \*. dll /DisplayDetail" means the file whose full name is /DisplayDetail and whose extension is dll, obviously does not exist.
### Examples:
- Displays details of all files in PSF, whose XML is located at D:\express.psf.cix.xml
>PSFExpand.exe /List /DisplayDetail /XmlFile:D:\express.psf.cix.xml

- List all DLL files in PSF whose XML is located at D:\express.psf.cix.xml
>PSFExpand.exe /List /XmlFile:D:\express.psf.cix.xml /Screen \*.dll