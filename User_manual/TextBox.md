# 静态文本框控件 (TextBox)

静态文本框控件是 CVUIL 库中用于创建静态文本显示的控件。静态文本框通常用于在界面上显示提示信息、标题或其他不可编辑的文本内容。

>## 创建静态文本框

静态文本框控件通过 `TextBox` 函数创建，其参数如下：

```cpp
ComponentInfo TextBox(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 静态文本框显示的文本
    int id,             // 控件唯一编号
    int x,              // 静态文本框的横坐标
    int y,              // 静态文本框的纵坐标
    int width,          // 静态文本框的宽度
    int height          // 静态文本框的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：静态文本框显示的文本，需要使用宽字符字符串（如 `L"Hello, World!"`）。
- `id`：静态文本框的唯一编号，用于区分不同的静态文本框。
- `x` 和 `y`：静态文本框在主界面中的位置，以像素为单位。
- `width` 和 `height`：静态文本框的宽度和高度，以像素为单位。

### TextBox返回值

返回一个 `ComponentInfo` 类型的结构体，包含静态文本框的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 修改静态文本框内容

可以通过 `SetComponentText` 函数动态修改静态文本框的内容，其参数如下：

```cpp
void SetComponentText(HWND hwnd, int id, LPCTSTR text);
```

- `hwnd`：主界面句柄。
- `id`：静态文本框的唯一编号。
- `text`：新的文本内容，需要使用宽字符字符串。

### 示例代码

### 创建一个静态文本框

以下代码展示了如何创建一个静态文本框并将其绑定到主界面：

```cpp
ComponentInfo textBox1 = TextBox(mainHwnd, L"Hello, World!", 7, 10, 10, 200, 30);
```

- `mainHwnd` 是主界面的句柄。
- 静态文本框显示的文本为 `"Hello, World!"`。
- 静态文本框的编号为 `7`。
- 静态文本框的位置为 `(10, 10)`，宽度为 `200`，高度为 `30`。

### 动态修改静态文本框内容

可以通过 `SetComponentText` 函数动态修改静态文本框的内容。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 8) // 假设有一个按钮触发修改静态文本框内容的事件
    {
        SetComponentText(hwnd, 7, L"Text Updated!"); // 修改编号为7的静态文本框内容
    }
}
```

- 当某个事件（如按钮点击）触发时，调用 `SetComponentText` 函数修改静态文本框的内容。
- 修改后的内容为 `"Text Updated!"`。

>## 注意事项

- 静态文本框的文本需要使用宽字符字符串（如 `L"Hello, World!"`）。
- 如果需要动态修改静态文本框的内容，建议使用 `ComponentInfo` 类型声明静态文本框，以便获取静态文本框的句柄和编号。
- 在创建多个静态文本框时，确保每个静态文本框的编号是唯一的。
- 静态文本框的内容默认是不可编辑的。

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
- [单行输入框控件](EditBox.md)
- [多行输入框控件](MultiEditBox.md) ← 上一篇
- [组合框控件](ComboBox.md) → 下一篇
- [列表控件](List.md)
- [表格控件](Table.md)
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
