# 其他功能 (Other Functions)

CVUIL 库除了提供丰富的控件创建和操作功能外，还包含了一些辅助功能，用于方便开发者进行其他操作。

>## 1. 数字转字符串 (NumToStr)

CVUIL 提供了一个 `NumToStr` 多态函数，用于将各种类型的数字转换为字符串。这些函数支持整型、浮点型、长整型等多种数据类型。

### 函数原型

```cpp
LPCTSTR NumToStr(short number);
LPCTSTR NumToStr(unsigned short number);
LPCTSTR NumToStr(int number);
LPCTSTR NumToStr(unsigned int number);
LPCTSTR NumToStr(long number);
LPCTSTR NumToStr(unsigned long number);
LPCTSTR NumToStr(long long number);
LPCTSTR NumToStr(unsigned long long number);
LPCTSTR NumToStr(float number);
LPCTSTR NumToStr(double number);
LPCTSTR NumToStr(long double number);
```

### 示例代码

```cpp
int mainNumber = 42;
LPCTSTR str = NumToStr(mainNumber); // 转换为字符串
MessageBox(NULL, str, L"Number to String", MB_OK); // 显示消息框
```

### 注意事项

- 转换后的字符串存储在静态缓冲区中，每次调用 `NumToStr` 会覆盖之前的值。
- 适用于临时转换数字为字符串的场景，不建议用于长期存储。

---

>## 2. 获取控件句柄 (GetComponentHWND)

可以通过 `GetComponentHWND` 函数获取指定控件的句柄。这在需要直接操作控件时非常有用。

### 函数原型

```cpp
HWND GetComponentHWND(HWND hwnd, int id);
```

- `hwnd`：主界面的句柄。
- `id`：控件的唯一编号。

### 示例代码

```cpp
HWND buttonHwnd = GetComponentHWND(mainHwnd, 1); // 获取编号为 1 的按钮的句柄
EnableWindow(buttonHwnd, FALSE); // 禁用按钮
```

### 注意事项

- 确保控件编号 `id` 是有效的，否则返回的句柄可能无效。

---

>## 3. 获取控件 ID (GetComponentId)

可以通过 `GetComponentId` 函数获取指定控件的 ID。

### 函数原型

```cpp
int GetComponentId(HWND hwndComponent);
```

- `hwndComponent`：控件的句柄。

### 示例代码

```cpp
int buttonId = GetComponentId(buttonHwnd); // 获取按钮的 ID
```

### 注意事项

- 确保提供的控件句柄是有效的。

---

>## 4. 修改控件文本 (SetComponentText)

可以通过 `SetComponentText` 函数动态修改控件的文本内容。这适用于按钮、静态文本框、输入框等控件。

### 函数原型

```cpp
void SetComponentText(HWND hwnd, int id, LPCTSTR text);
```

- `hwnd`：主界面的句柄。
- `id`：控件的唯一编号。
- `text`：新的文本内容，需要使用宽字符字符串。

### 示例代码

```cpp
SetComponentText(mainHwnd, 1, L"New Button Text"); // 修改编号为 1 的按钮的文本
```

### 注意事项

- 修改文本时，确保 `text` 是宽字符字符串。
- 如果控件编号 `id` 无效，修改操作将不会生效。

---

>## 5. 获取控件文本 (GetComponentText)

可以通过 `GetComponentText` 函数获取指定控件的文本内容。

### 函数原型

```cpp
LPCTSTR GetComponentText(HWND hwnd, int id);
```

- `hwnd`：主界面的句柄。
- `id`：控件的唯一编号。

### 示例代码

```cpp
LPCTSTR buttonText = GetComponentText(mainHwnd, 1); // 获取编号为 1 的按钮的文本
MessageBox(NULL, buttonText, L"Button Text", MB_OK); // 显示消息框
FreeString(buttonText); // 释放内存
```

### 注意事项

- 确保控件编号 `id` 是有效的，否则返回的文本可能为空。
- 返回的字符串需要使用 `FreeString` 函数释放内存。

---

>## 6. 释放字符串内存 (FreeString)

