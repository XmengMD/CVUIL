# 列表控件 (List)

列表控件是 CVUIL 库中用于创建列表框的控件。用户可以在列表框中选择一个或多个选项，程序可以通过获取选中项来实现交互。

>## 创建列表

列表控件通过 `List` 函数创建，其参数如下：

```cpp
ComponentInfo List(
    HWND hwnd,          // 主界面句柄
    int id,             // 控件唯一编号
    int x,              // 列表的横坐标
    int y,              // 列表的纵坐标
    int width,          // 列表的宽度
    int height          // 列表的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `id`：列表的唯一编号，用于区分不同的列表并处理事件。
- `x` 和 `y`：列表在主界面中的位置，以像素为单位。
- `width` 和 `height`：列表的宽度和高度，以像素为单位。

### List返回值

返回一个 `ComponentInfo` 类型的结构体，包含列表的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为列表添加选项

可以通过 `ListItem` 函数为列表添加选项，其参数如下：

```cpp
void ListItem(
    HWND hwnd,          // 主界面句柄
    int id,             // 列表的唯一编号
    LPCTSTR text        // 选项文本
);
```

- `hwnd`：主界面句柄。
- `id`：列表的唯一编号。
- `text`：选项文本，需要使用宽字符字符串（如 `L"Item 1"`）。

>## 删除列表项

可以通过 `ListDeleteItem` 函数删除列表中的指定项，其参数如下：

```cpp
void ListDeleteItem(HWND hwnd, int id, int index);
```

- `hwnd`：主界面句柄。
- `id`：列表的唯一编号。
- `index`：要删除的项的索引（从 0 开始）。

>## 清空列表

可以通过 `ListClear` 函数清空列表中的所有项，其参数如下：

```cpp
void ListClear(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：列表的唯一编号。

>## 获取列表的选中项

可以通过 `ListItemState` 函数获取列表的当前选中项，其参数如下：

```cpp
int ListItemState(
    HWND hwnd,          // 主界面句柄
    int id              // 列表的唯一编号
);
```

### ListItemState返回值

- 返回当前选中项的索引（从 0 开始）。
- 如果没有选中项或控件编号无效，返回 `-1`。

>## 获取列表选中项的文本

可以通过 `ListGetSelectedText` 函数获取列表当前选中项的文本，其参数如下：

```cpp
LPCTSTR ListGetSelectedText(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：列表的唯一编号。

### ListGetSelectedText返回值

返回选中项的文本。如果没有选中项或控件编号无效，返回 `NULL`。

>## 获取列表项数量

可以通过 `ListGetItemCount` 函数获取列表中的项数量，其参数如下：

```cpp
int ListGetItemCount(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：列表的唯一编号。

### ListGetItemCount返回值

返回列表中的项数量。如果控件编号无效，返回 `0`。

>## 示例代码

### 创建一个列表并添加选项

以下代码展示了如何创建一个列表并为其添加选项：

```cpp
ComponentInfo list1 = List(mainHwnd, 11, 10, 10, 200, 100); // 创建列表
ListItem(mainHwnd, 11, L"Item 1");
ListItem(mainHwnd, 11, L"Item 2");
ListItem(mainHwnd, 11, L"Item 3");
```

- `mainHwnd` 是主界面的句柄。
- 列表的编号为 `11`。
- 列表的位置为 `(10, 10)`，宽度为 `200`，高度为 `100`。
- 添加了三个选项：`"Item 1"`、`"Item 2"` 和 `"Item 3"`。

### 获取列表的选中项并处理

可以通过 `ListItemState` 函数获取列表的当前选中项，并在程序中使用。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 12) // 假设有一个按钮触发获取列表选中项的事件
    {
        int selectedIndex = ListItemState(hwnd, 11); // 获取编号为11的列表的选中项
        if (selectedIndex != -1)
        {
            LPCTSTR selectedText = ListGetSelectedText(hwnd, 11);
            if (selectedText != NULL)
            {
                MessageBox(hwnd, selectedText, L"Selected Item", MB_OK);
            }
        }
    }
}
```

- 当某个事件（如按钮点击）触发时，调用 `ListItemState` 函数获取列表的选中项。
- 如果有选中项，则弹出一个消息框显示选中项的文本。

>## 注意事项

- 列表的选项文本需要使用宽字符字符串（如 `L"Item 1"`）。
- 在创建多个列表时，确保每个列表的编号是唯一的。
- 添加选项时，确保选项文本不为空。
- 用户可以通过鼠标点击或键盘操作选择列表中的选项。

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
- [静态文本框控件](TextBox.md)
- [组合框控件](ComboBox.md) ← 上一篇
- [表格控件](Table.md) → 下一篇
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
