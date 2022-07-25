## PSFExpand.exe /Expand 操作命令
展开整个 PSF 文件。
### 文件信息选项
- CabFile 指定 CAB 文件在文件系统上的路径
- XmlFile 指定 XML 文件在文件系统上的路径
- PsfFile 指定 PSF 文件在文件系统上的路径
- OutDir 指定存储提取出文件的目录
### 选项
- Verify 校验文件数据是否完好
- SingleThread 仅使用单线程扩展文件
- NoProgressDisplay 整个文件扩展期间不显示进度
##### CabFile 选项
可选选项。使用方法为在命令行加入`/CabFile:<文件路径>`。可以不区分大小写，因此不支持路径长度过长的文件。使用此选项将隐式指定 XML 文件为存储目录下的 express.psf.cix.xml，若 CAB 文件名为 路径\Windows*.*-KB???????-*_*.cab 则 PSF 文件名为路径\Windows*.*-KB???????-*_*.psf，否则将 CAB 文件的文件扩展名改为“psf”并作为 PSF 文件名；存储目录为去除扩展名的 CAB 文件名。可显式指定 PSF、XML 文件名和存储文件夹。  
如果使用此选项，存储提取文件的文件夹必须为空。  
例如：在"D:"下存在 Windows10.0-KB0000000-x64_00000000.cab 文件和 Windows10.0-KB0000000-x64_ffffffff.psf 文件。运行 PSFExpand.exe /Expand /CabFile:D:\Windows10.0-KB0000000-x64_00000000.cab，程序会搜索 D:\Windows10.0-KB0000000-x64_*.psf 文件，最终选择 D:\Windows10.0-KB0000000-x64_ffffffff.psf.如果无误就自动将 CAB 文件展开到 D:\Windows10.0-KB0000000-amd64_00000000 目录下。选择包含文件信息的 XML 文件为 D:\Windows10.0-KB0000000-x64_00000000\express.psf.cix.xml 然后展开 PSF 文件到 D:\Windows10.0-KB0000000-x64_00000000。
##### XmlFile 选项
若未指定 /CabFile 则为必须选项。使用方法为在命令行加入`/XmlFile:<文件路径>`。可以不区分大小写。
##### PsfFile 选项
若未指定 /CabFile 则为必须选项。使用方法为在命令行加入`/PsfFile:<文件路径>`。可以不区分大小写。
##### OutDir 选项
若未指定 /CabFile 则为必须选项。使用方法为在命令行加入`/OutDir:<文件夹路径>`。可以不区分大小写。
##### Verify 选项
可选选项。加入`/Verify`选项程序在数据将要写入文件前进行哈希校验，若文件数据损坏会给出`数据无效`的错误消息。
##### SingleThread 选项
可选选项。加入`/SingleThread`选项程序将仅使用单线程扩展文件。对于不想默认多线程扩展占用太多资源的可以使用，会降低速度。其实多线程提高的速度一般并不比单线程快太多，因为制约 PSF 文件扩展速度的主要是硬盘读写速度而不是 CPU 的单线程速度。
##### NoProgressDisplay 选项
可选选项。加入`/NoProgressDisplay`选项程序将不显示进度。
### 备注
程序会首先显示相关的文件信息，若出错则显示错误信息。  
若指定的 /CabFile 选项又不指定 /XmlFile 选项，程序在完成 CAB 文件扩展之前不会进行初次检查 XML 文件。
### 例如
- 展开保存文件信息的 XML 为 D:\Windows10.0-KB0000000-x64\express.psf.cix.xml 的 D:\Windows10.0-KB0000000-x64.psf 文件，将文件展开到 D:\Windows10.0-KB0000000-x64，校验文件哈希。
>PSFExpand.exe /Expand /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /Verify /PsfFile:D:\Windows10.0-KB0000000-x64.psf /OutDir:D:\Windows10.0-KB0000000-x64

- 展开 D:\Windows10.0-KB0000000-x64_00000000.cab 到 D:\Windows10.0-KB0000000-x64_00000000，并搜索匹配的 PSF 文件，展开到 D:\Windows10.0-KB0000000-x64_00000000。校验文件哈希。
>PSFExpand.exe /Expand /CabFile:D:\Windows10.0-KB0000000-x64_00000000.cab /Verify
