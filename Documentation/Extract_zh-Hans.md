## PSFExpand.exe /Extract 操作命令
提取 PSF 中指定的文件。
### 选项
- XmlFile 指定 XML 文件在文件系统上的路径
- PsfFile 指定 PSF 文件在文件系统上的路径
- Filename 指定一个或多个欲提取的文件的文件名，通配符可提取匹配的多个文件名。
- OutDir 指定存储提取出文件的目录
- Verify 校验文件数据是否完好
- Verbose 啰嗦模式，每提取一个文件在控制台窗口打印出文件名
##### XmlFile 选项
必须选项。使用方法为在命令行加入`/XmlFile:<文件路径>`。可以不区分大小写。
##### PsfFile 选项
必须选项。使用方法为在命令行加入`/PsfFile:<文件路径>`。可以不区分大小写。
##### Filename 选项
必须选项。在命令行中直接加入文件名（不带斜杠“/”），可指定多个文件。程序会将所有匹配的文件全部提取。不区分大小写。
##### OutDir 选项
必须选项。使用方法为在命令行加入`/OutDir:<文件夹路径>`。可以不区分大小写。
##### Verify 选项
可选选项。加入`/Verify`选项程序在数据将要写入文件前进行哈希校验，若文件数据损坏会给出`数据无效`的错误消息。
##### Verbose 选项
可选选项。加入`/Verbose`选项程序会在成功写一个文件后输出文件名在控制台。
### 例如：
- 提取保存文件信息的 XML 为 D:\Windows10.0-KB0000000-x64\express.psf.cix.xml 的 D:\Windows10.0-KB0000000-x64.psf 中 historycix.cab 文件并校验，将文件输出到 D:\Windows10.0-KB0000000-x64。
>PSFExpand.exe /Extract /PsfFile:D:\Windows10.0-KB0000000-x64.psf historycix.cab /Verify /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml /OutDir:D:\Windows10.0-KB0000000-x64

- 提取保存文件信息的 XML 为 D:\Windows10.0-KB0000000-x64\express.psf.cix.xml 的 D:\Windows10.0-KB0000000-x64.psf 中的所有文件，存放到 D:\Windows10.0-KB0000000-x64，提取过程中显示每个提取的文件（提取所有文件不建议这么做，建议使用['Expand'操作命令](Expand_zh-Hans.md)，除非你希望显示每个提取的文件）。
>PSFExpand.exe /Extract /XmlFile:D:\Windows10.0-KB0000000-x64\express.psf.cix.xml * /PsfFile:D:\Windows10.0-KB0000000-x64.psf /OutDir:D:\Windows10.0-KB0000000-x64 /Verify
