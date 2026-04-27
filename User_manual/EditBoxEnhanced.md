# 输入框增强功能 (EditBox Enhanced)

CVUIL 为输入框控件提供了多项增强功能，包括密码框、数字输入限制、最大长度限制等。这些功能使得输入框更加灵活和实用。

>## 创建密码框

密码框是一种特殊的输入框，用户输入的字符会被隐藏（通常显示为星号或其他字符）。可以通过 `PasswordBox` 函数创建密码框，其参数如下：

```cpp
ComponentInfo PasswordBox(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 输入框默认显示的文本
    int id,             // 控件唯一编号
    int x,              // 输入框的横坐标
    int y,              // 输入框的纵坐标
    int width,          // 输入框的宽度
    int height,         // 输入框的高度
    TCHAR passwordChar  // 密码字符（如 '*'）
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：输入框默认显示的文本，需要使用宽字符字符串。
- `id`：输入框的唯一编号，用于区分不同的输入框并处理事件。
- `x` 和 `y`：输入框在主界面中的位置，以像素为单位。
- `width` 和 `height`：输入框的宽度和高度，以像素为单位。
- `passwordChar`：密码字符，用于隐藏用户输入的内容（如 `L'*'`）。

### 示例代码

```cpp
ComponentInfo passwordBox = PasswordBox(mainHwnd, L"", 10, 10, 10, 200, 30, L'*');
```

>## 设置输入框密码字符

可以通过 `SetEditBoxPasswordChar` 函数动态设置或修改输入框的密码字符，其参数如下：

```cpp
void SetEditBoxPasswordChar(HWND hwnd, int id, TCHAR ch);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：输入框的唯一编号。
- `ch`：密码字符，设置为 0 可以取消密码模式。

### 示例代码

```cpp
SetEditBoxPasswordChar(mainHwnd, 10, L'*'); // 设置为密码模式
SetEditBoxPasswordChar(mainHwnd, 10, 0);    // 取消密码模式
```

>## 设置输入框只允许数字输入

可以通过 `SetEditBoxNumberOnly` 函数限制输入框只允许输入数字，其参数如下：

```cpp
void SetEditBoxNumberOnly(HWND hwnd, int id, BOOL numberOnly);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：输入框的唯一编号。
- `numberOnly`：是否只允许数字输入，`TRUE` 表示只允许数字，`FALSE` 表示允许任意字符。

### 示例代码

```cpp
SetEditBoxNumberOnly(mainHwnd, 10, TRUE);  // 只允许数字输入
SetEditBoxNumberOnly(mainHwnd, 10, FALSE); // 允许任意字符输入
```

>## 设置输入框最大长度

可以通过 `SetEditBoxMaxLength` 函数限制输入框的最大输入长度，其参数如下：

```cpp
void SetEditBoxMaxLength(HWND hwnd, int id, UINT maxLength);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：输入框的唯一编号。
- `maxLength`：最大输入长度（字符数）。

### 示例代码

```cpp
SetEditBoxMaxLength(mainHwnd, 10, 20); // 限制最大输入20个字符
```

>## 设置输入框选中文本

可以通过 `SetEditBoxSelection` 函数设置输入框中选中某段文本，其参数如下：

```cpp
void SetEditBoxSelection(HWND hwnd, int id, DWORD start, DWORD end);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：输入框的唯一编号。
- `start`：选中文本的起始位置。
- `end`：选中文本的结束位置。

### 示例代码

```cpp
SetEditBoxSelection(mainHwnd, 10, 0, 5); // 选中前5个字符
```

>## 综合示例

以下代码展示了如何使用输入框增强功能：

```cpp
void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 创建普通输入框
    EditBox(hwnd, L"普通文本", 1, 10, 10, 200, 30);
    
    // 创建密码框
    PasswordBox(hwnd, L"", 2, 10, 50, 200, 30, L'*');
    
    // 创建数字输入框
    ComponentInfo numberBox = EditBox(hwnd, L"123", 3, 10, 90, 200, 30);
    SetEditBoxNumberOnly(hwnd, 3, TRUE);
    
    // 创建有限制长度的输入框
    ComponentInfo limitedBox = EditBox(hwnd, L"", 4, 10, 130, 200, 30);
    SetEditBoxMaxLength(hwnd, 4, 10); // 最多10个字符
}
```

>## 注意事项

- 密码框和普通输入框本质上是一样的，只是显示方式不同。
- `SetEditBoxNumberOnly` 限制的是用户输入，程序通过代码设置文本不受此限制。
- `SetEditBoxMaxLength` 限制的是字符数，不是字节数。对于Unicode字符，一个字符可能占用多个字节。
- 这些增强功能同样适用于 `MultiEditBox`（多行输入框）。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [单行输入框控件](EditBox.md)
- [多行输入框控件](MultiEditBox.md)

---
