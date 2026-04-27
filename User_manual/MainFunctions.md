# 主要函数介绍

在 CVUIL 库中，`CreateMainInterface`、`InitLogic` 和 `Logic` 是三个关键的基本函数，它们共同负责主界面的创建、初始化和控件的响应逻辑处理。一个CVUIL程序至少需要包含这三个函数，才能正常运行。

### 消息机制

CVUIL 库使用Windows API的消息循环机制来处理用户输入和控件事件。当用户与控件交互时，控件会将事件消息`uMsg`发送给全局的消息队列（Message Queue，范围为该程序实例内），`Logic` 函数会在全局消息队列中实时获取事件消息并被程序调用。

- `uMsg`：消息类型，用于标识事件的类型。
- `wParam` 和 `lParam`：消息的附加参数，用于传递事件的详细信息。具体参数含义依赖于`uMsg`的类型。`id`参数即是从`lParam`中提取的控件编号，用于标识触发事件的控件。
- 更多的消息类型和参数含义请参考Windows API的官方文档。

## 1. CreateMainInterface

`CreateMainInterface` 函数用于设置主界面的基本属性，如标题、宽度和高度。

### 函数原型

```cpp
void CreateMainInterface(LPCTSTR *name, int *width, int *height);
```

### 参数说明

- `name`：指向主界面标题的指针，类型为 `LPCTSTR`。可以通过此参数设置主界面的标题。
- `width`：指向主界面宽度的指针，类型为 `int`。可以通过此参数设置主界面的宽度。
- `height`：指向主界面高度的指针，类型为 `int`。可以通过此参数设置主界面的高度。

### 示例代码

```cpp
void CreateMainInterface(LPCTSTR *name, int *width, int *height) {
    *name = L"主界面名称"; // 设置主界面标题
    *width = 800;         // 设置主界面宽度
    *height = 600;        // 设置主界面高度
}
```

### 注意事项

- 该函数在程序启动时被调用一次，用于初始化主界面的基本属性。
- 如果不设置 `name`、`width` 或 `height`，将使用默认值（标题为 `"MainUI"`，宽度为 `640`，高度为 `360`）。

---

## 2. InitLogic

`InitLogic` 函数用于在主界面创建之前执行初始化逻辑，如创建控件、加载配置文件等。该函数在程序启动时被执行一次且仅执行一次。

### 函数原型

```cpp
void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
```

### 参数说明

- `hwnd`：主界面的句柄。
- `uMsg`：消息类型。
- `wParam` 和 `lParam`：消息的附加参数。

### 示例代码

```cpp
void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 创建按钮
    Button(hwnd, L"Click Me", 1, 10, 10, 100, 30);

    // 创建复选框
    CheckBox(hwnd, L"Check Box 1", CB_UNSELECTED, 2, 10, 50, 100, 20);
    CheckBox(hwnd, L"Check Box 2", CB_SELECTED, 3, 10, 80, 100, 20);

    // 创建单选按钮
    RadioButton(hwnd, L"Radio 1", 4, 10, 120, 100, 20);
    RadioButton(hwnd, L"Radio 2", 5, 10, 150, 100, 20);
    RadioButtonGroup(hwnd, 4, 4); // 将 Radio 1 设置为默认选中

    // 创建组合框
    ComponentInfo combo = ComboBox(hwnd, CB_EDITABLE, 6, 10, 180, 150, 20);
    ComboBoxOption(hwnd, 6, L"Option 1");
    ComboBoxOption(hwnd, 6, L"Option 2");
    ComboBoxDefaultOption(hwnd, 6, 0); // 设置默认选中项为 Option 1

    // 创建输入框
    EditBox(hwnd, L"Default Text", 7, 10, 220, 150, 20);

    // 创建静态文本框
    TextBox(hwnd, L"Static Text", 8, 10, 260, 150, 20);
}
```

### 注意事项

- 该函数在主界面创建之前执行，因此是创建控件和初始化程序状态的理想位置。
- 在此函数中创建的控件将自动绑定到主界面。

---

## 3. Logic

`Logic` 函数用于处理控件的响应逻辑，如按钮点击事件、鼠标移动事件等。该函数会在有控件响应时实时执行。

### 函数原型

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id);
```

### 参数说明

- `hwnd`：主界面的句柄。
- `uMsg`：消息类型。
- `wParam` 和 `lParam`：消息的附加参数。
- `id`：触发事件的控件的唯一编号。

### 示例代码

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id) {
    int checkBoxState = CheckBoxState(hwnd, 3);
    int radioButtonState = RadioButtonState(hwnd, 4);
    int radioButtonState2 = RadioButtonState(hwnd, 5);
    switch (id) {
        case 1: // 按钮点击事件
            MessageBox(hwnd, L"Button Clicked!", L"Info", MB_OK);
            break;

        case 3: // 复选框状态变化事件
            if (checkBoxState == 1) {
                MessageBox(hwnd, L"CheckBox is checked!", L"Info", MB_OK);
            } else if (checkBoxState == 0) {
                MessageBox(hwnd, L"CheckBox is unchecked!", L"Info", MB_OK);
            }
            break;

        case 4: // 单选按钮状态变化事件
            if (radioButtonState == 1) {
                MessageBox(hwnd, L"Option 1 is selected!", L"Info", MB_OK);
            }
            break;

        case 5: // 单选按钮状态变化事件
            if (radioButtonState2 == 1) {
                MessageBox(hwnd, L"Option 2 is selected!", L"Info", MB_OK);
            }
            break;
    }
}
```

