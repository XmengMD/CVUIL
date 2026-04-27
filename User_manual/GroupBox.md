# 分组框控件 (GroupBox)

分组框控件是 CVUIL 库中用于创建分组框的控件。分组框通常用于将一组控件组合在一起，以便于用户界面的组织和布局。

>## 创建分组框

分组框控件通过 `GroupBox` 函数创建，其参数如下：

```cpp
ComponentInfo GroupBox(
    HWND hwnd,          // 主界面句柄
    LPCTSTR text,       // 分组框内显示的文本
    int id,             // 控件唯一编号
    int x,              // 分组框的横坐标
    int y,              // 分组框的纵坐标
    int width,          // 分组框的宽度
    int height          // 分组框的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `text`：分组框内显示的文本，需要使用宽字符字符串（如 `L"Group Box"`）。
- `id`：分组框的唯一编号，用于区分不同的分组框并处理事件。
- `x` 和 `y`：分组框在主界面中的位置，以像素为单位。
- `width` 和 `height`：分组框的宽度和高度，以像素为单位。

### GroupBox返回值

返回一个 `ComponentInfo` 类型的结构体，包含分组框的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 示例代码

### 创建一个分组框

以下代码展示了如何创建一个分组框并将其绑定到主界面：

```cpp
ComponentInfo groupBox1 = GroupBox(mainHwnd, L"Group Box", 10, 10, 10, 200, 100);
```

- `mainHwnd` 是主界面的句柄。
- 分组框内显示的文本为 `"Group Box"`。
- 分组框的编号为 `10`。
- 分组框的位置为 `(10, 10)`，宽度为 `200`，高度为 `100`。

>## 注意事项

- 分组框的文本需要使用宽字符字符串（如 `L"Group Box"`）。
- 在创建多个分组框时，确保每个分组框的编号是唯一的。
- 分组框通常用于组织其他控件，如按钮、复选框等，但分组框本身不提供额外的功能。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [按钮控件](Button.md)
- [复选框控件](CheckBox.md)
- [单选按钮控件](RadioButton.md) ← 上一篇
- [单行输入框控件](EditBox.md) → 下一篇
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
