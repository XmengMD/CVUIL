# 工具提示控件 (ToolTips)

工具提示控件是 CVUIL 库中用于为其他控件添加提示信息的控件。当鼠标悬停在某个控件上时，工具提示会显示一段简短的文本，帮助用户了解该控件的功能或用途。

>## 创建工具提示

工具提示控件通过 `ToolTips` 函数创建，其参数如下：

```cpp
ComponentInfo ToolTips(
    HWND hwnd,          // 主界面句柄
    int id,             // 父控件的唯一编号
    LPCTSTR text        // 提示文本
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `id`：父控件的唯一编号，工具提示将关联到该控件。
- `text`：工具提示显示的文本，需要使用宽字符字符串（如 `L"Hover over me"`）。

### ToolTips返回值

返回一个 `ComponentInfo` 类型的结构体，包含工具提示的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 示例代码

### 创建一个工具提示

以下代码展示了如何为一个按钮创建一个工具提示：

```cpp
ComponentInfo button1 = Button(mainHwnd, L"Hover over me", 1, 10, 10, 100, 30);
ToolTips(mainHwnd, 1, L"This is a button");
```

- `mainHwnd` 是主界面的句柄。
- 按钮的编号为 `1`。
- 工具提示关联到按钮，并显示文本 `"This is a button"`。

>## 注意事项

- 工具提示的文本需要使用宽字符字符串（如 `L"This is a button"`）。
- 确保父控件的编号 `id` 是有效的，否则工具提示将无法正确关联到控件。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

#### 控件文档
- [按钮控件](Button.md) → 返回第一个控件
- [复选框控件](CheckBox.md)
- [单选按钮控件](RadioButton.md)
- [分组框控件](GroupBox.md)
- [单行输入框控件](EditBox.md)
- [多行输入框控件](MultiEditBox.md)
- [静态文本框控件](TextBox.md)
- [组合框控件](ComboBox.md)
- [列表控件](List.md)
- [表格控件](Table.md)
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md) ← 上一篇
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
