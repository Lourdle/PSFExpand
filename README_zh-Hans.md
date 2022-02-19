# PSF 文件扩展实用工具
- 简体中文（当前）
- [English](README.md)

列出、提取、扩展 Windows PSF 更新。
### 使用
PSF 文件的使用离不开描述 PSF 包含文件信息的 XML，一般是对应 CAB 文件中的 express.psf.cix.xml。可以使用 Expand.exe 先展开 PSF 对应的 CAB 文件。然后使用 PSFExpand.exe 展开 PSF。
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
