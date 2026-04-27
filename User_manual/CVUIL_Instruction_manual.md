# CVUIL (ConVenient UI Library) 使用说明

CVUIL 是一个自定义的 WINAPI 编程库，旨在简化 WINAPI 的调用，使 Windows 程序开发更加简单易用。通过 CVUIL，开发者可以快速创建和管理各种控件，而无需深入复杂的 WINAPI 细节。

>## 使用方法

找到CVUIL文件夹中的`CVUIL.h`与`CVUIL.cpp`文件，并在你的C或C++项目中包含它们（将这两个添加到现有项目中），在项目中使用`#include`引入头文件：

```c
#include "CVUIL.h"
```

然后就可以使用 CVUIL 提供的各种功能来创建和管理你的Windows程序窗口和控件了。

由于CVUIL是WINAPI的封装并基于C++运行，因此，CVUIL的函数调用方式与WINAPI的函数调用方式大致兼容，并且，你完全可以在包含CVUIL的头文件后直接继续使用所有C与C++代码以及WINAPI的函数。

### 注意事项

- Windows程序编译需要编译器支持UTF-8编码，支持C++11及更高版本，并支持创建适用于Windows API及窗口程序的项目，请确保你的开发环境支持这些要求。
- Visual Studio原生支持Windows程序开发，优先建议使用Visual Studio IDE进行开发，并使用Visual Studio的调试器进行调试。
- CVUIL已封装头文件：`stdio.h`、`stdlib.h`、`iostream`、`tchar.h`、`windows.h`、`winbase.h`、`winuser.h`、`commtrl.h`、`wincodec.h`、`shlwapi.h`、`shlobj.h`、`gdiplus.h`、`uxtheme.h`。

---

>## 基础概念

### 控件（Component）

控件（Component）是用户界面（UI）中的交互元素统称，用于软件与用户进行视觉交互。

在 CVUIL 中，控件是通过函数调用来创建的。控件创建时，可能会需要指定控件的类型、位置、大小等属性。这些属性由`ComponentInfo`结构体描述。

### id（ID）

id（ID）是用户自定义的唯一编号符，用于在代码中引用控件，为int类型，但大小不可超过65535（0xFFFF或2^16-1）。

### 句柄（Handle）

句柄（Handle）是 Windows 系统中用于标识对象的唯一标识符。在 CVUIL 中，句柄（Handle）被用于标识控件、窗口、对话框等对象。

与id不同，句柄（Handle）是系统分配的唯一标识符，用于在消息处理函数（在CVUIL中为`InitLogic`以及`Logic`函数）中识别控件的响应。而id（ID）是用户自定义的唯一编号符，用于在代码中引用和区分控件。

### ComponentInfo 结构体

`ComponentInfo` 是用于声明控件类型的结构体，用于描述控件的属性，`ComponentInfo`包含以下参数：

- `hwnd`：控件自身句柄。
- `hMenu`：控件所属菜单句柄。如果控件是菜单项，则需要指定所属菜单的句柄。
- `id`：控件的唯一编号。
- `text`：控件名称。用于在用户界面中显示该控件的文本，针对按钮控件等有显示文本的控件。默认值为空字符串。

原型：
```cpp
typedef struct ComponentInfo
{
    HWND hwnd;
    HMENU hMenu;
    int id;
    LPCTSTR text;
}ComponentInfo, * PComponentInfo;
```

创建控件时，如果不需要获取该控件的信息，则不需要为该控件声明`ComponentInfo`结构体，可以直接创建控件，如：

```cpp
//使用ComponentInfo声明
ComponentInfo Button1 = Button(mainHwnd, L"Click Me", 1, 10, 10, 100, 50);

//不使用ComponentInfo声明
Button(mainHwnd, L"Click Me", 2, 10, 70, 100, 50);
```

两种创建方式均有效。

### DialogInfo 结构体

`DialogInfo` 是用于对话框的结构体，包含以下参数：

- `hwnd`：对话框窗口句柄。
- `id`：对话框的唯一编号。
- `title`：对话框标题。

原型：

```cpp
typedef struct DialogInfo
{
    HWND hwnd;
    int id;
    LPCTSTR title;
}DialogInfo, * PDialogInfo;
```

### TreeItemHandle 类型

`TreeItemHandle` 是树形控件节点的句柄类型，用于操作树形控件的节点。

### 异步任务回调函数类型

```cpp
typedef void (*AsyncTaskFunc)(void* param);
typedef void (*AsyncCallbackFunc)(void* result);
typedef void (*AsyncProgressFunc)(int progress);
typedef void (*AsyncTaskWithProgressFunc)(void* param, void (*reportProgress)(int));
```

接下来，你可以依次通过目录中的链接，了解如何创建各种控件以及如何管理它们的响应逻辑。

---

>## 目录

1. [编译指南](BuildGuide.md)
2. [主界面与消息处理](MainFunctions.md)
3. [基础控件创建](Button.md)
   - [按钮控件](Button.md)
   - [复选框控件](CheckBox.md)
   - [单选按钮控件](RadioButton.md)
   - [分组框控件](GroupBox.md)
   - [单行输入框控件](EditBox.md)
   - [多行输入框控件](MultiEditBox.md)
   - [静态文本框控件](TextBox.md)
   - [组合框控件](ComboBox.md)
   - [列表控件](List.md)
   - [表格控件](Table.md)
   - [滑块控件](Slider.md)
   - [菜单栏](MenuBar.md)
   - [工具提示控件](ToolTips.md)
4. [输入框增强功能](EditBoxEnhanced.md)
5. [文件对话框](FileDialogs.md)
6. [进度条控件](ProgressBar.md)
7. [树形控件](TreeView.md)
8. [对话框支持](Dialogs.md)
9. [异步任务支持](AsyncTasks.md)
10. [其他功能](OtherFunctions.md)

---

>## 功能概览

CVUIL 提供以下主要功能：

### 基础控件

- **按钮控件** - 可点击的按钮，触发事件
- **复选框控件** - 可选中/取消选中的复选框
- **单选按钮控件** - 互斥选择的单选按钮组
- **分组框控件** - 用于组织和布局其他控件
- **输入框控件** - 单行和多行文本输入
- **静态文本框控件** - 显示不可编辑的文本
- **组合框控件** - 下拉选择框
- **列表控件** - 列表框，支持选择
- **表格控件** - 多行多列的数据表格
- **滑块控件** - 滑动选择器
- **菜单栏控件** - 窗口顶部菜单
- **工具提示控件** - 鼠标悬停提示

### 增强功能

- **输入框增强** - 密码框、数字限制、最大长度限制等
- **文件对话框** - 打开文件、保存文件、浏览文件夹
- **进度条控件** - 显示操作进度
- **树形控件** - 显示层次结构数据
- **对话框支持** - 模态对话框和非模态对话框
- **异步任务支持** - 后台执行耗时操作，不阻塞UI

### 辅助功能

- **控件状态管理**（启用/禁用、显示/隐藏、移动）
- **数字转字符串**
- **内存管理**
- **......**（更多辅助功能移步至 [其他功能](OtherFunctions.md)）
---
