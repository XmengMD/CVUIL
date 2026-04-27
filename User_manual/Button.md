# 按钮控件 (Button)

按钮控件是 CVUIL 库中最基础的控件之一，用于创建可点击的按钮。按钮可以触发事件（如点击事件），并执行相应的逻辑。

>## 创建按钮

按钮控件通过 `Button` 函数创建，其参数如下：

```cpp
ComponentInfo Button(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 按钮显示的文本
    int id,             // 控件唯一编号
    int x,              // 按钮的横坐标
    int y,              // 按钮的纵坐标
    int width,          // 按钮的宽度
    int height          // 按钮的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。指定该控件显示在哪个界面中。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：按钮上显示的文本，需要使用宽字符字符串（如 `L"Button"`）。
- `id`：按钮的唯一编号，用于区分不同的按钮并处理事件。
- `x` 和 `y`：按钮在主界面中的位置，以像素为单位。左上角为 (0, 0)，向右为x轴，向下为y轴。
- `width` 和 `height`：按钮的宽度和高度，以像素为单位。

### Button返回值

返回一个 `ComponentInfo` 类型的结构体，包含按钮的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 示例代码

### 创建一个简单的按钮

以下代码展示了如何创建一个按钮并将其绑定到主界面：

```cpp
ComponentInfo button1 = Button(mainHwnd, L"Click Me", 1, 10, 10, 100, 50);
```

- `mainHwnd` 是主界面的句柄。
- 按钮显示的文本为 `"Click Me"`。
- 按钮的编号为 `1`。
- 按钮的位置为 `(10, 10)`，宽度为 `100`，高度为 `50`。

### 处理按钮点击事件

按钮的点击事件可以通过 `Logic` 函数处理。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 1) // 检查是否是按钮1的点击事件
    {
        MessageBox(hwnd, L"Button Clicked!", L"Info", MB_OK); // 弹出消息框
    }
}
```

- 当按钮被点击时，`Logic` 函数会被调用。
- 通过检查 `id` 和消息类型，可以确定是否是按钮1的点击事件。
- 在这个示例中，点击按钮会弹出一个消息框。

>## 注意事项

- 按钮的文本需要使用宽字符字符串（如 `L"Button"`）。
- 如果不需要后续处理按钮的事件，可以直接调用 `Button` 函数而不存储返回值。
- 如果需要处理按钮的点击事件，建议使用 `ComponentInfo` 类型声明按钮，以便获取按钮的句柄和编号。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [复选框控件](CheckBox.md) → 下一篇
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

---
