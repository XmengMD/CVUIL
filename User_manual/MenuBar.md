# 菜单栏控件 (MenuBar)

菜单栏控件是 CVUIL 库中用于创建菜单栏的控件。菜单栏通常位于窗口的顶部，包含多个菜单项和子菜单，用于提供程序的各种功能选项。

>## 创建菜单栏

菜单栏控件通过 `MenuBar` 函数创建，其参数如下：

```cpp
ComponentInfo MenuBar();
```

### 参数说明

- 无参数。`MenuBar` 函数返回一个包含菜单栏的 `ComponentInfo` 结构体。

### MenuBar返回值

返回一个 `ComponentInfo` 类型的结构体，包含菜单栏的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为菜单栏添加菜单项

可以通过 `MenuItem` 函数为菜单栏添加菜单项，其参数如下：

```cpp
ComponentInfo MenuItem(
    HMENU hmenu,          // 菜单栏的 HMENU 句柄
    LPCTSTR text          // 菜单项的文本
);
```

- `hmenu`：菜单栏的 HMENU 句柄。
- `text`：菜单项的文本，需要使用宽字符字符串（如 `L"File"`）。
- 菜单栏中菜单项的顺序从左到右，由代码从上到下的顺序决定。

### MenuItem返回值

返回一个 `ComponentInfo` 类型的结构体，包含菜单项的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为菜单项添加子菜单项

子菜单项与按钮控件功能类似，同样拥有唯一的控件编号，只是它单独创建在菜单中，且由hmenu句柄管理。可以通过 `SubMenuItem` 函数为菜单项添加子菜单项，其参数如下：

```cpp
ComponentInfo SubMenuItem(
    HMENU hmenu,          // 菜单项的 HMENU 句柄
    LPCTSTR text,         // 子菜单项的文本
    int id                // 子菜单项的唯一编号
);
```

- `hmenu`：菜单项的 HMENU 句柄。
- `text`：子菜单项的文本，需要使用宽字符字符串（如 `L"Open"`）。
- `id`：子菜单项的唯一编号，用于区分不同的子菜单项并处理事件。

>## 将菜单栏设置到主界面

可以通过 `SetMenuBar` 函数将菜单栏设置到主界面，其参数如下：

```cpp
void SetMenuBar(
    HWND hwnd,            // 主界面句柄
    HMENU hmenu           // 菜单栏的 HMENU 句柄
);
```

- `hwnd`：主界面的句柄。
- `hmenu`：菜单栏的 HMENU 句柄。

>## 示例代码

### 创建一个菜单栏并添加菜单项和子菜单

以下代码展示了如何创建一个菜单栏并为其添加菜单项和子菜单：

```cpp
ComponentInfo menuBar = MenuBar(); // 创建菜单栏
ComponentInfo fileMenu = MenuItem(menuBar.hMenu, L"File"); // 添加 "File" 菜单项
SubMenuItem(fileMenu.hMenu, L"Open", 20); // 添加 "Open" 子菜单项
SubMenuItem(fileMenu.hMenu, L"Save", 21); // 添加 "Save" 子菜单项
SubMenuItem(fileMenu.hMenu, L"Exit", 22); // 添加 "Exit" 子菜单项

SetMenuBar(mainHwnd, menuBar.hMenu); // 将菜单栏设置到主界面
```

- `mainHwnd` 是主界面的句柄。
- 菜单栏包含一个 `"File"` 菜单项，该菜单项包含 `"Open"`、`"Save"` 和 `"Exit"` 三个子菜单项。

### 处理子菜单项点击事件

可以通过 `Logic` 函数处理菜单项的点击事件。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    switch (id)
    {
        case 20: // "Open" 子菜单项被点击
            MessageBox(hwnd, L"Open clicked", L"Info", MB_OK);
            break;
        case 21: // "Save" 子菜单项被点击
            MessageBox(hwnd, L"Save clicked", L"Info", MB_OK);
            break;
        case 22: // "Exit" 子菜单项被点击
            PostQuitMessage(0); // 退出程序
            break;
    }
}
```

- 当某个子菜单项被点击时，`Logic` 函数会被调用。
- 通过检查 `id`，可以确定是哪个菜单项被点击，并执行相应的操作。

>## 注意事项

- 菜单项和子菜单项的文本需要使用宽字符字符串（如 `L"File"`）。
- 在创建多个菜单项和子菜单项时，确保每个子菜单项的编号是唯一的，且和其他控件的编号不能冲突。
- 菜单栏必须通过 `SetMenuBar` 函数设置到主界面，才能正常显示和工作。
- 子菜单项的点击事件通过 `Logic` 函数处理，需要在该函数中根据 `id` 进行判断。

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
- [列表控件](List.md)
- [表格控件](Table.md)
- [滑块控件](Slider.md) ← 上一篇
- [工具提示控件](ToolTips.md) → 下一篇
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
