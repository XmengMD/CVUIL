# 复选框控件 (CheckBox)

复选框控件是 CVUIL 库中用于创建可选中或取消选中的复选框控件。复选框可以用于表示布尔值（选中或未选中），并触发相应的事件。

>## 创建复选框

复选框控件通过 `CheckBox` 函数创建，其参数如下：

```cpp
ComponentInfo CheckBox(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 复选框显示的文本
    int defaultStatus,  // 默认状态（选中或未选中）
    int id,             // 控件唯一编号
    int x,              // 复选框的横坐标
    int y,              // 复选框的纵坐标
    int width,          // 复选框的宽度
    int height          // 复选框的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：复选框上显示的文本，需要使用宽字符字符串（如 `L"Check Me"`）。
- `defaultStatus`：复选框的默认状态，`1` 表示默认选中，`0` 表示默认未选中。
- `id`：复选框的唯一编号，用于区分不同的复选框并处理事件。
- `x` 和 `y`：复选框在主界面中的位置，以像素为单位。
- `width` 和 `height`：复选框的宽度和高度，以像素为单位。

### CheckBox返回值

返回一个 `ComponentInfo` 类型的结构体，包含复选框的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 获取复选框状态

可以通过 `CheckBoxState` 函数获取复选框的当前状态，其参数如下：

```cpp
int CheckBoxState(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：复选框的唯一编号。

### CheckBoxState返回值

- `1`：表示复选框当前处于选中状态。
- `0`：表示复选框当前处于未选中状态。
- `-1`：表示复选框状态不确定（例如控件编号无效）。

>## 示例代码

### 创建一个复选框

以下代码展示了如何创建一个复选框并将其绑定到主界面：

```cpp
ComponentInfo checkBox1 = CheckBox(mainHwnd, L"Check Me", 0, 2, 10, 70, 100, 30);
```

- `mainHwnd` 是主界面的句柄。
- 复选框显示的文本为 `"Check Me"`。
- 默认状态为 `0`（未选中）。
- 复选框的编号为 `2`。
- 复选框的位置为 `(10, 70)`，宽度为 `100`，高度为 `30`。

### 处理复选框状态变化事件

复选框的状态变化事件可以通过 `Logic` 函数处理。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 2) // 检查是否是复选框1的状态变化事件
    {
        int state = CheckBoxState(hwnd, 2);
        if (state == 1)
        {
            MessageBox(hwnd, L"CheckBox is checked!", L"Info", MB_OK); // 弹出消息框
        }
        else if (state == 0)
        {
            MessageBox(hwnd, L"CheckBox is unchecked!", L"Info", MB_OK); // 弹出消息框
        }
    }
}
```

- 当复选框的状态发生变化时，`Logic` 函数会被调用。
- 通过检查 `id` 和消息类型，可以确定是否是复选框1的状态变化事件。
- 在这个示例中，当复选框被选中或取消选中时，会弹出一个消息框。

>## 注意事项

- 复选框的文本需要使用宽字符字符串（如 `L"Check Me"`）。
- 如果需要处理复选框的状态变化事件，建议使用 `ComponentInfo` 类型声明复选框，以便获取复选框的句柄和编号。
- 在创建多个复选框时，确保每个复选框的编号是唯一的。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [按钮控件](Button.md) ← 上一篇
- [单选按钮控件](RadioButton.md) → 下一篇
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
