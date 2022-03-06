# PSF 文件扩展实用工具
- 简体中文（当前）
- [English](README.md)

列出、提取、扩展 Windows PSF 更新。
### 使用
PSF 文件的使用离不开描述 PSF 包含文件信息的 XML，一般是对应 CAB 文件中的 express.psf.cix.xml。  
可手动展开 CAB 文件，然后使用此工具扩展 PSF 文件。
建议使用此工具展开 CAB 文件和 PSF 文件，因为运行 Expand.exe 在控制台输出了过多的文件名信息严重影响速度，此工具会避免此问题。
### PSFExpand 语言
应用提供简体中文和英语，正常情况会在运行时选择合适的语言。可使用`/Lang`选项显式指定语言。  
指定 /Lang 选项可选择显示语言。使用方法为在命令行中紧跟应用名加入'/Lang:<语言>'。

支持的语言：
- en         English
- zh-Hans    简体中文

示例：
- 使用英语作为显示语言并打印帮助
>PSFExpand.exe /Lang:en /?

若指定了不支持的语言，程序将使用英语作为显示语言。
### PSFExpand 提供操作命令
- [List](Documentation/List_zh-Hans.md)
- [Extract](Documentation/Extract_zh-Hans.md)
- [Expand](Documentation/Expand_zh-Hans.md)
### API
参阅 [PSF Extraction Handler API](Documentation/APIs_zh-Hans.md) 文档。
### 运行条件
操作系统必须是 Windows Vista 以上版本的 Windows。因为 PSF Extraction Handler API 的文件提取函数调用了 MSDelta 函数。
### 生成
两个项目的平台工具集为 v143，这意味着需要 Visual Studio 2022 或更新的版本。也可以手动修改平台工具集版本以支持早期版本的 Visual Studio。  
需要安装 Windows SDK，不一定必须使用最新的版本。
### 更新日志
参阅 [CHANGELOG_zh-Hans.md](CHANGELOG_zh-Hans.md)。
