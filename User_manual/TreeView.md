# 树形控件 (TreeView)

CVUIL 提供树形控件，用于显示层次结构的数据。树形控件常用于显示文件目录、组织结构、分类列表等具有层级关系的数据。

>## 创建树形控件

树形控件通过 `TreeView` 函数创建，其参数如下：

```cpp
ComponentInfo TreeView(
    HWND hwnd,          // 主界面句柄
    int id,             // 控件唯一编号
    int x,              // 树形控件的横坐标
    int y,              // 树形控件的纵坐标
    int width,          // 树形控件的宽度
    int height          // 树形控件的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `id`：树形控件的唯一编号，用于区分不同的树形控件。
- `x` 和 `y`：树形控件在主界面中的位置，以像素为单位。
- `width` 和 `height`：树形控件的宽度和高度，以像素为单位。

### TreeView返回值

返回一个 `ComponentInfo` 类型的结构体，包含树形控件的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 添加根节点

可以通过 `TreeAddRoot` 函数向树形控件添加根节点，其参数如下：

```cpp
TreeItemHandle TreeAddRoot(HWND hwnd, int id, LPCTSTR text);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `text`：节点显示的文本。

### 返回值

返回节点的句柄（`TreeItemHandle`），用于后续操作。如果失败，返回 `NULL`。

### 示例代码

```cpp
TreeItemHandle root = TreeAddRoot(hwnd, 10, L"根节点");
```

>## 添加子节点

可以通过 `TreeAddChild` 函数向指定节点添加子节点，其参数如下：

```cpp
TreeItemHandle TreeAddChild(HWND hwnd, int id, TreeItemHandle parent, LPCTSTR text);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `parent`：父节点的句柄。
- `text`：节点显示的文本。

### 返回值

返回节点的句柄（`TreeItemHandle`），用于后续操作。如果失败，返回 `NULL`。

### 示例代码

```cpp
TreeItemHandle child = TreeAddChild(hwnd, 10, root, L"子节点");
```

>## 删除节点

可以通过 `TreeDeleteItem` 函数删除指定的节点，其参数如下：

```cpp
void TreeDeleteItem(HWND hwnd, int id, TreeItemHandle item);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `item`：要删除的节点句柄。

### 示例代码

```cpp
TreeDeleteItem(hwnd, 10, child);
```

>## 清空树形控件

可以通过 `TreeClear` 函数清空树形控件中的所有节点，其参数如下：

```cpp
void TreeClear(HWND hwnd, int id);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。

### 示例代码

```cpp
TreeClear(hwnd, 10);
```

>## 获取选中节点

可以通过 `TreeGetSelection` 函数获取当前选中的节点，其参数如下：

```cpp
TreeItemHandle TreeGetSelection(HWND hwnd, int id);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。

### 返回值

返回当前选中节点的句柄。如果没有选中节点，返回 `NULL`。

### 示例代码

```cpp
TreeItemHandle selected = TreeGetSelection(hwnd, 10);
```

>## 设置节点文本

可以通过 `TreeSetItemText` 函数设置指定节点的文本，其参数如下：

```cpp
void TreeSetItemText(HWND hwnd, int id, TreeItemHandle item, LPCTSTR text);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `item`：节点句柄。
- `text`：新的文本内容。

### 示例代码

```cpp
TreeSetItemText(hwnd, 10, root, L"新的根节点名称");
```

>## 获取节点文本

可以通过 `TreeGetItemText` 函数获取指定节点的文本，其参数如下：

```cpp
LPCTSTR TreeGetItemText(HWND hwnd, int id, TreeItemHandle item);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `item`：节点句柄。

### 返回值

返回节点的文本。如果失败，返回 `NULL`。

### 示例代码

```cpp
LPCTSTR text = TreeGetItemText(hwnd, 10, root);
if (text != NULL)
{
    MessageBox(hwnd, text, L"节点文本", MB_OK);
    FreeString(text);
}
```

>## 展开节点

可以通过 `TreeExpandItem` 函数展开指定节点，显示其子节点，其参数如下：

```cpp
void TreeExpandItem(HWND hwnd, int id, TreeItemHandle item);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `item`：节点句柄。

### 示例代码

```cpp
TreeExpandItem(hwnd, 10, root);
```

>## 折叠节点

可以通过 `TreeCollapseItem` 函数折叠指定节点，隐藏其子节点，其参数如下：

```cpp
void TreeCollapseItem(HWND hwnd, int id, TreeItemHandle item);
```

### 参数说明

- `hwnd`：主界面句柄。
- `id`：树形控件的唯一编号。
- `item`：节点句柄。

### 示例代码

```cpp
TreeCollapseItem(hwnd, 10, root);
```

>## 综合示例

以下代码展示了如何创建和使用树形控件：

```cpp
#include "CVUIL.h"

TreeItemHandle rootNode;
TreeItemHandle folder1;
TreeItemHandle folder2;

void CreateMainInterface(LPCTSTR* name, int* width, int* height)
{
    *name = L"树形控件示例";
    *width = 400;
    *height = 400;
}

void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 创建树形控件
    TreeView(hwnd, 1, 10, 10, 370, 300);
    
    // 添加根节点
    rootNode = TreeAddRoot(hwnd, 1, L"我的电脑");
    
    // 添加子节点
    folder1 = TreeAddChild(hwnd, 1, rootNode, L"C盘");
    folder2 = TreeAddChild(hwnd, 1, rootNode, L"D盘");
    
    // 添加孙节点
    TreeAddChild(hwnd, 1, folder1, L"Program Files");
    TreeAddChild(hwnd, 1, folder1, L"Windows");
    TreeAddChild(hwnd, 1, folder2, L"游戏");
    TreeAddChild(hwnd, 1, folder2, L"文档");
    
    // 展开根节点
    TreeExpandItem(hwnd, 1, rootNode);
    
    // 创建控制按钮
    Button(hwnd, L"获取选中", 2, 10, 320, 100, 30);
    Button(hwnd, L"展开全部", 3, 120, 320, 100, 30);
    Button(hwnd, L"折叠全部", 4, 230, 320, 100, 30);
}

void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    switch (id)
    {
        case 2: // 获取选中
        {
            TreeItemHandle selected = TreeGetSelection(hwnd, 1);
            if (selected != NULL)
            {
                LPCTSTR text = TreeGetItemText(hwnd, 1, selected);
                if (text != NULL)
                {
                    MessageBox(hwnd, text, L"选中的节点", MB_OK);
                    FreeString(text);
                }
            }
            else
            {
                MessageBox(hwnd, L"没有选中任何节点", L"提示", MB_OK);
            }
            break;
        }
        case 3: // 展开全部
            TreeExpandItem(hwnd, 1, rootNode);
            TreeExpandItem(hwnd, 1, folder1);
            TreeExpandItem(hwnd, 1, folder2);
            break;
        case 4: // 折叠全部
            TreeCollapseItem(hwnd, 1, rootNode);
            break;
    }
}
```

>## 注意事项

- 树形控件的节点句柄（`TreeItemHandle`）需要妥善保存，以便后续操作。
- `TreeGetItemText` 返回的字符串需要使用 `FreeString` 函数释放内存。
- 在创建多个树形控件时，确保每个树形控件的编号是唯一的。
- 节点的展开/折叠状态不会影响其子节点的展开/折叠状态。

>## 相关链接

#### 导航
- [主文档](CVUIL_Instruction_manual.md)：返回主文档首页
- [主要函数](MainFunctions.md)：查看 `CreateMainInterface`、`InitLogic` 和 `Logic` 的使用方法
- [其他功能](OtherFunctions.md)：查看其他功能的使用方法

---
