# 异步任务支持 (Async Tasks)

CVUIL 支持异步任务功能，这是 CVUIL 框架的重要特性。异步任务允许在后台线程执行耗时操作，而不会阻塞 UI，从而解决单线程下 UI 卡顿问题。

>## 核心特性

- ✅ **后台执行** - 耗时任务在后台线程运行，不阻塞UI
- ✅ **线程安全** - 自动处理UI更新，无需手动同步
- ✅ **进度报告** - 支持实时进度反馈
- ✅ **延时执行** - 简单的延时任务支持
- ✅ **任务管理** - 支持取消任务和查询状态

>## 启动简单异步任务

可以通过 `RunAsync` 函数启动一个简单的异步任务，其参数如下：

```cpp
void RunAsync(AsyncTaskFunc taskFunc, AsyncCallbackFunc callbackFunc, void* param);
```

### 参数说明

- `taskFunc` - 后台执行的任务函数，类型为 `void (*)(void*)`。
- `callbackFunc` - 任务完成后在主线程执行的回调（可安全更新UI），类型为 `void (*)(void*)`。
- `param` - 传递给任务的参数（可选，可为 `NULL`）。

### 回调函数类型定义

```cpp
typedef void (*AsyncTaskFunc)(void* param);
typedef void (*AsyncCallbackFunc)(void* result);
```

### 使用示例

```cpp
// 后台任务函数
void LoadFileTask(void* param)
{
    LPCTSTR path = (LPCTSTR)param;
    // 模拟耗时操作（3秒）
    Sleep(3000);
    // 加载文件...
}

// 完成回调（在主线程执行）
void LoadFileCallback(void* param)
{
    LPCTSTR path = (LPCTSTR)param;
    SetComponentText(mainHwnd, 2, L"加载完成！");
    FreeString(path);
}

// 使用
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        LPCTSTR path = OpenFileDialog(hwnd, L"文本文件|*.txt");
        if (path != NULL)
        {
            RunAsync(LoadFileTask, LoadFileCallback, (void*)path);
            SetComponentText(hwnd, 2, L"加载中...");
        }
    }
}
```

>## 启动带进度的异步任务

可以通过 `RunAsyncWithProgress` 函数启动一个带进度的异步任务，其参数如下：

```cpp
void RunAsyncWithProgress(
    AsyncTaskWithProgressFunc taskFunc, 
    AsyncProgressFunc progressFunc, 
    AsyncCallbackFunc callbackFunc, 
    void* param
);
```

### 参数说明

- `taskFunc` - 任务函数，内部调用 `ReportProgress()` 报告进度，类型为 `void (*)(void*, void (*)(int))`。
- `progressFunc` - 进度更新回调（在主线程执行），类型为 `void (*)(int)`。
- `callbackFunc` - 完成回调（在主线程执行），类型为 `void (*)(void*)`。
- `param` - 传递给任务的参数。

### 回调函数类型定义

```cpp
typedef void (*AsyncProgressFunc)(int progress);
typedef void (*AsyncTaskWithProgressFunc)(void* param, void (*reportProgress)(int));
```

### 使用示例

```cpp
// 带进度任务
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
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        RunAsyncWithProgress(DownloadTask, UpdateProgress, DownloadComplete, NULL);
    }
}
```

>## 报告进度

在带进度的异步任务中，通过调用 `ReportProgress` 函数来更新进度。

```cpp
void ReportProgress(int progress);
```

### 参数说明

- `progress` - 进度值（0-100）。

### 注意事项

- 只能在 `RunAsyncWithProgress` 启动的任务中调用。
- 建议每 100ms 以上报告一次，避免 UI 过于频繁更新。

>## 延时执行

可以通过 `Delay` 函数实现延时执行，不阻塞 UI，其参数如下：

```cpp
void Delay(int milliseconds, AsyncCallbackFunc callbackFunc, void* param);
```

### 参数说明

- `milliseconds` - 延时毫秒数。
- `callbackFunc` - 延时结束后执行的回调。
- `param` - 传递给回调的参数（可选，可为 `NULL`）。

### 使用示例

```cpp
void DelayCallback(void* param)
{
    MessageBox(mainHwnd, L"3秒到了！", L"提示", MB_OK);
}

// 3秒后执行
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        Delay(3000, DelayCallback, NULL);
    }
}
```

>## 取消所有任务

可以通过 `CancelAllAsyncTasks` 函数取消所有正在运行的异步任务。

```cpp
void CancelAllAsyncTasks();
```

### 说明

- 取消所有正在运行的异步任务。
- 正在执行的任务会继续完成，但回调不会执行。

### 使用示例

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 99) // 取消按钮
    {
        CancelAllAsyncTasks();
        SetComponentText(hwnd, 10, L"任务已取消");
    }
}
```

>## 获取运行中任务数

可以通过 `GetRunningAsyncTaskCount` 函数获取当前正在运行的异步任务数量。

```cpp
int GetRunningAsyncTaskCount();
```

### 返回值

返回当前正在运行的异步任务数量。

### 使用示例

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        int count = GetRunningAsyncTaskCount();
        if (count > 0)
        {
            MessageBox(hwnd, L"有任务正在运行，请等待", L"提示", MB_OK);
            return;
        }
        // 启动新任务
        RunAsync(TaskFunc, CallbackFunc, NULL);
    }
}
```

