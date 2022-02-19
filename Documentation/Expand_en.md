## PSFExpand.exe /Expand Operation Command
Expand the entire PSF file.
### Options
- XmlFile specifies the path of the XML file on the file system
- PsfFile specifies the path of the PSF file on the file system
- StorageDirectory specifies the directory where the extracted files are stored
- Verify to verify that the file data is intact
- SingleThread only uses single thread extension files
- NoProgressDisplay the entire file extension does not show progress
##### XmlFile Options
Required option. The usage method is to add `/XmlFile:<file path>` to the command line. Can be case-insensitive and does not support the use of UNC path specifications, so files with excessively long path lengths are not supported.
##### PsfFile Options
Required option. The usage method is to add `/PsfFile:<file path>` to the command line. Can be case insensitive.
##### StorageDirectory Options
Required option. To use it, add `/StorageDirectory:<folder path>` to the command line. Can be case-insensitive, so files with long path lengths are not supported.
##### Verify Options
Optional option. Adding the `/Verify` option to the program to perform a hash check before the data is written to the file. If the file data is corrupted, it will give a 'data invalid' error message.
##### SingleThread Options
Optional option. Adding the `/SingleThread` option the program will only use single-threaded extension files. For those who do not want the default multi-threaded extension to take up too many resources, it will reduce the speed. In fact, the speed of multi-threading is generally not much faster than that of single-threading, because the main limitation of PSF file expansion speed is the speed of hard disk read and write rather than the single-threaded speed of CPU.
##### NoProgressDisplay
Optional option. Adding the `/NoProgressDisplay` option program will not show progress. The effect on the speed of expanding the file is not obvious.
### For example
- Expand the D:\Windows10.0-KB0000000-x64.psf file where the XML that saves the file information is D:\Windows10.0-KB0000000-x64\express.psf.cix.xml, and check the file hash.
>PSFExpand.exe /Expand /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /PsfFile:D:\Windows10.0-KB0000000-x64.psf /StorageDirectory:D:\Windows10. 0-KB0000000-x64/Verify