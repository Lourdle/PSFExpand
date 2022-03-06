## PSFExpand.exe /List 操作命令
通过 PSF 文件指定的 XML 文件列出其包含的文件信息。
### 选项
- XmlFile 指定 XML 文件在文件系统上的路径
- DisplayDetail 显示每个文件的详情，包括大小（单位 字节）和文件在 PSF 中的类型（PA30、PA19、原始文件）
- Screen {条件} 筛选出符合指定标准的文件，可以使用完整文件名或带有文件通配符 (\*, ?) 的文件名
##### XmlFile 选项
必须选项。使用方法为在命令行加入`/XmlFile:<文件路径>`。可以不区分大小写。
##### DisplayDetail 选项
可选选项。在命令行中加入`/DisplayDetail`即可启用。
##### Screen 选项
可选选项。加入`/Screen`在后面参数加入你要列出文件的标准，完成后会显示符合的文件数。如果使用此选项，必须将其置于所有选项之后，因为在 /Screen 之后的所有命令行参数都会成为成为过滤器。过滤器是包含需要列出文件条件的字符串，不要在字符串开头添加斜杠，不区分大小写，在路径中的斜杠符用正斜杠`/`或反斜杠`\\`都是合法的。如“/Screen \*.dll”表示所有 dll 为扩展名的文件；“/Screen historycix.cab”则专指 historycix.cab 文件;若在“/Screen”后添加其它选项如“/Screen \*.dll /DisplayDetail”则表示文件全名为 /DisplayDetail 且扩展名为 dll 的文件，显然是不存在的。
### 例如：
- 显示 PSF 中所有文件的详细信息，其 XML 位于 D:\express.psf.cix.xml
>PSFExpand.exe /List /DisplayDetail /XmlFile:D:\express.psf.cix.xml

- 列出 PSF 中所有 DLL 文件，其 XML 位于 D:\express.psf.cix.xml
>PSFExpand.exe /List /XmlFile:D:\express.psf.cix.xml /Screen \*.dll
