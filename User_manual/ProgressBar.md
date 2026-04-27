# 进度条控件 (ProgressBar)

CVUIL 提供进度条控件，用于显示操作的进度。进度条可以直观地展示任务的完成情况，常用于文件传输、数据处理等场景。

>## 创建进度条

进度条控件通过 `ProgressBar` 函数创建，其参数如下：

```cpp
ComponentInfo ProgressBar(
    HWND hwnd,          // 主界面句柄
    int id,             // 控件唯一编号
    int x,              // 进度条的横坐标
    int y,              // 进度条的纵坐标
    int width,          // 进度条的宽度
    int height          // 进度条的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `id`：进度条的唯一编号，用于区分不同的进度条。
- `x` 和 `y`：进度条在主界面中的位置，以像素为单位。
- `width` 和 `height`：进度条的宽度和高度，以像素为单位。

### ProgressBar返回值

返回一个 `ComponentInfo` 类型的结构体，包含进度条的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

### 默认设置

创建的进度条默认范围为 0-100，初始值为 0。

>## 设置进度条范围

可以通过 `SetProgressBarRange` 函数设置进度条的范围，其参数如下：

```cpp
void SetProgressBarRange(HWND hwnd, int id, int min, int max);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：进度条的唯一编号。
- `min`：最小值。
- `max`：最大值。

### 示例代码

```cpp
SetProgressBarRange(hwnd, 10, 0, 1000); // 设置范围为 0-1000
```

>## 设置进度条值

可以通过 `SetProgressBarValue` 函数设置进度条的当前值，其参数如下：

```cpp
void SetProgressBarValue(HWND hwnd, int id, int value);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：进度条的唯一编号。
- `value`：当前值。

### 示例代码

```cpp
SetProgressBarValue(hwnd, 10, 50); // 设置进度为 50%
```

>## 获取进度条值

可以通过 `GetProgressBarValue` 函数获取进度条的当前值，其参数如下：

```cpp
int GetProgressBarValue(HWND hwnd, int id);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：进度条的唯一编号。

### 返回值

返回进度条的当前值。如果控件编号无效，返回 0。

### 示例代码

```cpp
int currentValue = GetProgressBarValue(hwnd, 10);
```

>## 设置进度条步进值

可以通过 `SetProgressBarStep` 函数设置进度条的步进值，其参数如下：

```cpp
void SetProgressBarStep(HWND hwnd, int id, int step);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：进度条的唯一编号。
- `step`：步进值。

### 示例代码

```cpp
SetProgressBarStep(hwnd, 10, 10); // 设置步进值为 10
```

>## 进度条步进

可以通过 `ProgressBarStep` 函数让进度条按步进值增加，其参数如下：

```cpp
void ProgressBarStep(HWND hwnd, int id);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：进度条的唯一编号。

### 示例代码

```cpp
ProgressBarStep(hwnd, 10); // 进度条按步进值增加
```

>## 综合示例

以下代码展示了如何创建和使用进度条：

```cpp
#include "CVUIL.h"

void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"进度条示例";
    *width = 400;
    *height = 200;
}

void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 创建进度条
    ProgressBar(hwnd, 1, 10, 10, 350, 30);
    
    // 创建控制按钮
    Button(hwnd, L"增加10%", 2, 10, 50, 100, 30);
    Button(hwnd, L"重置", 3, 120, 50, 100, 30);
    
    // 创建显示当前进度的文本框
    TextBox(hwnd, L"进度: 0%", 4, 10, 90, 200, 30);
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    int currentValue;
    wchar_t buffer[32];
    
    switch (id)
    {
        case 2: // 增加10%
            currentValue = GetProgressBarValue(hwnd, 1);
            if (currentValue < 100)
            {
                SetProgressBarValue(hwnd, 1, currentValue + 10);
                swprintf(buffer, 32, L"进度: %d%%", currentValue + 10);
                SetComponentText(hwnd, 4, buffer);
            }
            break;
            
        case 3: // 重置
            SetProgressBarValue(hwnd, 1, 0);
            SetComponentText(hwnd, 4, L"进度: 0%");
            break;
    }
}
```

>## 与异步任务配合使用

进度条通常与异步任务配合使用，用于显示后台任务的进度。以下是一个简单的示例：

```cpp
// 后台任务函数
void DownloadTask(void* param, void (*reportProgress)(int))
{
    for (int i = 0; i <= 100; i += 10)
    {
        Sleep(200);  // 模拟下载
        reportProgress(i);  // 报告进度
    }
}

// 进度更新回调
void UpdateProgress(int progress)
{
    SetProgressBarValue(mainHwnd, 10, progress);
    wchar_t buf[32];
    swprintf(buf, 32, L"下载中...%d%%", progress);
    SetComponentText(mainHwnd, 11, buf);
}

// 完成回调
void DownloadComplete(void* param)
{
    SetComponentText(mainHwnd, 11, L"下载完成！");
}

// 使用
RunAsyncWithProgress(DownloadTask, UpdateProgress, DownloadComplete, NULL);
```

更多关于异步任务的信息，请参考 [异步任务支持](AsyncTasks.md)。

>## 注意事项

- 进度条的值必须在设置的范围内，超出范围的值会被自动调整到边界值。
- 进度条不会自动更新，需要程序主动调用 `SetProgressBarValue` 或 `ProgressBarStep` 来更新。
- 在创建多个进度条时，确保每个进度条的编号是唯一的。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法
- [异步任务支持](AsyncTasks.md) - 进度条常与异步任务配合使用

---