CVUIL 中许多函数返回动态分配的字符串，使用完毕后需要调用 `FreeString` 函数释放内存。

### 函数原型

```cpp
void FreeString(LPCTSTR str);
```

- `str`：要释放的字符串指针。

### 示例代码

```cpp
LPCTSTR text = GetEditBoxText(hwnd, 1);
if (text != NULL)
{
    // 使用 text
    MessageBox(hwnd, text, L"内容", MB_OK);
    
    // 释放内存
    FreeString(text);
}
```

### 注意事项

- 只能释放 CVUIL 函数返回的字符串，不要释放其他来源的字符串。
- 释放 `NULL` 是安全的，不会导致错误。

---

>## 7. 控件状态管理

### 启用/禁用控件 (EnableComponent)

```cpp
void EnableComponent(HWND hwnd, int id, BOOL enable);
```

- `enable` 为 `TRUE` 时启用控件，为 `FALSE` 时禁用控件。

### 显示/隐藏控件 (ShowComponent)

```cpp
void ShowComponent(HWND hwnd, int id, BOOL show);
```

- `show` 为 `TRUE` 时显示控件，为 `FALSE` 时隐藏控件。

### 移动控件 (MoveComponent)

```cpp
void MoveComponent(HWND hwnd, int id, int x, int y, int width, int height);
```

- 移动控件到指定位置并设置大小。

### 设置控件焦点 (SetComponentFocus)

```cpp
void SetComponentFocus(HWND hwnd, int id);
```

- 将焦点设置到指定控件。

### 示例代码

```cpp
EnableComponent(hwnd, 1, FALSE);    // 禁用按钮
ShowComponent(hwnd, 2, FALSE);      // 隐藏文本框
MoveComponent(hwnd, 3, 100, 100, 200, 30); // 移动输入框
SetComponentFocus(hwnd, 4);         // 设置焦点到输入框
```

---

>## 8. 关闭程序 (PostQuitMessage)

WINAPI内部函数，用于关闭该Windows程序。

### 示例代码

```cpp
PostQuitMessage(0);
```

---

>## 9. 弹窗 (MessageBox)

WINAPI内部函数，用于显示一个消息框。

### 函数原型

```cpp
int MessageBox(HWND hwnd, LPCTSTR text, LPCTSTR caption, UINT type);
```

- `hwnd`：主界面的句柄。
- `text`：弹窗内的文本内容，需要使用宽字符字符串。
- `caption`：弹窗标题，需要使用宽字符字符串。
- `type`：弹窗的按钮组合类型，可以是以下值之一：
  - `MB_OK`：显示一个包含了一个确定按钮的消息框。
  - `MB_OKCANCEL`：显示一个包含了确定和取消按钮的消息框。
  - `MB_YESNO`：显示一个包含了是和否按钮的消息框。
  - `MB_YESNOCANCEL`：显示一个包含了是、否和取消按钮的消息框。

### 示例代码

```c++
int result = MessageBox(mainHwnd, L"确定要退出吗？", L"提示", MB_YESNO);
if (result == IDYES) {
    // 用户点击了是按钮
    PostQuitMessage(0);
} 
else {
    // 用户点击了否按钮或取消按钮
    return;
}
```

### 注意事项

- 弹窗函数会阻塞程序执行，直到用户点击按钮后才会返回。
- 根据 `type` 参数的不同，返回值可能不同，例如 `MB_OK` 返回 `IDOK`，`MB_YESNO` 返回 `IDYES` 或 `IDNO`，可自行查阅WINAPI相关文档。

---

>## 10. 其他

由于CVUIL是WINAPI的封装并基于C++运行，因此，CVUIL的函数调用方式与WINAPI的函数调用方式大致兼容，并且，你完全可以在包含CVUIL的头文件后直接继续使用所有C++代码以及WINAPI的函数。

---

### 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法

#### 控件文档
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
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

#### 功能文档
- [输入框增强功能](EditBoxEnhanced.md)
- [文件对话框](FileDialogs.md)
- [对话框支持](Dialogs.md)
- [异步任务支持](AsyncTasks.md)

---
