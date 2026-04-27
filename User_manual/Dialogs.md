# 对话框支持 (Dialogs)

CVUIL 支持对话框功能，包括模态对话框和非模态对话框。对话框可以用于显示附加信息、获取用户输入或执行特定任务。

>## 显示模态对话框

模态对话框会阻塞父窗口的交互，直到对话框关闭。可以通过 `ShowModalDialog` 函数显示模态对话框，其参数如下：

```cpp
void ShowModalDialog(
    HWND parentHwnd,    // 父窗口句柄
    LPCTSTR title,      // 对话框标题
    int width,          // 对话框宽度
    int height,         // 对话框高度
    void (*dialogInitFunc)(HWND),                           // 对话框初始化函数
    void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int) // 对话框逻辑函数
);
```

### 参数说明

- `parentHwnd`：父窗口句柄。对话框关闭前，父窗口将被禁用。
- `title`：对话框标题。
- `width` 和 `height`：对话框的宽度和高度。
- `dialogInitFunc`：对话框初始化函数，在对话框创建时调用，用于创建对话框内的控件。
- `dialogLogicFunc`：对话框逻辑函数，用于处理对话框内控件的事件。

### 示例代码

```cpp
// 对话框初始化函数
void DialogInit(HWND hwnd)
{
    TextBox(hwnd, L"请输入用户名：", 1, 10, 10, 200, 20);
    EditBox(hwnd, L"", 2, 10, 35, 200, 25);
    Button(hwnd, L"确定", 3, 60, 70, 80, 30);
}

// 对话框逻辑函数
void DialogLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 3) // 确定按钮
    {
        LPCTSTR username = GetEditBoxText(hwnd, 2);
        if (username != NULL && wcslen(username) > 0)
        {
            MessageBox(hwnd, username, L"欢迎", MB_OK);
            FreeString(username);
        }
    }
}

// 在主窗口中显示对话框
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1) // 按钮点击
    {
        ShowModalDialog(hwnd, L"登录", 250, 150, DialogInit, DialogLogic);
    }
}
```

>## 显示非模态对话框

非模态对话框不会阻塞父窗口的交互，用户可以同时操作父窗口和对话框。可以通过 `ShowModelessDialog` 函数显示非模态对话框，其参数如下：

```cpp
HWND ShowModelessDialog(
    HWND parentHwnd,    // 父窗口句柄
    LPCTSTR title,      // 对话框标题
    int width,          // 对话框宽度
    int height,         // 对话框高度
    void (*dialogInitFunc)(HWND),                           // 对话框初始化函数
    void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int) // 对话框逻辑函数
);
```

### 参数说明

与 `ShowModalDialog` 相同。

### 返回值

返回对话框的窗口句柄。如果失败，返回 `NULL`。

### 示例代码

```cpp
HWND dialogHwnd = NULL;

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1) // 显示对话框
    {
        if (dialogHwnd == NULL || !IsWindow(dialogHwnd))
        {
            dialogHwnd = ShowModelessDialog(hwnd, L"工具窗口", 300, 200, ToolDialogInit, ToolDialogLogic);
        }
    }
}
```

>## 关闭对话框

可以通过 `CloseDialog` 函数关闭指定的对话框，其参数如下：

```cpp
void CloseDialog(HWND dialogHwnd);
```

### 参数说明

- `dialogHwnd`：对话框的窗口句柄。

### 示例代码

```cpp
void DialogLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == IDCANCEL) // 取消按钮
    {
        CloseDialog(hwnd);
    }
}
```

>## 对话框回调函数

### 对话框初始化函数

```cpp
void DialogInit(HWND hwnd);
```

- `hwnd`：对话框的窗口句柄。

在对话框初始化函数中，可以创建对话框内的控件，类似于 `InitLogic` 函数。

### 对话框逻辑函数

```cpp
void DialogLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id);
```

- `hwnd`：对话框的窗口句柄。
- `uMsg`：消息类型。
- `wParam` 和 `lParam`：消息的附加参数。
- `id`：触发事件的控件的唯一编号。

对话框逻辑函数用于处理对话框内控件的事件，类似于 `Logic` 函数。

>## 完整示例

以下代码展示了如何创建和使用对话框：

```cpp
#include "CVUIL.h"

// 主窗口
void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"对话框示例";
    *width = 400;
    *height = 300;
}

void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Button(hwnd, L"显示模态对话框", 1, 10, 10, 150, 30);
    Button(hwnd, L"显示非模态对话框", 2, 10, 50, 150, 30);
}

// 模态对话框函数
void ModalDialogInit(HWND hwnd)
{
    TextBox(hwnd, L"这是一个模态对话框", 1, 10, 10, 200, 20);
    Button(hwnd, L"关闭", IDCANCEL, 80, 60, 80, 30);
}

void ModalDialogLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == IDCANCEL)
    {
        // 模态对话框会自动关闭，不需要手动调用 CloseDialog
    }
}

// 非模态对话框函数
HWND modelessDialog = NULL;

void ModelessDialogInit(HWND hwnd)
{
    TextBox(hwnd, L"这是一个非模态对话框", 1, 10, 10, 200, 20);
    Button(hwnd, L"关闭", IDCANCEL, 80, 60, 80, 30);
}

void ModelessDialogLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == IDCANCEL)
    {
        CloseDialog(hwnd);
        modelessDialog = NULL;
    }
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    switch (id)
    {
        case 1: // 显示模态对话框
            ShowModalDialog(hwnd, L"模态对话框", 250, 120, ModalDialogInit, ModalDialogLogic);
            break;
            
        case 2: // 显示非模态对话框
            if (modelessDialog == NULL || !IsWindow(modelessDialog))
            {
                modelessDialog = ShowModelessDialog(hwnd, L"非模态对话框", 250, 120, 
                                                     ModelessDialogInit, ModelessDialogLogic);
            }
            break;
    }
}
```

>## 对话框与主窗口的区别

| 特性 | 主窗口 | 对话框 |
|------|--------|--------|
| 创建方式 | 自动创建 | 通过函数手动创建 |
| 生命周期 | 与程序相同 | 可以动态创建和销毁 |
| 父窗口 | 无 | 有，必须指定父窗口句柄（`hwnd`） |
| 模态性 | 非模态 | 可选择模态或非模态 |
| 控件创建 | 在 `InitLogic` 中 | 在自定义的对话框初始化函数中 |
| 事件处理 | 在 `Logic` 中 | 在自定义的对话框逻辑函数中 |

>## 注意事项

- 模态对话框会禁用父窗口的UI交互，直到该对话框关闭。
- 非模态对话框不会禁用父窗口，用户可以同时操作多个窗口。
- 对话框内的控件 ID 可以与主窗口的控件 ID 重复，因为它们在不同的消息循环中处理。
- 非模态对话框需要保存窗口句柄，以便后续操作（如关闭）。
- 对话框关闭时，其中的控件会自动销毁。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

---
