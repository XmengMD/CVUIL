#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _UNICODE 1
#define UNICODE 1
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <winbase.h>
#include <WinUser.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <uxtheme.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")

/*
CVUIL.h (ConVenient UI Library)
这是一个自定义的WINAPI编程库，用于简化WINAPI的调用，使WINAPI的调用更加简单。
版本: release 1.0.0
*/

#ifndef CVUIL_H

#define CVUIL_H

extern HWND mainHwnd;
extern LPCTSTR mainUIName[512];
extern int mainUIwidth;
extern int mainUIheight;

#ifdef UNICODE
#define MainWindow WINAPI wWinMain
#else
#define MainWindow WINAPI WinMain
#endif

#define INF 0x7fffffff
#define CB_EDITABLE 1
#define CB_UNEDITED 0
#define CB_SELECTED 1
#define CB_UNSELECTED 0
#define S_VERTICAL 1
#define S_HORIZONTAL 0

// 错误码定义
#define CVUIL_OK 0
#define CVUIL_ERROR_INVALID_HANDLE -1
#define CVUIL_ERROR_INVALID_ID -2
#define CVUIL_ERROR_NOT_FOUND -3
#define CVUIL_ERROR_OUT_OF_MEMORY -4
#define CVUIL_ERROR_CANCELLED -5

typedef struct ComponentInfo
{
	HWND hwnd;
	HMENU hMenu;
	int id;
	LPCTSTR text;
}ComponentInfo, * PComponentInfo;

typedef struct DialogInfo
{
	HWND hwnd;
	int id;
	LPCTSTR title;
}DialogInfo, * PDialogInfo;

typedef HTREEITEM TreeItemHandle;

// 异步任务回调函数类型定义 (Alpha4新增)
typedef void (*AsyncTaskFunc)(void* param);
typedef void (*AsyncCallbackFunc)(void* result);
typedef void (*AsyncProgressFunc)(int progress);
typedef void (*AsyncTaskWithProgressFunc)(void* param, void (*reportProgress)(int));

// ==================== 基础控件创建函数 ====================

ComponentInfo Button(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);
ComponentInfo CheckBox(HWND hwnd, LPCTSTR text, int defaultStatus, int id, int x, int y, int width, int height);
int CheckBoxState(HWND hwnd, int id);
ComponentInfo RadioButton(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);
void RadioButtonGroup(HWND hwnd, int firstRadioButton, int defaultRadioButton);
int RadioButtonState(HWND hwnd, int id);
ComponentInfo GroupBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);
ComponentInfo ComboBox(HWND hwnd, int editable, int id, int x, int y, int width, int height);
void ComboBoxOption(HWND hwnd, int id, LPCTSTR text);
void ComboBoxDefaultOption(HWND hwnd, int id, int index);
int ComboBoxOptionState(HWND hwnd, int id);
ComponentInfo EditBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);
LPCTSTR GetEditBoxText(HWND hwnd, int id);
ComponentInfo MultiEditBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);
LPCTSTR GetMultiEditBoxText(HWND hwnd, int id);
ComponentInfo TextBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height);

// ==================== 表格控件函数 ====================

ComponentInfo Table(HWND hwnd, int id, int x, int y, int width, int height);
void TableColumn(HWND hwnd, int id, LPCTSTR text, int col, int width);
void TableRow(HWND hwnd, int id);
void TableItem(HWND hwnd, int id, LPCTSTR text, int row, int col);
void TableDeleteRow(HWND hwnd, int id, int row);
void TableClear(HWND hwnd, int id);
int TableGetSelectedRow(HWND hwnd, int id);
int TableGetRowCount(HWND hwnd, int id);
int TableGetColumnCount(HWND hwnd, int id);

// ==================== 菜单栏函数 ====================

ComponentInfo MenuBar();
ComponentInfo MenuItem(HMENU hmenu, LPCTSTR text);
ComponentInfo SubMenuItem(HMENU hmenu, LPCTSTR text, int id);
void SetMenuBar(HWND hwnd, HMENU hmenu);

// ==================== 滑块控件函数 ====================

ComponentInfo Slider(HWND hwnd, int direction, int min, int max, int defaultValue, int id, int x, int y, int width, int height);
void SliderTick(HWND hwnd, int id, int density);
void SetSlider(HWND hwnd, int id, int value);
int SliderValue(HWND hwnd, int id);
int SliderGetMin(HWND hwnd, int id);
int SliderGetMax(HWND hwnd, int id);

// ==================== 列表控件函数 ====================

ComponentInfo List(HWND hwnd, int id, int x, int y, int width, int height);
void ListItem(HWND hwnd, int id, LPCTSTR text);
void ListDeleteItem(HWND hwnd, int id, int index);
void ListClear(HWND hwnd, int id);
int ListItemState(HWND hwnd, int id);
LPCTSTR ListGetSelectedText(HWND hwnd, int id);
int ListGetItemCount(HWND hwnd, int id);

// ==================== 工具提示函数 ====================

ComponentInfo ToolTips(HWND hwnd, int id, LPCTSTR text);

// ==================== 控件状态管理函数 ====================

void EnableComponent(HWND hwnd, int id, BOOL enable);
void ShowComponent(HWND hwnd, int id, BOOL show);
void MoveComponent(HWND hwnd, int id, int x, int y, int width, int height);
void SetComponentFocus(HWND hwnd, int id);

