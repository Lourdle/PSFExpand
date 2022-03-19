## PSFExpand.exe /Expand Operation Command
Expand the entire PSF file.
### File info options
- CabFile specifies the path of the CAB file on the filesystem
- XmlFile specifies the path of the XML file on the filesystem
- PsfFile specifies the path of the PSF file on the filesystem
- StorageDirectory specifies the directory where the extracted files are stored
### options
- Verify to verify that the file data is intact
- SingleThread only uses single thread to expand files
- NoProgressDisplay the entire file expansion duration does not show progress
##### CabFile Options
Optional option. The usage method is to add `/CabFile:<file path>` to the command line. Using this option will implicitly specify the XML file as express.psf.cix.xml in the storage directory, if the CAB file name is path\Windows*.*-KB???????-*_*.cab then PSF The file name is path\Windows*.*-KB???????-*_*.psf, otherwise, change the file extension of the CAB file to "psf" and use it as the PSF file name; the storage directory is to remove the extension CAB file name. The PSF, XML filename and storage folder can be specified explicitly.
If this option is used, the folder where the extracted files are stored must be empty.   
For example: Windows10.0-KB0000000-x64_00000000.cab file and Windows10.0-KB0000000-x64_ffffffff.psf file exist under "D":. Run PSFExpand.exe /Expand /CabFile:D:\Windows10.0-KB0000000-x64_00000000.cab, the program will search for D:\Windows10.0-KB0000000-x64_*.psf files, and finally select D:\Windows10.0-KB0000000 -x64_ffffffff.psf. If there is no error, the CAB file will be automatically expanded to the D:\Windows10.0-KB0000000-amd64_00000000 directory. Select the XML file containing the file information as D:\Windows10.0-KB0000000-x64_00000000\express.psf.cix.xml and expand the PSF file to D:\Windows10.0-KB0000000-x64_00000000.
##### XmlFile Options
Required if /CabFile is not specified. The usage method is to add `/XmlFile:<file path>` to the command line. Can be case insensitive.
##### PsfFile Options
Required if /CabFile is not specified. The usage method is to add `/PsfFile:<file path>` to the command line. Can be case insensitive.
##### StorageDirectory Options
Required if /CabFile is not specified. To use it, add `/StorageDirectory:<folder path>` to the command line. Can be case insensitive.
##### Verify Options
Required if /CabFile is not specified. Appending the `/Verify` option to the program to perform a hash check before the data is written to the file. If the file data is corrupted, it will give a `data invalid` error message.
##### SingleThread Options
Required if /CabFile is not specified. Appending the `/SingleThread` option the program will only use single-threaded extension files. For those who do not want the default multi-threaded expansion to take up too many resources, it will reduce the speed. In fact, the speed of multi-threading is generally not much faster than that of single-threading, because the main limitation of the expansion speed of PSF files is the speed of reading and writing of the hard disk rather than the single-thread speed of the CPU.
##### NoProgressDisplay
Required if /CabFile is not specified. Appending `/NoProgressDisplay` option program will not show progress. The effect on the speed of expanding the file is not obvious.
### Remarks
The program will first display the relevant file information, and if there is an error, the error message will be displayed.  
If the /CabFile option is specified and the /XmlFile option is not specified, the program will not perform an initial check of the XML file before completing the CAB file expansion.
### Examples
- Expand the D:\Windows10.0-KB0000000-x64.psf file where the XML that saves the file information is D:\Windows10.0-KB0000000-x64\express.psf.cix.xml, and expand the file to D:\Windows10. 0-KB0000000-x64, check the file hash.
>PSFExpand.exe /Expand /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /Verify /PsfFile:D:\Windows10.0-KB0000000-x64.psf /StorageDirectory:D:\ Windows10.0-KB0000000-x64

- Expand D:\Windows10.0-KB0000000-x64_00000000.cab to D:\Windows10.0-KB0000000-x64_00000000, and search for a matching PSF file, expand to D:\Windows10.0-KB0000000-x64_00000000. Check file hash.
>PSFExpand.exe /Expand /CabFile:D:\Windows10.0-KB0000000-x64_00000000.cab /Verify
