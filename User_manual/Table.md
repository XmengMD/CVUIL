# 表格控件 (Table)

表格控件是 CVUIL 库中用于创建表格视图的控件。表格控件可以显示多行多列的数据，并支持用户选择行或单元格。它通常用于显示和管理结构化数据。

>## 创建表格

表格控件通过 `Table` 函数创建，其参数如下：

```cpp
ComponentInfo Table(
    HWND hwnd,          // 主界面句柄
    int id,             // 控件唯一编号
    int x,              // 表格的横坐标
    int y,              // 表格的纵坐标
    int width,          // 表格的宽度
    int height          // 表格的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `id`：表格的唯一编号，用于区分不同的表格并处理事件。
- `x` 和 `y`：表格在主界面中的位置，以像素为单位。
- `width` 和 `height`：表格的宽度和高度，以像素为单位。

### Table返回值

返回一个 `ComponentInfo` 类型的结构体，包含表格的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为表格添加列

可以通过 `TableColumn` 函数为表格添加列，其参数如下：

```cpp
void TableColumn(
    HWND hwnd,          // 主界面句柄
    int id,             // 表格的唯一编号
    LPCTSTR text,       // 列名
    int col,            // 列序号（从 0 开始）
    int width           // 列宽度
);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。
- `text`：列名，需要使用宽字符字符串（如 `L"Column 1"`）。
- `col`：列序号，从 0 开始。
- `width`：列宽度，以像素为单位。

>## 为表格添加行

可以通过 `TableRow` 函数为表格添加行，其参数如下：

```cpp
void TableRow(
    HWND hwnd,          // 主界面句柄
    int id              // 表格的唯一编号
);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。

>## 为表格设置单元格内容

可以通过 `TableItem` 函数为表格设置单元格内容，其参数如下：

```cpp
void TableItem(
    HWND hwnd,          // 主界面句柄
    int id,             // 表格的唯一编号
    LPCTSTR text,       // 单元格内容
    int row,            // 行序号（从 0 开始）
    int col             // 列序号（从 0 开始）
);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。
- `text`：单元格内容，需要使用宽字符字符串（如 `L"Cell Content"`）。
- `row`：行序号，从 0 开始。
- `col`：列序号，从 0 开始。

>## 删除表格行

可以通过 `TableDeleteRow` 函数删除表格中的指定行，其参数如下：

```cpp
void TableDeleteRow(HWND hwnd, int id, int row);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。
- `row`：要删除的行序号（从 0 开始）。

>## 清空表格

可以通过 `TableClear` 函数清空表格中的所有行，其参数如下：

```cpp
void TableClear(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。

>## 获取表格选中行

可以通过 `TableGetSelectedRow` 函数获取表格当前选中的行，其参数如下：

```cpp
int TableGetSelectedRow(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。

### TableGetSelectedRow返回值

返回当前选中行的索引（从 0 开始）。如果没有选中行或控件编号无效，返回 `-1`。

>## 获取表格行数

可以通过 `TableGetRowCount` 函数获取表格的行数，其参数如下：

```cpp
int TableGetRowCount(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。

### TableGetRowCount返回值

返回表格的行数。如果控件编号无效，返回 `0`。

>## 获取表格列数

可以通过 `TableGetColumnCount` 函数获取表格的列数，其参数如下：

```cpp
int TableGetColumnCount(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：表格的唯一编号。

### TableGetColumnCount返回值

返回表格的列数。如果控件编号无效，返回 `0`。

>## 示例代码

### 创建一个表格并添加列和行

以下代码展示了如何创建一个表格并为其添加列和行：

```cpp
ComponentInfo table1 = Table(mainHwnd, 13, 10, 10, 400, 300); // 创建表格
TableColumn(mainHwnd, 13, L"Column 1", 0, 100); // 添加第一列
TableColumn(mainHwnd, 13, L"Column 2", 1, 150); // 添加第二列
TableColumn(mainHwnd, 13, L"Column 3", 2, 150); // 添加第三列

TableRow(mainHwnd, 13); // 启用第一行
TableItem(mainHwnd, 13, L"Cell 1-1", 0, 0); // 设置单元格 (0, 0) 的内容
TableItem(mainHwnd, 13, L"Cell 1-2", 0, 1); // 设置单元格 (0, 1) 的内容
TableItem(mainHwnd, 13, L"Cell 1-3", 0, 2); // 设置单元格 (0, 2) 的内容

TableRow(mainHwnd, 13); // 启用第二行
TableItem(mainHwnd, 13, L"Cell 2-1", 1, 0); // 设置单元格 (1, 0) 的内容
TableItem(mainHwnd, 13, L"Cell 2-2", 1, 1); // 设置单元格 (1, 1) 的内容
TableItem(mainHwnd, 13, L"Cell 2-3", 1, 2); // 设置单元格 (1, 2) 的内容
```

- `mainHwnd` 是主界面的句柄。
- 表格的编号为 `13`。
- 表格的位置为 `(10, 10)`，宽度为 `400`，高度为 `300`。
- 添加了三列和两行，并设置了每个单元格的内容。

>## 注意事项

- 表格的列名和单元格内容需要使用宽字符字符串（如 `L"Column 1"`）。
- 在创建多个表格时，确保每个表格的编号是唯一的。
- 添加列时，列序号必须从 0 开始，并且连续。
- 添加行时，行序号会自动递增，无需手动指定。
- 设置单元格内容时，行序号和列序号必须在有效范围内。

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
- [组合框控件](ComboBox.md)
- [列表控件](List.md) ← 上一篇
- [滑块控件](Slider.md) → 下一篇
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
