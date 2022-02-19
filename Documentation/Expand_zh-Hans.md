## PSFExpand.exe /Expand 操作命令
展开整个 PSF 文件。
### 选项
- XmlFile 指定 XML 文件在文件系统上的路径
- PsfFile 指定 PSF 文件在文件系统上的路径
- StorageDirectory 指定存储提取出文件的目录
- Verify 校验文件数据是否完好
- SingleThread 仅使用单线程扩展文件
- NoProgressDisplay 整个文件扩展不显示进度
##### XmlFile 选项
必须选项。使用方法为在命令行加入`/XmlFile:<文件路径>`。可以不区分大小写，不支持使用 UNC 路径规范，因此不支持路径长度过长的文件。
##### PsfFile 选项
必须选项。使用方法为在命令行加入`/PsfFile:<文件路径>`。可以不区分大小写。
##### StorageDirectory 选项
必须选项。使用方法为在命令行加入`/StorageDirectory:<文件夹路径>`。可以不区分大小写，因此不支持路径长度过长的文件。
##### Verify 选项
可选选项。加入`/Verify`选项程序在数据将要写入文件前进行哈希校验，若文件数据损坏会给出'数据无效'的错误消息。
##### SingleThread 选项
可选选项。加入`/SingleThread`选项程序将仅使用单线程扩展文件。对于不想默认多线程扩展占用太多资源的可以使用，会降低速度。其实多线程提高的速度一般并不比单线程快太多，因为制约 PSF 文件扩展速度的主要是硬盘读写速度而不是 CPU 的单线程速度。
##### NoProgressDisplay
可选选项。加入`/NoProgressDisplay`选项程序将不显示进度。对展开文件速度的影响效果不明显。
### 例如
- 展开保存文件信息的 XML 为 D:\Windows10.0-KB0000000-x64\express.psf.cix.xml 的 D:\Windows10.0-KB0000000-x64.psf 文件，校验文件哈希。
>PSFExpand.exe /Expand /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /PsfFile:D:\Windows10.0-KB0000000-x64.psf /StorageDirectory:D:\Windows10.0-KB0000000-x64 /Verify
