# 编译指南

本文档详细介绍如何编译使用 CVUIL 框架的 Windows 程序。

---

## 编译命令详解

### GCC (MinGW/TDM-GCC) 编译命令

```bash
g++.exe -o 输出文件名.exe 主程序.cpp CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

#### 参数说明

| 参数 | 说明 |
|------|------|
| `-o 文件名.exe` | 指定输出文件名 |
| `-mwindows` | 创建Windows GUI程序（使用WinMain入口，而非main） |
| `-municode` | 启用Unicode支持（定义UNICODE和_UNICODE宏） |
| `-std=c++11` | 使用C++11标准 |
| `-O2` | 启用优化（可选） |
| `-lgdiplus` | 链接GDI+库（图像处理，可选） |
| `-lshlwapi` | 链接Shell API库（路径操作等） |
| `-lshlobj` | 链接Shell Object库（文件夹浏览等） |
| `-lcomctl32` | 链接通用控件库 |
| `-luxtheme` | 链接Windows主题库 |
| `-lwinmm` | 链接Windows多媒体库 |
| `-luser32` | 链接User32库（核心Windows API） |
| `-lgdi32` | 链接GDI32库（图形设备接口） |
| `-lshell32` | 链接Shell32库（用于文件夹浏览） |
| `-lcomdlg32` | 链接ComDlg32库（用于文件对话框） |
| `-lole32` | 链接OLE32库 |

### Visual Studio (cl.exe) 编译命令

```cmd
cl.exe /Fe:输出文件名.exe 主程序.cpp CVUIL.cpp /link user32.lib gdi32.lib comctl32.lib gdiplus.lib shlwapi.lib shlobj.lib uxtheme.lib winmm.lib shell32.lib comdlg32.lib ole32.lib /SUBSYSTEM:WINDOWS /UNICODE
```

#### 参数说明

| 参数 | 说明 |
|------|------|
| `/Fe:文件名.exe` | 指定输出文件名 |
| `/link` | 开始链接器选项 |
| `/SUBSYSTEM:WINDOWS` | 创建Windows GUI程序 |
| `/UNICODE` | 启用Unicode支持 |

---

## 支持的编译器

### 1. TDM-GCC-64 (推荐)

- **下载**: https://jmeubank.github.io/tdm-gcc/
- **特点**: 专为Windows优化，包含所有必要库
- **安装**: 下载安装包，按向导安装，勾选添加到PATH

### 2. MinGW-w64

- **下载**: https://www.mingw-w64.org/downloads/
- **特点**: 标准MinGW，支持64位
- **安装**: 下载并解压到C:\MinGW，添加到PATH

### 3. MSYS2

- **下载**: https://www.msys2.org/
- **特点**: 完整的Linux-like环境
- **安装**:
  
  ```bash
  pacman -S mingw-w64-x86_64-gcc
  pacman -S mingw-w64-x86_64-gdb
  ```

### 4. Visual Studio Community

- **下载**: https://visualstudio.microsoft.com/
- **特点**: 微软官方编译器，IDE功能强大
- **安装**: 安装时勾选"使用C++的桌面开发"

---

## 快速开始

### 手动编译

**使用 GCC**

```bash
g++.exe -o MyApp.exe MyApp.cpp CVUIL\CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

**使用 Visual Studio**
打开 "Developer Command Prompt for VS"，然后：

```cmd
cl.exe /Fe:MyApp.exe MyApp.cpp CVUIL\CVUIL.cpp /link user32.lib gdi32.lib comctl32.lib gdiplus.lib shlwapi.lib shlobj.lib uxtheme.lib winmm.lib shell32.lib comdlg32.lib ole32.lib /SUBSYSTEM:WINDOWS
```

---

## 不同编译器的使用方法

### TDM-GCC-64

**安装路径**: `C:\TDM-GCC-64\bin\g++.exe`

**编译命令**:

```bash
C:\TDM-GCC-64\bin\g++.exe -o App.exe App.cpp CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

**添加到PATH**:

1. 右键"此电脑" → 属性 → 高级系统设置
2. 环境变量 → 系统变量 → Path
3. 添加 `C:\TDM-GCC-64\bin`

### MinGW-w64

**安装路径**: `C:\mingw64\bin\g++.exe`

**编译命令**:

```bash
C:\mingw64\bin\g++.exe -o App.exe App.cpp CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

### MSYS2

**安装路径**: `C:\msys64\mingw64\bin\g++.exe`

**编译命令**:

```bash
C:\msys64\mingw64\bin\g++.exe -o App.exe App.cpp CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

### Visual Studio

**使用Developer Command Prompt**:

1. 开始菜单 → Visual Studio 2022 → Developer Command Prompt
2. 切换到项目目录
3. 运行编译命令

**编译命令**:

```cmd
cl.exe /Fe:App.exe App.cpp CVUIL.cpp /link user32.lib gdi32.lib comctl32.lib gdiplus.lib shlwapi.lib shlobj.lib uxtheme.lib winmm.lib shell32.lib comdlg32.lib ole32.lib /SUBSYSTEM:WINDOWS
```

---

## 常见问题

### Q1: 提示 "g++ 不是内部或外部命令"

**解决**: 编译器未添加到PATH，使用完整路径或添加到环境变量。

### Q2: 提示 "undefined reference to `WinMain'"

**解决**: 忘记添加 `-mwindows` 参数。

### Q3: 提示 "无法找到 -lgdiplus"

**解决**: 编译器不完整，建议安装TDM-GCC-64。

### Q4: 中文显示乱码

**解决**: 确保添加 `-municode` 参数，且源文件保存为UTF-8编码。

### Q5: 程序运行时缺少DLL

**解决**: 

- 静态链接：添加 `-static` 参数
- 或复制所需DLL到程序目录

### Q6: Visual Studio 提示 "无法打开包括文件: 'windows.h'"

**解决**: 安装时未选择Windows SDK，重新运行安装程序添加。

### Q7: Alpha3/Alpha4 文件对话框无法打开

**解决**: 确保添加了 `-lshell32 -lcomdlg32 -lole32` 链接库。

---

## 项目结构示例

```
MyProject/
├── MyApp.cpp          # 你的主程序
├── CVUIL/             # CVUIL库目录
│   ├── CVUIL.h        # CVUIL头文件
│   └── CVUIL.cpp      # CVUIL实现文件
└── README.md          # 项目说明
```

---

## 最小CVUIL程序示例

```cpp
#include "CVUIL\CVUIL.h"

// 创建主界面
void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"Hello CVUIL";
    *width = 400;
    *height = 300;
}

// 初始化
void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Button(hwnd, L"点击我", 1, 150, 100, 100, 30);
}

// 事件处理
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        MessageBox(hwnd, L"Hello, CVUIL!", L"提示", MB_OK);
    }
}
```

编译：

```bash
g++.exe -o Hello.exe Hello.cpp CVUIL\CVUIL.cpp -mwindows -lgdiplus -lshlwapi -lshlobj -lcomctl32 -luxtheme -lwinmm -luser32 -lgdi32 -lshell32 -lcomdlg32 -lole32 -municode -std=c++11
```

---

### 相关链接

- [主文档](CVUIL_Instruction_manual.md) - 返回主文档首页
- [主要函数](MainFunctions.md) - 了解 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法

---
