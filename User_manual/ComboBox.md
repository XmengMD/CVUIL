# 组合框控件 (ComboBox)

组合框控件是 CVUIL 库中用于创建下拉选择框的控件。用户可以从下拉列表中选择一个选项，也可以在可编辑的组合框中输入自定义内容（如果允许编辑）。

>## 创建组合框

组合框控件通过 `ComboBox` 函数创建，其参数如下：

```cpp
ComponentInfo ComboBox(
    HWND hwnd,          // 主界面句柄
    int editable,       // 是否可编辑（1 表示可编辑，0 表示不可编辑）
    int id,             // 控件唯一编号
    int x,              // 组合框的横坐标
    int y,              // 组合框的纵坐标
    int width,          // 组合框的宽度
    int height          // 组合框的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `editable`：组合框是否可编辑。`1` 表示可编辑，`0` 表示不可编辑。
- `id`：组合框的唯一编号，用于区分不同的组合框并处理事件。
- `x` 和 `y`：组合框在主界面中的位置，以像素为单位。
- `width` 和 `height`：组合框的宽度和高度，以像素为单位。

### ComboBox返回值

返回一个 `ComponentInfo` 类型的结构体，包含组合框的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为组合框添加选项

可以通过 `ComboBoxOption` 函数为组合框添加选项，其参数如下：

```cpp
void ComboBoxOption(
    HWND hwnd,          // 主界面句柄
    int id,             // 组合框的唯一编号
    LPCTSTR text        // 选项文本
);
```

- `hwnd`：主界面句柄。
- `id`：组合框的唯一编号。
- `text`：选项文本，需要使用宽字符字符串（如 `L"Option 1"`）。

>## 设置组合框的默认选中项

可以通过 `ComboBoxDefaultOption` 函数设置组合框的默认选中项，其参数如下：

```cpp
void ComboBoxDefaultOption(
    HWND hwnd,          // 主界面句柄
    int id,             // 组合框的唯一编号
    int index           // 选项索引（从 0 开始）
);
```

- `hwnd`：主界面句柄。
- `id`：组合框的唯一编号。
- `index`：选项索引，从 0 开始。

>## 获取组合框的选中项

可以通过 `ComboBoxOptionState` 函数获取组合框的当前选中项，其参数如下：

```cpp
int ComboBoxOptionState(
    HWND hwnd,          // 主界面句柄
    int id              // 组合框的唯一编号
);
```

### ComboBoxOptionState返回值

- 返回当前选中项的索引（从 0 开始）。
- 如果没有选中项或控件编号无效，返回 `-1`。

>## 示例代码

### 创建一个组合框并添加选项

以下代码展示了如何创建一个组合框并为其添加选项：

```cpp
ComponentInfo comboBox1 = ComboBox(mainHwnd, 0, 9, 10, 10, 200, 30); // 创建不可编辑的组合框
ComboBoxOption(mainHwnd, 9, L"Option 1");
ComboBoxOption(mainHwnd, 9, L"Option 2");
ComboBoxOption(mainHwnd, 9, L"Option 3");
ComboBoxDefaultOption(mainHwnd, 9, 1); // 默认选中第二个选项
```

- `mainHwnd` 是主界面的句柄。
- 组合框的编号为 `9`。
- 组合框的位置为 `(10, 10)`，宽度为 `200`，高度为 `30`。
- 添加了三个选项：`"Option 1"`、`"Option 2"` 和 `"Option 3"`。
- 默认选中第二个选项（索引为 `1`）。

### 获取组合框的选中项并处理

可以通过 `ComboBoxOptionState` 函数获取组合框的当前选中项，并在程序中使用。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 10) // 假设有一个按钮触发获取组合框选中项的事件
    {
        int selectedIndex = ComboBoxOptionState(hwnd, 9); // 获取编号为9的组合框的选中项
        if (selectedIndex != -1)
        {
            MessageBox(hwnd, L"Option selected", L"Info", MB_OK); // 弹出消息框
        }
    }
}
```

- 当某个事件（如按钮点击）触发时，调用 `ComboBoxOptionState` 函数获取组合框的选中项。
- 如果有选中项，则弹出一个消息框。

>## 注意事项

- 组合框的选项文本需要使用宽字符字符串（如 `L"Option 1"`）。
- 如果组合框是可编辑的，用户可以在输入框中输入自定义内容。
- 在创建多个组合框时，确保每个组合框的编号是唯一的。
- 添加选项时，确保选项文本不为空。

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
- [多行输入框控件](MultiEditBox.md)
- [静态文本框控件](TextBox.md) ← 上一篇
- [列表控件](List.md) → 下一篇
- [表格控件](Table.md)
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