// ==================== 输入框增强函数 ====================

ComponentInfo PasswordBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height, TCHAR passwordChar);
void SetEditBoxPasswordChar(HWND hwnd, int id, TCHAR ch);
void SetEditBoxNumberOnly(HWND hwnd, int id, BOOL numberOnly);
void SetEditBoxMaxLength(HWND hwnd, int id, UINT maxLength);
void SetEditBoxSelection(HWND hwnd, int id, DWORD start, DWORD end);

// ==================== 文件对话框函数 ====================

LPCTSTR OpenFileDialog(HWND hwnd, LPCTSTR filter);
LPCTSTR SaveFileDialog(HWND hwnd, LPCTSTR filter, LPCTSTR defaultFileName);
LPCTSTR BrowseFolderDialog(HWND hwnd, LPCTSTR title);

// ==================== 进度条控件函数 ====================

ComponentInfo ProgressBar(HWND hwnd, int id, int x, int y, int width, int height);
void SetProgressBarRange(HWND hwnd, int id, int min, int max);
void SetProgressBarValue(HWND hwnd, int id, int value);
int GetProgressBarValue(HWND hwnd, int id);
void SetProgressBarStep(HWND hwnd, int id, int step);
void ProgressBarStep(HWND hwnd, int id);

// ==================== 树形控件函数 ====================

ComponentInfo TreeView(HWND hwnd, int id, int x, int y, int width, int height);
TreeItemHandle TreeAddRoot(HWND hwnd, int id, LPCTSTR text);
TreeItemHandle TreeAddChild(HWND hwnd, int id, TreeItemHandle parent, LPCTSTR text);
void TreeDeleteItem(HWND hwnd, int id, TreeItemHandle item);
void TreeClear(HWND hwnd, int id);
TreeItemHandle TreeGetSelection(HWND hwnd, int id);
void TreeSetItemText(HWND hwnd, int id, TreeItemHandle item, LPCTSTR text);
LPCTSTR TreeGetItemText(HWND hwnd, int id, TreeItemHandle item);
void TreeExpandItem(HWND hwnd, int id, TreeItemHandle item);
void TreeCollapseItem(HWND hwnd, int id, TreeItemHandle item);

// ==================== 对话框支持函数 ====================

void ShowModalDialog(HWND parentHwnd, LPCTSTR title, int width, int height, 
                     void (*dialogInitFunc)(HWND), void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int));
HWND ShowModelessDialog(HWND parentHwnd, LPCTSTR title, int width, int height,
                        void (*dialogInitFunc)(HWND), void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int));
void CloseDialog(HWND dialogHwnd);

// ==================== 异步任务支持函数 (Alpha4新增) ====================

//启动异步任务（在后台线程执行，不阻塞UI）
//taskFunc - 后台执行的任务函数
//callbackFunc - 任务完成后在主线程回调（可安全更新UI）
//param - 传递给任务的参数（可选，可为NULL）
void RunAsync(AsyncTaskFunc taskFunc, AsyncCallbackFunc callbackFunc, void* param);

//启动带进度报告的异步任务
//taskFunc - 任务函数，可通过reportProgress回调报告进度（0-100）
//progressFunc - 进度更新回调（在主线程执行，可安全更新UI）
//callbackFunc - 完成回调（在主线程执行）
//param - 传递给任务的参数（可选，可为NULL）
void RunAsyncWithProgress(AsyncTaskWithProgressFunc taskFunc, AsyncProgressFunc progressFunc, 
                          AsyncCallbackFunc callbackFunc, void* param);

//延时执行（不阻塞UI，在指定毫秒后执行回调）
//milliseconds - 延时毫秒数
//callbackFunc - 延时结束后执行的回调
//param - 传递给回调的参数（可选，可为NULL）
void Delay(int milliseconds, AsyncCallbackFunc callbackFunc, void* param);

//取消所有正在运行的异步任务（正在执行的任务会继续完成，但回调不会执行）
void CancelAllAsyncTasks();

//获取当前正在运行的异步任务数量
int GetRunningAsyncTaskCount();

//报告进度（在带进度的异步任务中调用）
void ReportProgress(int progress);

// ==================== 辅助函数 ====================

HWND GetComponentHWND(HWND hwnd, int id);
int GetComponentId(HWND hwndComponent);
void SetComponentText(HWND hwnd, int id, LPCTSTR text);
LPCTSTR GetComponentText(HWND hwnd, int id);
void FreeString(LPCTSTR str);

LPCTSTR NumToStr(short number);
LPCTSTR NumToStr(unsigned short number);
LPCTSTR NumToStr(int number);
LPCTSTR NumToStr(unsigned int number);
LPCTSTR NumToStr(long number);
LPCTSTR NumToStr(unsigned long number);
LPCTSTR NumToStr(long long number);
LPCTSTR NumToStr(unsigned long long number);
LPCTSTR NumToStr(float number);
LPCTSTR NumToStr(double number);
LPCTSTR NumToStr(long double number);

// ==================== 回调函数声明 ====================

extern void InitLogic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void Logic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int id);
extern void CreateMainInterface(LPCTSTR* name, int* width, int* height);

#endif // CVUIL_H