### 注意事项

- 该函数会在用户与控件交互时被调用，因此是处理用户输入和控件事件的理想位置。
- `id`是CVUIL库中一个重要的参数，这是每一个控件自身在整个程序中的唯一号码，不可重复，且从1开始，不大于65535（0xFFFF或2^16-1）。

---

## 示例代码

以下是一个完整的示例代码，在mainWindow.cpp中展示了如何使用 `CreateMainInterface`、`InitLogic` 和 `Logic` 函数创建windows程序：

```cpp

// mainWindow.cpp

#include "CVUIL.h"

// 创建主界面
void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"CVUIL Test Application";
    *width = 400;
    *height = 700;
}

// 初始化逻辑函数
void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 创建按钮
    Button(hwnd, L"Click Me", 1, 10, 10, 100, 30);

    // 创建复选框
    CheckBox(hwnd, L"Check Box 1", CB_UNSELECTED, 2, 10, 50, 100, 20);
    CheckBox(hwnd, L"Check Box 2", CB_SELECTED, 3, 10, 80, 100, 20);

    // 创建单选按钮
    RadioButton(hwnd, L"Radio 1", 4, 10, 120, 100, 20);
    RadioButton(hwnd, L"Radio 2", 5, 10, 150, 100, 20);
    RadioButtonGroup(hwnd, 4, 4); // 将 Radio 1 设置为默认选中

    // 创建输入框
    EditBox(hwnd, L"Default Text", 6, 10, 180, 150, 20);

    // 创建静态文本框
    TextBox(hwnd, L"Static Text", 7, 10, 220, 150, 20);

    // 创建组合框
    ComponentInfo combo = ComboBox(hwnd, CB_EDITABLE, 8, 10, 260, 150, 20);
    ComboBoxOption(hwnd, 8, L"Option 1");
    ComboBoxOption(hwnd, 8, L"Option 2");
    ComboBoxDefaultOption(hwnd, 8, 0); // 设置默认选中项为 Option 1

    // 创建列表
    ComponentInfo list = List(hwnd, 9, 10, 300, 150, 100);
    ListItem(hwnd, 9, L"Item 1");
    ListItem(hwnd, 9, L"Item 2");

    // 创建表格
    ComponentInfo table = Table(hwnd, 10, 10, 420, 300, 100);
    TableColumn(hwnd, 10, L"Column 1", 0, 100);
    TableColumn(hwnd, 10, L"Column 2", 1, 100);
    TableRow(hwnd, 10);
    TableItem(hwnd, 10, L"Row 1, Col 1", 0, 0);
    TableItem(hwnd, 10, L"Row 1, Col 2", 0, 1);

    // 创建菜单栏
    ComponentInfo menuBar = MenuBar();
    ComponentInfo fileMenu = MenuItem(menuBar.hMenu, L"File");
    SubMenuItem(fileMenu.hMenu, L"Open", 13);
    SubMenuItem(fileMenu.hMenu, L"Exit", 14);
    SetMenuBar(hwnd, menuBar.hMenu);
    ComponentInfo helpMenu = MenuItem(menuBar.hMenu, L"Help");
    SubMenuItem(helpMenu.hMenu, L"About", 15);
}

// 控件响应逻辑函数
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    switch (id)
    {
        case 1: // 按钮点击事件
            MessageBox(hwnd, L"Button Clicked!", L"Message", MB_OK);
            break;
        case 13: // 菜单项 "Open" 点击事件
            MessageBox(hwnd, L"File Open Clicked!", L"Message", MB_OK);
            break;
        case 14: // 菜单项 "Exit" 点击事件
            PostQuitMessage(0); //该函数为关闭程序
            break;
        case 15: // 菜单项 "About" 点击事件
            MessageBox(hwnd, L"CVUIL Test Application\nVersion 1.0", L"About", MB_OK);
            break;
    }
}
```

你可以复制该代码并保存为mainWindow.cpp，然后使用Visual Studio或其他C++编译器编译运行该程序，即可看到效果。

接下来，你可以继续阅读控件部分文档，了解如何创建各种控件以及管理它们的响应逻辑。

---

### 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
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
- [滑块控件](Slider.md)
- [菜单栏控件](MenuBar.md)
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

#### 功能文档
- [输入框增强功能](EditBoxEnhanced.md)
- [文件对话框](FileDialogs.md)
- [对话框支持](Dialogs.md)
- [异步任务支持](AsyncTasks.md)

---
