## PSFExpand.exe /List 操作命令
通过 PSF 文件指定的 XML 文件列出其包含的文件信息。
### 选项
- XmlFile 指定 XML 文件在文件系统上的路径
- DisplayDetail 显示每个文件的详情，包括大小（单位 字节）和文件在 PSF 中的类型（PA30、PA19、原始文件）
- Filename 指定一个或多个欲列出的文件的文件名，通配符可列出匹配的多个文件名。
##### XmlFile 选项
必须选项。使用方法为在命令行加入`/XmlFile:<文件路径>`。可以不区分大小写。
##### DisplayDetail 选项
可选选项。在命令行中加入`/DisplayDetail`即可启用。
##### Filename 选项
可选选项。在命令行中直接加入文件名（不带斜杠“/”），可指定多个文件。程序会将所有匹配的文件全部列出。不区分大小写。
### 例如：
- 显示 PSF 中所有文件的详细信息，其 XML 位于 D:\express.psf.cix.xml
>PSFExpand.exe /List /DisplayDetail /XmlFile:D:\express.psf.cix.xml

- 列出 PSF 中所有 DLL 文件，其 XML 位于 D:\express.psf.cix.xml。
>PSFExpand.exe /List /XmlFile:D:\express.psf.cix.xml \*.dll
