# 单选按钮控件 (RadioButton)

单选按钮控件是 CVUIL 库中用于创建单选按钮的控件。单选按钮通常用于一组选项中选择一个，同一组内的单选按钮互斥，只能选择一个。

>## 创建单选按钮

单选按钮控件通过 `RadioButton` 函数创建，其参数如下：

```cpp
ComponentInfo RadioButton(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 单选按钮显示的文本
    int id,             // 控件唯一编号
    int x,              // 单选按钮的横坐标
    int y,              // 单选按钮的纵坐标
    int width,          // 单选按钮的宽度
    int height          // 单选按钮的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：单选按钮上显示的文本，需要使用宽字符字符串（如 `L"Option 1"`）。
- `id`：单选按钮的唯一编号，用于区分不同的单选按钮并处理事件。
- `x` 和 `y`：单选按钮在主界面中的位置，以像素为单位。
- `width` 和 `height`：单选按钮的宽度和高度，以像素为单位。

### RadioButton返回值

返回一个 `ComponentInfo` 类型的结构体，包含单选按钮的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 合并单选按钮组

可以通过 `RadioButtonGroup` 函数将多个单选按钮组合成一组，其参数如下：

```cpp
void RadioButtonGroup(
    HWND hwnd,          // 主界面句柄
    int firstRadioButton, // 组内第一个单选按钮的编号
    int defaultRadioButton // 默认选中的单选按钮编号
);
```

- `hwnd`：主界面句柄。
- `firstRadioButton`：组内第一个单选按钮的编号。
- `defaultRadioButton`：默认选中的单选按钮编号。
- **为什么没有组内最后一个单选按钮的编号？**
  - `RadioButtonGroup`函数即为一个单选按钮组的末尾，会从`firstRadioButton`由代码从上到下的顺序自动识别单选按钮组范围，并合并所有连续的单选按钮。

### 获取单选按钮状态

可以通过 `RadioButtonState` 函数获取单选按钮的当前状态，其参数如下：

```cpp
int RadioButtonState(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：单选按钮的唯一编号。

### RadioButtonState返回值

- `1`：表示单选按钮当前处于选中状态。
- `0`：表示单选按钮当前处于未选中状态。
- `-1`：表示单选按钮状态不确定（例如控件编号无效）。

>## 示例代码

### 创建一组单选按钮

以下代码展示了如何创建一组单选按钮并将其绑定到主界面：

```cpp
ComponentInfo radioButton1 = RadioButton(mainHwnd, L"Option 1", 3, 10, 10, 100, 30);
ComponentInfo radioButton2 = RadioButton(mainHwnd, L"Option 2", 4, 10, 50, 100, 30);
RadioButtonGroup(mainHwnd, 3, 3); // 将编号为3和4的单选按钮组合成一组，并默认选中编号为3的按钮
```

- `mainHwnd` 是主界面的句柄。
- 单选按钮的文本分别为 `"Option 1"` 和 `"Option 2"`。
- 单选按钮的编号分别为 `3` 和 `4`。
- 单选按钮的位置分别为 `(10, 10)` 和 `(10, 50)`，宽度为 `100`，高度为 `30`。

### 处理单选按钮状态变化事件

单选按钮的状态变化事件可以通过 `Logic` 函数处理。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 3 || id == 4) // 检查是否是单选按钮的状态变化事件
    {
        int state1 = RadioButtonState(hwnd, 3);
        int state2 = RadioButtonState(hwnd, 4);
        if (state1 == 1)
        {
            MessageBox(hwnd, L"Option 1 is selected!", L"Info", MB_OK); // 弹出消息框
        }
        else if (state2 == 1)
        {
            MessageBox(hwnd, L"Option 2 is selected!", L"Info", MB_OK); // 弹出消息框
        }
    }
}
```

- 当单选按钮的状态发生变化时，`Logic` 函数会被调用。
- 通过检查 `id` 和消息类型，可以确定是哪个单选按钮的状态发生了变化。
- 在这个示例中，当单选按钮被选中时，会弹出一个消息框。

>## 注意事项

- 单选按钮的文本需要使用宽字符字符串（如 `L"Option 1"`）。
- 同一组内的单选按钮编号必须不同。
- 在创建多个单选按钮时，确保每个单选按钮的编号是唯一的。
- 完成单选按钮的创建后，必须要使用`RadioButtonGroup` 函数来合并单选按钮组结尾，该函数会按照代码的上下顺序将单选按钮合成独立的一组
- 使用 `RadioButtonGroup` 函数将单选按钮合成一组后，组内的单选按钮将互斥（只能选择一个）。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [按钮控件](Button.md)
- [复选框控件](CheckBox.md) ← 上一篇
- [分组框控件](GroupBox.md) → 下一篇
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
