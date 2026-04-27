# 滑块控件 (Slider)

滑块控件是 CVUIL 库中用于创建滑动选择器的控件。用户可以通过拖动滑块来选择一个值，通常用于调整数值或控制进度。

>## 创建滑块

滑块控件通过 `Slider` 函数创建，其参数如下：

```cpp
ComponentInfo Slider(
    HWND hwnd,              // 主界面句柄
    int direction,          // 滑块方向（0 表示水平，1 表示垂直）
    int min,                // 滑块的最小值
    int max,                // 滑块的最大值
    int defaultValue,       // 滑块的初始值
    int id,                 // 控件唯一编号
    int x,                  // 滑块的横坐标
    int y,                  // 滑块的纵坐标
    int width,              // 滑块的宽度
    int height              // 滑块的高度
);
```

### 参数说明

- `hwnd`：绑定的主界面句柄。如果为 `NULL`，则默认使用全局的 `mainHwnd`。
- `direction`：滑块的方向，`0` 表示水平，`1` 表示垂直。
- `min` 和 `max`：滑块的最小值和最大值，定义了滑块的取值范围。
- `defaultValue`：滑块的初始值，必须在 `[min, max]` 范围内。
- `id`：滑块的唯一编号，用于区分不同的滑块并处理事件。
- `x` 和 `y`：滑块在主界面中的位置，以像素为单位。
- `width` 和 `height`：滑块的宽度和高度，以像素为单位。对于水平滑块，高度通常固定；对于垂直滑块，宽度通常固定。

### Slider返回值

返回一个 `ComponentInfo` 类型的结构体，包含滑块的句柄 `hwnd`、菜单句柄 `hMenu`、编号 `id` 和文本 `text`。

>## 为滑块添加刻度

可以通过 `SliderTick` 函数为滑块添加刻度，其参数如下：

```cpp
void SliderTick(
    HWND hwnd,          // 主界面句柄
    int id,             // 滑块的唯一编号
    int density         // 刻度密度（刻度间隔）
);
```

- `hwnd`：主界面句柄。
- `id`：滑块的唯一编号。
- `density`：刻度密度，表示刻度之间的间隔值。

>## 设置滑块的值

可以通过 `SetSlider` 函数设置滑块的当前值，其参数如下：

```cpp
void SetSlider(HWND hwnd, int id, int value);
```

- `hwnd`：主界面句柄。
- `id`：滑块的唯一编号。
- `value`：要设置的值。

>## 获取滑块的当前值

可以通过 `SliderValue` 函数获取滑块的当前值，其参数如下：

```cpp
int SliderValue(
    HWND hwnd,          // 主界面句柄
    int id              // 滑块的唯一编号
);
```

### SliderValue返回值

返回滑块的当前值。如果滑块句柄无效或编号无效，返回 `INF`（一个非常大的数值）。

>## 获取滑块的最小值

可以通过 `SliderGetMin` 函数获取滑块的最小值，其参数如下：

```cpp
int SliderGetMin(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：滑块的唯一编号。

### SliderGetMin返回值

返回滑块的最小值。如果控件编号无效，返回 `0`。

>## 获取滑块的最大值

可以通过 `SliderGetMax` 函数获取滑块的最大值，其参数如下：

```cpp
int SliderGetMax(HWND hwnd, int id);
```

- `hwnd`：主界面句柄。
- `id`：滑块的唯一编号。

### SliderGetMax返回值

返回滑块的最大值。如果控件编号无效，返回 `0`。

>## 示例代码

### 创建一个水平滑块并设置初始值和刻度

以下代码展示了如何创建一个水平滑块并为其设置初始值和刻度：

```cpp
ComponentInfo slider1 = Slider(mainHwnd, 0, 0, 100, 50, 17, 10, 10, 300, 30); // 创建水平滑块
SliderTick(mainHwnd, 17, 10); // 设置刻度密度为 10
```

- `mainHwnd` 是主界面的句柄。
- 滑块的方向为水平（`0`）。
- 滑块的范围为 `[0, 100]`，初始值为 `50`。
- 滑块的编号为 `17`。
- 滑块的位置为 `(10, 10)`，宽度为 `300`，高度为 `30`。
- 刻度密度为 `10`，表示每隔 `10` 的位置有一个刻度。

### 获取滑块的当前值并处理

可以通过 `SliderValue` 函数获取滑块的当前值，并在程序中使用。以下是一个简单的示例：

```cpp
void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id)
{
    if (id == 17) // 滑块值变化事件
    {
        int currentValue = SliderValue(hwnd, 17); // 获取编号为17的滑块的当前值
        // 处理滑块值变化
    }
}
```

- 当滑块值变化时，`Logic` 函数会被调用。
- 调用 `SliderValue` 函数获取滑块的当前值。

>## 注意事项

- 用户拖动滑块时，会实时触发Logic事件。
- 滑块的方向、范围和初始值必须在创建时指定。
- 刻度密度必须为正整数，表示刻度之间的间隔值。
- 在创建多个滑块时，确保每个滑块的编号是唯一的。
- 滑块的宽度和高度应根据方向进行调整。对于水平滑块，高度通常固定；对于垂直滑块，宽度通常固定。

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
- [表格控件](Table.md) ← 上一篇
- [菜单栏控件](MenuBar.md) → 下一篇
- [工具提示控件](ToolTips.md)
- [进度条控件](ProgressBar.md)
- [树形控件](TreeView.md)

---
