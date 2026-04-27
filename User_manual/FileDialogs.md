# 文件对话框 (File Dialogs)

CVUIL 提供文件对话框支持，包括打开文件对话框、保存文件对话框和浏览文件夹对话框。这些功能使得用户可以方便地选择文件和文件夹。

>## 打开文件对话框

可以通过 `OpenFileDialog` 函数打开一个文件选择对话框，让用户选择文件。支持单文件选择和多文件选择（按住 Ctrl 或 Shift 键可选择多个文件）。其参数如下：

```cpp
LPCTSTR OpenFileDialog(HWND hwnd, LPCTSTR filter);
```

### 参数说明

- `hwnd`：父窗口句柄。
- `filter`：文件过滤器，用于限制显示的文件类型。格式为 `"描述|扩展名|描述|扩展名|..."`，如 `"文本文件|*.txt|所有文件|*.*"`。

### 返回值

- **单文件选择**：返回单个文件的完整路径，如 `C:\Folder\file.txt`
- **多文件选择**：返回多个文件的完整路径，用分号 `;` 分隔，如 `C:\Folder\file1.txt;C:\Folder\file2.txt;C:\Folder\file3.txt`
- **取消选择**：返回 `NULL`

### 示例代码

#### 单文件选择

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1) // 按钮点击
    {
        LPCTSTR filePath = OpenFileDialog(hwnd, L"文本文件|*.txt|所有文件|*.*");
        if (filePath != NULL)
        {
            MessageBox(hwnd, filePath, L"选择的文件", MB_OK);
            FreeString(filePath); // 使用完毕后释放内存
        }
    }
}
```

#### 多文件选择处理

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1) // 按钮点击
    {
        LPCTSTR filePaths = OpenFileDialog(hwnd, L"图片文件|*.jpg;*.png|所有文件|*.*");
        if (filePaths != NULL)
        {
            // 检查是否包含分号（多文件）
            if (wcschr(filePaths, L';') != NULL)
            {
                // 多文件情况
                wchar_t buffer[4096];
                wcscpy(buffer, filePaths);
                
                wchar_t* token = wcstok(buffer, L";");
                while (token != NULL)
                {
                    // 处理每个文件路径
                    MessageBox(hwnd, token, L"选择的文件", MB_OK);
                    token = wcstok(NULL, L";");
                }
            }
            else
            {
                // 单文件情况
                MessageBox(hwnd, filePaths, L"选择的文件", MB_OK);
            }
            
            FreeString(filePaths); // 释放内存
        }
    }
}
```

>## 保存文件对话框

可以通过 `SaveFileDialog` 函数打开一个文件保存对话框，让用户选择保存位置。其参数如下：

```cpp
LPCTSTR SaveFileDialog(HWND hwnd, LPCTSTR filter, LPCTSTR defaultFileName);
```

### 参数说明

- `hwnd`：父窗口句柄。
- `filter`：文件过滤器，格式与 `OpenFileDialog` 相同。
- `defaultFileName`：默认文件名，可以为 `NULL`。

### 返回值

返回用户选择的文件路径。如果用户取消选择或发生错误，返回 `NULL`。

### 示例代码

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 2) // 按钮点击
    {
        LPCTSTR filePath = SaveFileDialog(hwnd, L"文本文件|*.txt", L"新建文件.txt");
        if (filePath != NULL)
        {
            MessageBox(hwnd, filePath, L"保存到", MB_OK);
            FreeString(filePath); // 使用完毕后释放内存
        }
    }
}
```

>## 浏览文件夹对话框

可以通过 `BrowseFolderDialog` 函数打开一个文件夹浏览对话框，让用户选择一个文件夹。其参数如下：

```cpp
LPCTSTR BrowseFolderDialog(HWND hwnd, LPCTSTR title);
```

### 参数说明

- `hwnd`：父窗口句柄。
- `title`：对话框标题。

### 返回值

返回用户选择的文件夹路径。如果用户取消选择或发生错误，返回 `NULL`。

### 示例代码

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 3) // 按钮点击
    {
        LPCTSTR folderPath = BrowseFolderDialog(hwnd, L"请选择一个文件夹");
        if (folderPath != NULL)
        {
            MessageBox(hwnd, folderPath, L"选择的文件夹", MB_OK);
            FreeString(folderPath); // 使用完毕后释放内存
        }
    }
}
```

>## 文件过滤器格式

文件过滤器的格式为 `"描述|扩展名|描述|扩展名|..."`，多个过滤器用 `|` 分隔。例如：

```cpp
|               过滤器字符串                |          说明          |
|------------------------------------------|------------------------|
| L"文本文件|*.txt"                         | 只显示txt文件 |
| L"所有文件|*.*"                           | 显示所有文件 |
| L"图片文件|*.jpg;*.png;*.bmp|所有文件|*.*" | 显示jpg、png、bmp文件或所有文件 |
| L"C++源文件|*.cpp|头文件|*.h|所有文件|*.*" | 显示cpp、h文件或所有文件 |
```

>## 内存管理注意事项

文件对话框返回的字符串是通过动态内存分配创建的，使用完毕后需要调用 `FreeString` 函数释放内存，以避免内存泄漏。

```cpp
LPCTSTR filePath = OpenFileDialog(hwnd, L"文本文件|*.txt");
if (filePath != NULL)
{
    // 使用 filePath
    // ...
    
    FreeString(filePath); // 释放内存
}
```

>## 完整示例

以下代码展示了如何创建一个包含文件操作功能的界面：

```cpp
#include "CVUIL.h"

void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"文件对话框示例";
    *width = 400;
    *height = 200;
}

void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Button(hwnd, L"打开文件", 1, 10, 10, 100, 30);
    Button(hwnd, L"保存文件", 2, 120, 10, 100, 30);
    Button(hwnd, L"选择文件夹", 3, 230, 10, 120, 30);
    
    TextBox(hwnd, L"未选择文件", 4, 10, 50, 340, 30);
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    LPCTSTR path = NULL;
    
    switch (id)
    {
        case 1: // 打开文件
            path = OpenFileDialog(hwnd, L"文本文件|*.txt|所有文件|*.*");
            break;
        case 2: // 保存文件
            path = SaveFileDialog(hwnd, L"文本文件|*.txt", L"新建文件.txt");
            break;
        case 3: // 选择文件夹
            path = BrowseFolderDialog(hwnd, L"请选择一个文件夹");
            break;
    }
    
    if (path != NULL)
    {
        SetComponentText(hwnd, 4, path);
        FreeString(path);
    }
}
```

>## 注意事项

- 文件对话框返回的字符串需要使用 `FreeString` 函数释放内存。
- 如果用户点击取消按钮，函数会返回 `NULL`。
- 文件对话框是模态对话框，会阻塞用户与父窗口的交互，直到对话框关闭。
- 编译时需要链接 `shell32.lib` 和 `comdlg32.lib` 库。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

---