>## 对比：同步 vs 异步

### ❌ 同步方式（会卡住UI）

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        // 这会卡住UI 3秒！
        Sleep(3000);
        SetComponentText(hwnd, 2, L"完成");
    }
}
```

### ✅ 异步方式（UI保持响应）

```cpp
void BackgroundTask(void* param)
{
    Sleep(3000);  // 在后台线程执行
}

void TaskComplete(void* param)
{
    SetComponentText(mainHwnd, 2, L"完成");  // 安全更新UI
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        RunAsync(BackgroundTask, TaskComplete, NULL);
        SetComponentText(hwnd, 2, L"处理中...");
    }
}
```

>## 完整示例

以下代码展示了异步任务的综合使用：

```cpp
#include "CVUIL.h"

void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"异步任务示例";
    *width = 400;
    *height = 300;
}

void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 创建进度条
    ProgressBar(hwnd, 1, 10, 10, 350, 30);
    
    // 创建状态文本
    TextBox(hwnd, L"就绪", 2, 10, 50, 200, 25);
    
    // 创建按钮
    Button(hwnd, L"开始任务", 3, 10, 90, 100, 30);
    Button(hwnd, L"取消任务", 4, 120, 90, 100, 30);
    
    // 显示任务数量
    TextBox(hwnd, L"运行中任务: 0", 5, 10, 130, 200, 25);
}

// 后台任务
void BackgroundTask(void* param, void (*reportProgress)(int))
{
    for (int i = 0; i <= 100; i += 5)
    {
        Sleep(100);  // 模拟工作
        reportProgress(i);
    }
}

// 进度更新
void UpdateProgress(int progress)
{
    SetProgressBarValue(mainHwnd, 1, progress);
    wchar_t buf[32];
    swprintf(buf, 32, L"处理中...%d%%", progress);
    SetComponentText(mainHwnd, 2, buf);
}

// 任务完成
void TaskComplete(void* param)
{
    SetComponentText(mainHwnd, 2, L"任务完成！");
    SetProgressBarValue(mainHwnd, 1, 100);
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    switch (id)
    {
        case 3: // 开始任务
        {
            int count = GetRunningAsyncTaskCount();
            if (count >= 5)
            {
                MessageBox(hwnd, L"同时运行的任务太多，请稍后再试", L"提示", MB_OK);
                return;
            }
            
            RunAsyncWithProgress(BackgroundTask, UpdateProgress, TaskComplete, NULL);
            SetComponentText(hwnd, 2, L"启动任务...");
            break;
        }
        case 4: // 取消任务
            CancelAllAsyncTasks();
            SetComponentText(hwnd, 2, L"任务已取消");
            SetProgressBarValue(hwnd, 1, 0);
            break;
    }
    
    // 更新任务数量显示
    int count = GetRunningAsyncTaskCount();
    wchar_t buf[32];
    swprintf(buf, 32, L"运行中任务: %d", count);
    SetComponentText(hwnd, 5, buf);
}
```

>## 重要说明

### 线程安全

- **后台任务中** - 不要直接操作UI控件。
- **回调函数中** - 可以安全地更新UI。
- **数据传递** - 通过 `param` 参数传递数据，注意内存管理。

### 限制

- 最大并发任务数：64个。
- 进度报告频率：建议每 100ms 以上报告一次，避免 UI 过于频繁更新。

### 内存管理

```cpp
void Task(void* param)
{
    LPCTSTR data = (LPCTSTR)param;
    // 使用数据...
}

void Callback(void* param)
{
    LPCTSTR data = (LPCTSTR)param;
    // 使用数据...
    FreeString(data);  // 在回调中释放内存
}

// 启动时复制数据
LPCTSTR dataCopy = new WCHAR[wcslen(data) + 1];
wcscpy(dataCopy, data);
RunAsync(Task, Callback, dataCopy);
```

>## 最佳实践

### 1. 耗时操作使用异步

```cpp
// 文件IO、网络请求、复杂计算等耗时操作
void HeavyComputation(void* param)
{
    // 耗时计算...
}

RunAsync(HeavyComputation, OnComplete, data);
```

### 2. 进度反馈提升体验

```cpp
void ProcessWithProgress(void* param, void (*reportProgress)(int))
{
    int total = GetTotalItems();
    for (int i = 0; i < total; i++)
    {
        ProcessItem(i);
        reportProgress((i * 100) / total);  // 报告进度
    }
}
```

### 3. 防止重复启动

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1)
    {
        if (GetRunningAsyncTaskCount() > 0)
        {
            MessageBox(hwnd, L"任务已在运行", L"提示", MB_OK);
            return;
        }
        RunAsync(TaskFunc, CallbackFunc, NULL);
    }
}
```

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法
- [进度条控件](ProgressBar.md) - 常与异步任务配合使用

---
