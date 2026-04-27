# 多行输入框控件 (MultiEditBox)

多行输入框控件是 CVUIL 库中用于创建多行文本输入框的控件。用户可以在多行输入框中输入和编辑多行文本，程序可以通过获取输入框的内容来实现交互。

>## 创建多行输入框

多行输入框控件通过 `MultiEditBox` 函数创建，其参数如下：

```cpp
ComponentInfo MultiEditBox(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 输入框默认显示的文本
    int id,             // 控件唯一编号
    int x,              // 输入框的横坐标
    int y,              // 输入框的纵坐标
    int width,          // 输入框的宽度
    int height          // 输入框的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：输入框默认显示的文本，需要使用宽字符字符串（如 `L"Default Text"`）。
- `id`：输入框的唯一编号，用于区分不同的输入框并处理事件。
- `x` 和 `y`：输入框在主界面中的位置，以像素为单位。
- `width` 和 `height`：输入框的宽度和高度，以像素为单位。

### MultiEditBox返回值

返回一个 `ComponentInfo` 类型的结构体，包含输入框的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 获取多行输入框内容

可以通过 `GetMultiEditBoxText` 函数获取多行输入框的当前内容，其参数如下：

```cpp
LPCTSTR GetMultiEditBoxText(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：输入框的唯一编号。

### GetMultiEditBoxText返回值

返回一个指向输入框内容的宽字符字符串指针。如果输入框中没有内容或控件编号无效，返回 `NULL`。

>## 示例代码

### 创建一个多行输入框

以下代码展示了如何创建一个多行输入框并将其绑定到主界面：

```cpp
ComponentInfo multiEditBox1 = MultiEditBox(mainHwnd, L"Default Text\nSecond Line", 6, 10, 10, 200, 100);
```

- `mainHwnd` 是主界面的句柄。
- 输入框默认显示的文本为 `"Default Text\nSecond Line"`。
- 输入框的编号为 `6`。
- 输入框的位置为 `(10, 10)`，宽度为 `200`，高度为 `100`。

### 获取多行输入框内容并处理

可以通过 `GetMultiEditBoxText` 函数获取多行输入框的内容，并在程序中使用。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 7) // 假设有一个按钮触发获取多行输入框内容的事件
    {
        LPCTSTR text = GetMultiEditBoxText(hwnd, 6); // 获取编号为6的多行输入框内容
        if (text != NULL)
        {
            MessageBox(hwnd, text, L"Multi-Line Input Text", MB_OK); // 弹出消息框显示输入的内容
        }
    }
}
```

- 当某个事件（如按钮点击）触发时，调用 `GetMultiEditBoxText` 函数获取多行输入框的内容。
- 如果输入框中有内容，则将其显示在消息框中。

>## 注意事项

- 多行输入框的默认文本需要使用宽字符字符串（如 `L"Default Text\nSecond Line"`）。
- 如果需要处理多行输入框的内容，建议使用 `ComponentInfo` 类型声明多行输入框，以便获取输入框的句柄和编号。
- 在创建多个多行输入框时，确保每个输入框的编号是唯一的。
- 多行输入框支持换行符（`\n`），可以显示多行文本。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [按钮控件](Button.md)
- [复选框控件](CheckBox.md)
- [单选按钮控件](RadioButton.md)
- [分组框控件](GroupBox.md)
- [单行输入框控件](EditBox.md) ← 上一篇
- [静态文本框控件](TextBox.md) → 下一篇
- [组合框控件](ComboBox.md)
- [列表控件](List.md)
- [表格控件](Table.md)
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
