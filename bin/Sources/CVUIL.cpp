#include "CVUIL.h"
#include <commdlg.h>
#include <shlobj.h>
#include <process.h>

#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

/*
CVUIL.cpp (ConVenient UI Library)
这是一个自定义的WINAPI编程库，用于简化WINAPI的调用，使WINAPI的调用更加简单。
版本: release 1.0.0
*/

HWND mainHwnd;
LPCTSTR mainUIName[512];
int mainUIwidth;
int mainUIheight;

// 前向声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 对话框回调函数指针
typedef void (*DialogInitFunc)(HWND);
typedef void (*DialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int);
DialogInitFunc g_currentDialogInit = NULL;
DialogLogicFunc g_currentDialogLogic = NULL;

// ==================== 异步任务支持结构 (Alpha4新增) ====================

#define WM_ASYNC_CALLBACK (WM_USER + 100)
#define WM_ASYNC_PROGRESS (WM_USER + 101)
#define WM_DELAY_CALLBACK (WM_USER + 102)
#define MAX_ASYNC_TASKS 64

typedef struct AsyncTask
{
	int id;
	BOOL active;
	BOOL cancelled;
	HANDLE thread;
	AsyncTaskFunc taskFunc;
	AsyncCallbackFunc callbackFunc;
	void* param;
	void* result;
	AsyncProgressFunc progressFunc;
	AsyncTaskWithProgressFunc taskWithProgressFunc;
} AsyncTask;

typedef struct DelayTask
{
	int id;
	BOOL active;
	HANDLE thread;
	int milliseconds;
	AsyncCallbackFunc callbackFunc;
	void* param;
} DelayTask;

static AsyncTask g_asyncTasks[MAX_ASYNC_TASKS];
static DelayTask g_delayTasks[MAX_ASYNC_TASKS];
static int g_asyncTaskCount = 0;
static int g_delayTaskCount = 0;
static CRITICAL_SECTION g_asyncCS;
static BOOL g_asyncInitialized = FALSE;

// 初始化异步任务系统
void InitAsyncSystem()
{
	if (!g_asyncInitialized)
	{
		InitializeCriticalSection(&g_asyncCS);
		ZeroMemory(g_asyncTasks, sizeof(g_asyncTasks));
		ZeroMemory(g_delayTasks, sizeof(g_delayTasks));
		g_asyncInitialized = TRUE;
	}
}

// 清理异步任务系统
void CleanupAsyncSystem()
{
	if (g_asyncInitialized)
	{
		CancelAllAsyncTasks();
		DeleteCriticalSection(&g_asyncCS);
		g_asyncInitialized = FALSE;
	}
}

// 获取可用的异步任务槽
int GetAvailableAsyncTaskSlot()
{
	for (int i = 0; i < MAX_ASYNC_TASKS; i++)
	{
		if (!g_asyncTasks[i].active)
			return i;
	}
	return -1;
}

// 获取可用的延时任务槽
int GetAvailableDelayTaskSlot()
{
	for (int i = 0; i < MAX_ASYNC_TASKS; i++)
	{
		if (!g_delayTasks[i].active)
			return i;
	}
	return -1;
}

// 报告进度（从工作线程调用）
void ReportProgressInternal(int taskId, int progress)
{
	if (mainHwnd != NULL && IsWindow(mainHwnd))
	{
		PostMessage(mainHwnd, WM_ASYNC_PROGRESS, (WPARAM)taskId, (LPARAM)progress);
	}
}

// 异步任务线程函数（普通任务）
unsigned __stdcall AsyncTaskThread(void* param)
{
	int taskId = (int)(intptr_t)param;
	if (taskId < 0 || taskId >= MAX_ASYNC_TASKS) return 0;
	
	AsyncTask* task = &g_asyncTasks[taskId];
	if (!task->active || task->cancelled) return 0;
	
	// 执行任务
	if (task->taskFunc != NULL)
	{
		task->taskFunc(task->param);
	}
	
	// 发送完成消息到主线程
	if (!task->cancelled && mainHwnd != NULL && IsWindow(mainHwnd))
	{
		PostMessage(mainHwnd, WM_ASYNC_CALLBACK, (WPARAM)taskId, 0);
	}
	
	return 0;
}

// 当前正在执行的带进度任务的ID（用于ReportProgress函数）
static int g_currentProgressTaskId = -1;

// 全局进度报告函数（供用户任务调用）
void ReportProgress(int progress)
{
	if (g_currentProgressTaskId >= 0)
	{
		ReportProgressInternal(g_currentProgressTaskId, progress);
	}
}

// 异步任务线程函数（带进度）
unsigned __stdcall AsyncTaskWithProgressThread(void* param)
{
	int taskId = (int)(intptr_t)param;
	if (taskId < 0 || taskId >= MAX_ASYNC_TASKS) return 0;
	
	AsyncTask* task = &g_asyncTasks[taskId];
	if (!task->active || task->cancelled) return 0;
	
	// 设置当前任务ID（用于ReportProgress）
	g_currentProgressTaskId = taskId;
	
	// 执行任务 - 用户通过调用ReportProgress来报告进度
	if (task->taskWithProgressFunc != NULL)
	{
		task->taskWithProgressFunc(task->param, ReportProgress);
	}
	
	// 清除当前任务ID
	if (g_currentProgressTaskId == taskId)
	{
		g_currentProgressTaskId = -1;
	}
	
	// 发送完成消息到主线程
	if (!task->cancelled && mainHwnd != NULL && IsWindow(mainHwnd))
	{
		PostMessage(mainHwnd, WM_ASYNC_CALLBACK, (WPARAM)taskId, 0);
	}
	
	return 0;
}

// 延时任务线程函数
unsigned __stdcall DelayTaskThread(void* param)
{
	int taskId = (int)(intptr_t)param;
	if (taskId < 0 || taskId >= MAX_ASYNC_TASKS) return 0;
	
	DelayTask* task = &g_delayTasks[taskId];
	if (!task->active) return 0;
	
	// 延时
	Sleep(task->milliseconds);
	
	// 发送完成消息到主线程
	if (task->active && mainHwnd != NULL && IsWindow(mainHwnd))
	{
		PostMessage(mainHwnd, WM_DELAY_CALLBACK, (WPARAM)taskId, 0);
	}
	
	return 0;
}

// ==================== 基础控件创建函数 ====================

ComponentInfo Button(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"BUTTON", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_VCENTER | BS_FLAT,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

ComponentInfo CheckBox(HWND hwnd, LPCTSTR text, int defaultStatus, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"BUTTON", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_CENTER | BS_VCENTER,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	if (defaultStatus == 1) SendMessage(info.hwnd, BM_SETCHECK, BST_CHECKED, 0);
	else SendMessage(info.hwnd, BM_SETCHECK, BST_UNCHECKED, 0);
	return info;
}

int CheckBoxState(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return -1;
	HWND checkBoxHwnd = GetDlgItem(hwnd, id);
	if (checkBoxHwnd != NULL) return static_cast<int>(SendMessage(checkBoxHwnd, BM_GETCHECK, 0, 0));
	return -1;
}

ComponentInfo RadioButton(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"BUTTON", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_CENTER | BS_VCENTER,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

void RadioButtonGroup(HWND hwnd, int firstRadioButton, int defaultRadioButton)
{
	if (firstRadioButton <= 0 || defaultRadioButton <= 0) return;
	HWND firstHwnd = GetDlgItem(hwnd, firstRadioButton);
	HWND defaultHwnd = GetDlgItem(hwnd, defaultRadioButton);
	LONG_PTR style = GetWindowLongPtr(firstHwnd, GWL_STYLE);
	style |= WS_GROUP;
	SetWindowLongPtr(firstHwnd, GWL_STYLE, style);
	SendMessage(defaultHwnd, BM_SETCHECK, BST_CHECKED, 0);
}

int RadioButtonState(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return -1;
	HWND radioButtonHwnd = GetDlgItem(hwnd, id);
	if (radioButtonHwnd != NULL) return static_cast<int>(SendMessage(radioButtonHwnd, BM_GETCHECK, 0, 0));
	return -1;
}

ComponentInfo GroupBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"BUTTON", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_GROUPBOX | BS_CENTER | BS_VCENTER,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

ComponentInfo ComboBox(HWND hwnd, int editable, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL;
	if (editable) style |= CBS_DROPDOWN;
	info.hwnd = CreateWindowEx(0, L"COMBOBOX", NULL, style,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

void ComboBoxOption(HWND hwnd, int id, LPCTSTR text)
{
	if (hwnd == NULL || id <= 0 || text == NULL) return;
	HWND comboBoxHwnd = GetDlgItem(hwnd, id);
	if (comboBoxHwnd != NULL) SendMessage(comboBoxHwnd, CB_ADDSTRING, 0, (LPARAM)text);
}

void ComboBoxDefaultOption(HWND hwnd, int id, int index)
{
	if (hwnd == NULL || id <= 0) return;
	HWND comboBoxHwnd = GetDlgItem(hwnd, id);
	if (comboBoxHwnd != NULL) SendMessage(comboBoxHwnd, CB_SETCURSEL, index, 0);
}

int ComboBoxOptionState(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return -1;
	HWND hwndComboBox = GetDlgItem(hwnd, id);
	if (hwndComboBox != NULL) return static_cast<int>(SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0));
	return -1;
}

ComponentInfo EditBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"EDIT", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

LPCTSTR GetEditBoxText(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return NULL;
	HWND editBoxHwnd = GetDlgItem(hwnd, id);
	if (editBoxHwnd != NULL)
	{
		int length = GetWindowTextLength(editBoxHwnd);
		if (length > 0)
		{
			LPWSTR text = new WCHAR[length + 1];
			GetWindowText(editBoxHwnd, text, length + 1);
			return text;
		}
	}
	return NULL;
}

ComponentInfo MultiEditBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"EDIT", text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

LPCTSTR GetMultiEditBoxText(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return NULL;
	HWND multiEditBoxHwnd = GetDlgItem(hwnd, id);
	if (multiEditBoxHwnd != NULL)
	{
		int length = GetWindowTextLength(multiEditBoxHwnd);
		if (length > 0)
		{
			LPWSTR text = new WCHAR[length + 1];
			GetWindowText(multiEditBoxHwnd, text, length + 1);
			return text;
		}
	}
	return NULL;
}

ComponentInfo TextBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"STATIC", text,
		WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

// ==================== 表格控件函数 ====================

ComponentInfo Table(HWND hwnd, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, WC_LISTVIEW, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EX_GRIDLINES,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	ListView_SetExtendedListViewStyle(info.hwnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

void TableColumn(HWND hwnd, int id, LPCTSTR text, int col, int width)
{
	if (hwnd == NULL || id <= 0 || text == NULL || col < 0 || width <= 0) return;
	HWND hwndListView = GetDlgItem(hwnd, id);
	LVCOLUMN lvColumn = {};
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.pszText = (LPWSTR)text;
	lvColumn.cx = width;
	ListView_InsertColumn(hwndListView, col, &lvColumn);
}

void TableRow(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hwndListView = GetDlgItem(hwnd, id);
	LVITEM lvItem = {};
	lvItem.mask = LVIF_TEXT;
	lvItem.pszText = NULL;
	lvItem.iItem = ListView_GetItemCount(hwndListView);
	ListView_InsertItem(hwndListView, &lvItem);
}

void TableItem(HWND hwnd, int id, LPCTSTR text, int row, int col)
{
	if (hwnd == NULL || id <= 0 || row < 0 || col < 0) return;
	HWND hwndListView = GetDlgItem(hwnd, id);
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = row;
	item.iSubItem = col;
	item.pszText = (LPWSTR)text;
	ListView_SetItem(hwndListView, &item);
}

void TableDeleteRow(HWND hwnd, int id, int row)
{
	if (hwnd == NULL || id <= 0 || row < 0) return;
	HWND hwndListView = GetDlgItem(hwnd, id);
	if (hwndListView != NULL) ListView_DeleteItem(hwndListView, row);
}

void TableClear(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hwndListView = GetDlgItem(hwnd, id);
	if (hwndListView != NULL) ListView_DeleteAllItems(hwndListView);
}

int TableGetSelectedRow(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return -1;
	HWND hwndListView = GetDlgItem(hwnd, id);
	if (hwndListView != NULL) return ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);
	return -1;
}

int TableGetRowCount(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND hwndListView = GetDlgItem(hwnd, id);
	if (hwndListView != NULL) return ListView_GetItemCount(hwndListView);
	return 0;
}

int TableGetColumnCount(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND hwndListView = GetDlgItem(hwnd, id);
	if (hwndListView != NULL)
	{
		HWND hHeader = ListView_GetHeader(hwndListView);
		if (hHeader != NULL) return Header_GetItemCount(hHeader);
	}
	return 0;
}

// ==================== 菜单栏函数 ====================

ComponentInfo MenuBar()
{
	ComponentInfo info;
	info.id = -1;
	info.text = NULL;
	info.hMenu = CreateMenu();
	info.hwnd = NULL;
	return info;
}

ComponentInfo MenuItem(HMENU hmenu, LPCTSTR text)
{
	ComponentInfo info;
	info.id = -1;
	info.text = text;
	info.hMenu = CreatePopupMenu();
	info.hwnd = NULL;
	AppendMenu(hmenu, MF_POPUP, (UINT_PTR)info.hMenu, text);
	return info;
}

ComponentInfo SubMenuItem(HMENU hmenu, LPCTSTR text, int id)
{
	ComponentInfo info;
	info.id = id;
	info.text = text;
	info.hMenu = CreatePopupMenu();
	info.hwnd = NULL;
	AppendMenu(hmenu, MF_STRING, id, text);
	return info;
}

void SetMenuBar(HWND hwnd, HMENU hmenu)
{
	SetMenu(hwnd, hmenu);
}

// ==================== 滑块控件函数 ====================

ComponentInfo Slider(HWND hwnd, int direction, int minValue, int maxValue, int defaultValue, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | TBS_NOTICKS | (direction == 0 ? TBS_HORZ : TBS_VERT),
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	if (direction == 0)
	{
		SendMessage(info.hwnd, TBM_SETRANGE, TRUE, MAKELONG(minValue, maxValue));
		SendMessage(info.hwnd, TBM_SETPOS, TRUE, defaultValue);
	}
	else
	{
		SendMessage(info.hwnd, TBM_SETRANGE, TRUE, MAKELONG(-maxValue, -minValue));
		SendMessage(info.hwnd, TBM_SETPOS, TRUE, -defaultValue);
	}
	return info;
}

void SliderTick(HWND hwnd, int id, int density)
{
	if (hwnd == NULL || id <= 0 || density <= 0) return;
	HWND sliderHwnd = GetDlgItem(hwnd, id);
	if (sliderHwnd != NULL)
	{
		LONG style = GetWindowLong(sliderHwnd, GWL_STYLE);
		style &= ~TBS_NOTICKS;
		style |= TBS_AUTOTICKS;
		SetWindowLong(sliderHwnd, GWL_STYLE, style);
		SendMessage(sliderHwnd, TBM_SETTICFREQ, density, 0);
	}
}

void SetSlider(HWND hwnd, int id, int value)
{
	HWND sliderHwnd = GetDlgItem(hwnd, id);
	if (sliderHwnd == NULL) return;
	DWORD style = GetWindowLong(sliderHwnd, GWL_STYLE);
	if (style & TBS_VERT) value = -value;
	SendMessage(sliderHwnd, TBM_SETPOS, TRUE, value);
}

int SliderValue(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return INF;
	HWND sliderHwnd = GetDlgItem(hwnd, id);
	if (sliderHwnd != NULL)
	{
		DWORD style = GetWindowLong(sliderHwnd, GWL_STYLE);
		if (style & TBS_VERT) return -static_cast<int>(SendMessage(sliderHwnd, TBM_GETPOS, 0, 0));
		else return static_cast<int>(SendMessage(sliderHwnd, TBM_GETPOS, 0, 0));
	}
	return INF;
}

int SliderGetMin(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND sliderHwnd = GetDlgItem(hwnd, id);
	if (sliderHwnd != NULL)
	{
		DWORD style = GetWindowLong(sliderHwnd, GWL_STYLE);
		int min = static_cast<int>(SendMessage(sliderHwnd, TBM_GETRANGEMIN, 0, 0));
		if (style & TBS_VERT) min = -min;
		return min;
	}
	return 0;
}

int SliderGetMax(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND sliderHwnd = GetDlgItem(hwnd, id);
	if (sliderHwnd != NULL)
	{
		DWORD style = GetWindowLong(sliderHwnd, GWL_STYLE);
		int max = static_cast<int>(SendMessage(sliderHwnd, TBM_GETRANGEMAX, 0, 0));
		if (style & TBS_VERT) max = -max;
		return max;
	}
	return 0;
}

// ==================== 列表控件函数 ====================

ComponentInfo List(HWND hwnd, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, L"LISTBOX", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | LBS_STANDARD,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

void ListItem(HWND hwnd, int id, LPCTSTR text)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL) SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)text);
}

void ListDeleteItem(HWND hwnd, int id, int index)
{
	if (hwnd == NULL || id <= 0 || index < 0) return;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL) SendMessage(hwndListBox, LB_DELETESTRING, index, 0);
}

void ListClear(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL) SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
}

int ListItemState(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return -1;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL) return static_cast<int>(SendMessage(hwndListBox, LB_GETCURSEL, 0, 0));
	return -1;
}

LPCTSTR ListGetSelectedText(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return NULL;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL)
	{
		int index = static_cast<int>(SendMessage(hwndListBox, LB_GETCURSEL, 0, 0));
		if (index >= 0)
		{
			int length = static_cast<int>(SendMessage(hwndListBox, LB_GETTEXTLEN, index, 0));
			if (length > 0)
			{
				LPWSTR text = new WCHAR[length + 1];
				SendMessage(hwndListBox, LB_GETTEXT, index, (LPARAM)text);
				return text;
			}
		}
	}
	return NULL;
}

int ListGetItemCount(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND hwndListBox = GetDlgItem(hwnd, id);
	if (hwndListBox != NULL) return static_cast<int>(SendMessage(hwndListBox, LB_GETCOUNT, 0, 0));
	return 0;
}

// ==================== 工具提示函数 ====================

ComponentInfo ToolTips(HWND hwnd, int id, LPCTSTR text)
{
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	if (hwnd == NULL || id <= 0)
	{
		info.hwnd = NULL;
		return info;
	}
	HWND hwndTool = GetDlgItem(hwnd, id);
	HWND hwndTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd, NULL, NULL, NULL);
	size_t len = wcslen(text) + 1;
	wchar_t* modifiableStr = new wchar_t[len];
	wcscpy(modifiableStr, text);
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hwnd;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = modifiableStr;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(info.hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	return info;
}

// ==================== 控件状态管理函数 ====================

void EnableComponent(HWND hwnd, int id, BOOL enable)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hControl = GetDlgItem(hwnd, id);
	if (hControl != NULL) EnableWindow(hControl, enable);
}

void ShowComponent(HWND hwnd, int id, BOOL show)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hControl = GetDlgItem(hwnd, id);
	if (hControl != NULL) ShowWindow(hControl, show ? SW_SHOW : SW_HIDE);
}

void MoveComponent(HWND hwnd, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hControl = GetDlgItem(hwnd, id);
	if (hControl != NULL) SetWindowPos(hControl, NULL, x, y, width, height, SWP_NOZORDER);
}

void SetComponentFocus(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hControl = GetDlgItem(hwnd, id);
	if (hControl != NULL) SetFocus(hControl);
}

// ==================== 输入框增强函数 ====================

ComponentInfo PasswordBox(HWND hwnd, LPCTSTR text, int id, int x, int y, int width, int height, TCHAR passwordChar)
{
	ComponentInfo info = EditBox(hwnd, text, id, x, y, width, height);
	if (info.hwnd != NULL && passwordChar != 0)
		SendMessage(info.hwnd, EM_SETPASSWORDCHAR, passwordChar, 0);
	return info;
}

void SetEditBoxPasswordChar(HWND hwnd, int id, TCHAR ch)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hEdit = GetDlgItem(hwnd, id);
	if (hEdit != NULL)
	{
		SendMessage(hEdit, EM_SETPASSWORDCHAR, ch, 0);
		InvalidateRect(hEdit, NULL, TRUE);
	}
}

void SetEditBoxNumberOnly(HWND hwnd, int id, BOOL numberOnly)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hEdit = GetDlgItem(hwnd, id);
	if (hEdit != NULL)
	{
		LONG_PTR style = GetWindowLongPtr(hEdit, GWL_STYLE);
		if (numberOnly) style |= ES_NUMBER;
		else style &= ~ES_NUMBER;
		SetWindowLongPtr(hEdit, GWL_STYLE, style);
		SetWindowPos(hEdit, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void SetEditBoxMaxLength(HWND hwnd, int id, UINT maxLength)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hEdit = GetDlgItem(hwnd, id);
	if (hEdit != NULL) SendMessage(hEdit, EM_LIMITTEXT, maxLength, 0);
}

void SetEditBoxSelection(HWND hwnd, int id, DWORD start, DWORD end)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hEdit = GetDlgItem(hwnd, id);
	if (hEdit != NULL) SendMessage(hEdit, EM_SETSEL, start, end);
}

// ==================== 文件对话框函数 ====================

LPCTSTR OpenFileDialog(HWND hwnd, LPCTSTR filter)
{
	OPENFILENAME ofn;
	wchar_t szFile[MAX_PATH * 64] = { 0 };
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	
	if (GetOpenFileName(&ofn))
	{
		// 检查是否选择了多个文件
		// 多选时格式: 目录\0文件1\0文件2\0...\0\0
		// 单选时格式: 完整路径\0\0
		int len = (int)wcslen(szFile);
		
		// 检查是否存在第二个字符串（多选情况）
		if (len + 1 < (int)ofn.nMaxFile && szFile[len + 1] != L'\0')
		{
			// 多文件选择
			wchar_t* dir = szFile;
			wchar_t* file = szFile + len + 1;
			
			// 计算需要的总长度
			int totalLen = 0;
			wchar_t* f = file;
			while (*f != L'\0')
			{
				totalLen += wcslen(dir) + 1 + wcslen(f) + 1; // 目录 + \ + 文件名 + ;
				f += wcslen(f) + 1;
			}
			
			LPWSTR result = new WCHAR[totalLen + 1];
			result[0] = L'\0';
			
			f = file;
			bool first = true;
			while (*f != L'\0')
			{
				if (!first)
					wcscat(result, L";");
				first = false;
				
				// 添加目录
				wcscat(result, dir);
				
				// 添加分隔符（如果目录不以\结尾）
				int dirLen = (int)wcslen(dir);
				if (dirLen > 0 && dir[dirLen - 1] != L'\\' && dir[dirLen - 1] != L'/')
					wcscat(result, L"\\");
				
				// 添加文件名
				wcscat(result, f);
				
				f += wcslen(f) + 1;
			}
			return result;
		}
		else
		{
			// 单文件选择
			LPWSTR result = new WCHAR[len + 1];
			wcscpy(result, szFile);
			return result;
		}
	}
	return NULL;
}

LPCTSTR SaveFileDialog(HWND hwnd, LPCTSTR filter, LPCTSTR defaultFileName)
{
	OPENFILENAME ofn;
	wchar_t szFile[MAX_PATH] = { 0 };
	
	if (defaultFileName != NULL)
		wcscpy(szFile, defaultFileName);
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	
	if (GetSaveFileName(&ofn))
	{
		LPWSTR result = new WCHAR[wcslen(szFile) + 1];
		wcscpy(result, szFile);
		return result;
	}
	return NULL;
}

LPCTSTR BrowseFolderDialog(HWND hwnd, LPCTSTR title)
{
	BROWSEINFO bi;
	wchar_t szDisplayName[MAX_PATH];
	LPITEMIDLIST pidl;
	LPWSTR result = NULL;
	
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = hwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = title;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	
	pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		wchar_t szPath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, szPath))
		{
			result = new WCHAR[wcslen(szPath) + 1];
			wcscpy(result, szPath);
		}
		CoTaskMemFree(pidl);
	}
	return result;
}

// ==================== 进度条控件函数 ====================

ComponentInfo ProgressBar(HWND hwnd, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(0, PROGRESS_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	SendMessage(info.hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(info.hwnd, PBM_SETPOS, 0, 0);
	return info;
}

void SetProgressBarRange(HWND hwnd, int id, int min, int max)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hProgress = GetDlgItem(hwnd, id);
	if (hProgress != NULL) SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(min, max));
}

void SetProgressBarValue(HWND hwnd, int id, int value)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hProgress = GetDlgItem(hwnd, id);
	if (hProgress != NULL) SendMessage(hProgress, PBM_SETPOS, value, 0);
}

int GetProgressBarValue(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return 0;
	HWND hProgress = GetDlgItem(hwnd, id);
	if (hProgress != NULL) return (int)SendMessage(hProgress, PBM_GETPOS, 0, 0);
	return 0;
}

void SetProgressBarStep(HWND hwnd, int id, int step)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hProgress = GetDlgItem(hwnd, id);
	if (hProgress != NULL) SendMessage(hProgress, PBM_SETSTEP, step, 0);
}

void ProgressBarStep(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hProgress = GetDlgItem(hwnd, id);
	if (hProgress != NULL) SendMessage(hProgress, PBM_STEPIT, 0, 0);
}

// ==================== 树形控件函数 ====================

ComponentInfo TreeView(HWND hwnd, int id, int x, int y, int width, int height)
{
	if (hwnd == NULL) hwnd = mainHwnd;
	ComponentInfo info;
	info.id = id;
	info.text = NULL;
	info.hMenu = NULL;
	info.hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, NULL,
		WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		x, y, width, height, hwnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)), NULL, NULL);
	return info;
}

TreeItemHandle TreeAddRoot(HWND hwnd, int id, LPCTSTR text)
{
	if (hwnd == NULL || id <= 0) return NULL;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree == NULL) return NULL;
	
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT;
	tvis.item.pszText = (LPWSTR)text;
	return (TreeItemHandle)TreeView_InsertItem(hTree, &tvis);
}

TreeItemHandle TreeAddChild(HWND hwnd, int id, TreeItemHandle parent, LPCTSTR text)
{
	if (hwnd == NULL || id <= 0 || parent == NULL) return NULL;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree == NULL) return NULL;
	
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = parent;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT;
	tvis.item.pszText = (LPWSTR)text;
	return (TreeItemHandle)TreeView_InsertItem(hTree, &tvis);
}

void TreeDeleteItem(HWND hwnd, int id, TreeItemHandle item)
{
	if (hwnd == NULL || id <= 0 || item == NULL) return;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL) TreeView_DeleteItem(hTree, item);
}

void TreeClear(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL) TreeView_DeleteAllItems(hTree);
}

TreeItemHandle TreeGetSelection(HWND hwnd, int id)
{
	if (hwnd == NULL || id <= 0) return NULL;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL) return (TreeItemHandle)TreeView_GetSelection(hTree);
	return NULL;
}

void TreeSetItemText(HWND hwnd, int id, TreeItemHandle item, LPCTSTR text)
{
	if (hwnd == NULL || id <= 0 || item == NULL) return;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL)
	{
		TVITEM tvi;
		tvi.mask = TVIF_TEXT;
		tvi.hItem = item;
		tvi.pszText = (LPWSTR)text;
		TreeView_SetItem(hTree, &tvi);
	}
}

LPCTSTR TreeGetItemText(HWND hwnd, int id, TreeItemHandle item)
{
	if (hwnd == NULL || id <= 0 || item == NULL) return NULL;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL)
	{
		wchar_t buffer[256];
		TVITEM tvi;
		tvi.mask = TVIF_TEXT;
		tvi.hItem = item;
		tvi.pszText = buffer;
		tvi.cchTextMax = 256;
		if (TreeView_GetItem(hTree, &tvi))
		{
			LPWSTR result = new WCHAR[wcslen(buffer) + 1];
			wcscpy(result, buffer);
			return result;
		}
	}
	return NULL;
}

void TreeExpandItem(HWND hwnd, int id, TreeItemHandle item)
{
	if (hwnd == NULL || id <= 0 || item == NULL) return;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL) TreeView_Expand(hTree, item, TVE_EXPAND);
}

void TreeCollapseItem(HWND hwnd, int id, TreeItemHandle item)
{
	if (hwnd == NULL || id <= 0 || item == NULL) return;
	HWND hTree = GetDlgItem(hwnd, id);
	if (hTree != NULL) TreeView_Expand(hTree, item, TVE_COLLAPSE);
}

// ==================== 对话框支持函数 ====================

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);
	
	switch (uMsg)
	{
		case WM_CREATE:
			if (g_currentDialogInit != NULL)
				g_currentDialogInit(hwnd);
			return 0;
			
		case WM_COMMAND:
			if (g_currentDialogLogic != NULL)
				g_currentDialogLogic(hwnd, uMsg, wParam, lParam, id);
			if (id == IDOK || id == IDCANCEL)
			{
				// 不要直接销毁窗口，发送WM_CLOSE消息
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			return 0;
			
		case WM_CLOSE:
		{
			// 获取父窗口并重新启用它
			HWND parentHwnd = GetParent(hwnd);
			if (parentHwnd != NULL && IsWindow(parentHwnd))
			{
				EnableWindow(parentHwnd, TRUE);
				SetForegroundWindow(parentHwnd);
			}
			DestroyWindow(hwnd);
			return 0;
		}
			
		case WM_DESTROY:
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ShowModalDialog(HWND parentHwnd, LPCTSTR title, int width, int height,
                     void (*dialogInitFunc)(HWND), void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int))
{
	const wchar_t DIALOG_CLASS[] = L"CVUIL_Dialog_Class";
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = DialogProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = DIALOG_CLASS;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	RegisterClass(&wc);
	
	g_currentDialogInit = dialogInitFunc;
	g_currentDialogLogic = dialogLogicFunc;
	
	HWND hDlg = CreateWindowEx(
		WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
		DIALOG_CLASS, title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		parentHwnd, NULL, GetModuleHandle(NULL), NULL);
	
	if (hDlg == NULL) return;
	
	if (parentHwnd != NULL)
		EnableWindow(parentHwnd, FALSE);
	
	// 强制刷新禁用状态
	if (parentHwnd != NULL)
		RedrawWindow(parentHwnd, NULL, NULL, RDW_UPDATENOW);
	
	// 对话框消息循环 - 只处理对话框的消息
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// 错误处理
			break;
		}
		
		// 检查对话框是否还存在
		if (!IsWindow(hDlg))
			break;
		
		// 只处理对话框的消息
		if (!IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	// 重要：在重新启用父窗口之前，确保对话框已完全销毁
	if (IsWindow(hDlg))
	{
		DestroyWindow(hDlg);
	}
	
	// 重新启用父窗口
	if (parentHwnd != NULL && IsWindow(parentHwnd))
	{
		EnableWindow(parentHwnd, TRUE);
		// 将父窗口带到前台
		SetForegroundWindow(parentHwnd);
		// 强制刷新
		RedrawWindow(parentHwnd, NULL, NULL, RDW_UPDATENOW);
	}
	
	g_currentDialogInit = NULL;
	g_currentDialogLogic = NULL;
}

HWND ShowModelessDialog(HWND parentHwnd, LPCTSTR title, int width, int height,
                        void (*dialogInitFunc)(HWND), void (*dialogLogicFunc)(HWND, UINT, WPARAM, LPARAM, int))
{
	const wchar_t DIALOG_CLASS[] = L"CVUIL_Modeless_Dialog_Class";
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = DialogProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = DIALOG_CLASS;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	RegisterClass(&wc);
	
	g_currentDialogInit = dialogInitFunc;
	g_currentDialogLogic = dialogLogicFunc;
	
	HWND hDlg = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		DIALOG_CLASS, title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		parentHwnd, NULL, GetModuleHandle(NULL), NULL);
	
	return hDlg;
}

void CloseDialog(HWND dialogHwnd)
{
	if (dialogHwnd != NULL && IsWindow(dialogHwnd))
		DestroyWindow(dialogHwnd);
}

// ==================== 异步任务支持函数 (Alpha4新增) ====================

void RunAsync(AsyncTaskFunc taskFunc, AsyncCallbackFunc callbackFunc, void* param)
{
	InitAsyncSystem();
	
	EnterCriticalSection(&g_asyncCS);
	int slot = GetAvailableAsyncTaskSlot();
	if (slot < 0)
	{
		LeaveCriticalSection(&g_asyncCS);
		return;
	}
	
	AsyncTask* task = &g_asyncTasks[slot];
	task->id = slot;
	task->active = TRUE;
	task->cancelled = FALSE;
	task->taskFunc = taskFunc;
	task->callbackFunc = callbackFunc;
	task->param = param;
	task->progressFunc = NULL;
	task->taskWithProgressFunc = NULL;
	g_asyncTaskCount++;
	
	LeaveCriticalSection(&g_asyncCS);
	
	task->thread = (HANDLE)_beginthreadex(NULL, 0, AsyncTaskThread, (void*)(intptr_t)slot, 0, NULL);
}

void RunAsyncWithProgress(AsyncTaskWithProgressFunc taskFunc, AsyncProgressFunc progressFunc, 
                          AsyncCallbackFunc callbackFunc, void* param)
{
	InitAsyncSystem();
	
	EnterCriticalSection(&g_asyncCS);
	int slot = GetAvailableAsyncTaskSlot();
	if (slot < 0)
	{
		LeaveCriticalSection(&g_asyncCS);
		return;
	}
	
	AsyncTask* task = &g_asyncTasks[slot];
	task->id = slot;
	task->active = TRUE;
	task->cancelled = FALSE;
	task->taskFunc = NULL;
	task->callbackFunc = callbackFunc;
	task->param = param;
	task->progressFunc = progressFunc;
	task->taskWithProgressFunc = taskFunc;
	g_asyncTaskCount++;
	
	LeaveCriticalSection(&g_asyncCS);
	
	task->thread = (HANDLE)_beginthreadex(NULL, 0, AsyncTaskWithProgressThread, (void*)(intptr_t)slot, 0, NULL);
}

void Delay(int milliseconds, AsyncCallbackFunc callbackFunc, void* param)
{
	InitAsyncSystem();
	
	EnterCriticalSection(&g_asyncCS);
	int slot = GetAvailableDelayTaskSlot();
	if (slot < 0)
	{
		LeaveCriticalSection(&g_asyncCS);
		return;
	}
	
	DelayTask* task = &g_delayTasks[slot];
	task->id = slot;
	task->active = TRUE;
	task->milliseconds = milliseconds;
	task->callbackFunc = callbackFunc;
	task->param = param;
	g_delayTaskCount++;
	
	LeaveCriticalSection(&g_asyncCS);
	
	task->thread = (HANDLE)_beginthreadex(NULL, 0, DelayTaskThread, (void*)(intptr_t)slot, 0, NULL);
}

void CancelAllAsyncTasks()
{
	if (!g_asyncInitialized) return;
	
	EnterCriticalSection(&g_asyncCS);
	
	for (int i = 0; i < MAX_ASYNC_TASKS; i++)
	{
		if (g_asyncTasks[i].active)
		{
			g_asyncTasks[i].cancelled = TRUE;
			if (g_asyncTasks[i].thread != NULL)
			{
				WaitForSingleObject(g_asyncTasks[i].thread, 100);
				CloseHandle(g_asyncTasks[i].thread);
				g_asyncTasks[i].thread = NULL;
			}
			g_asyncTasks[i].active = FALSE;
		}
	}
	
	for (int i = 0; i < MAX_ASYNC_TASKS; i++)
	{
		if (g_delayTasks[i].active)
		{
			g_delayTasks[i].active = FALSE;
			if (g_delayTasks[i].thread != NULL)
			{
				WaitForSingleObject(g_delayTasks[i].thread, 100);
				CloseHandle(g_delayTasks[i].thread);
				g_delayTasks[i].thread = NULL;
			}
		}
	}
	
	g_asyncTaskCount = 0;
	g_delayTaskCount = 0;
	
	LeaveCriticalSection(&g_asyncCS);
}

int GetRunningAsyncTaskCount()
{
	return g_asyncTaskCount + g_delayTaskCount;
}

// ==================== 辅助函数 ====================

HWND GetComponentHWND(HWND hwnd, int id)
{
	return GetDlgItem(hwnd, id);
}

int GetComponentId(HWND hwndComponent)
{
	return GetDlgCtrlID(hwndComponent);
}

void SetComponentText(HWND hwnd, int id, LPCTSTR text)
{
	if (text == NULL) text = L"";
	SetWindowText(GetDlgItem(hwnd, id), text);
}

LPCTSTR GetComponentText(HWND hwnd, int id)
{
	int length = GetWindowTextLength(GetDlgItem(hwnd, id));
	if (length == 0) return NULL;
	TCHAR* text = new TCHAR[length + 1];
	GetWindowText(GetDlgItem(hwnd, id), text, length + 1);
	return text;
}

void FreeString(LPCTSTR str)
{
	if (str != NULL) delete[] str;
}

LPCTSTR NumToStr(short number)
{
	static wchar_t buffer[8];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%hd", number);
	return buffer;
}

LPCTSTR NumToStr(unsigned short number)
{
	static wchar_t buffer[8];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%hu", number);
	return buffer;
}

LPCTSTR NumToStr(int number)
{
	static wchar_t buffer[16];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%d", number);
	return buffer;
}

LPCTSTR NumToStr(unsigned int number)
{
	static wchar_t buffer[16];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%u", number);
	return buffer;
}

LPCTSTR NumToStr(long number)
{
	static wchar_t buffer[16];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%ld", number);
	return buffer;
}

LPCTSTR NumToStr(unsigned long number)
{
	static wchar_t buffer[16];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%lu", number);
	return buffer;
}

LPCTSTR NumToStr(long long number)
{
	static wchar_t buffer[32];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%lld", number);
	return buffer;
}

LPCTSTR NumToStr(unsigned long long number)
{
	static wchar_t buffer[32];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%llu", number);
	return buffer;
}

LPCTSTR NumToStr(float number)
{
	static wchar_t buffer[32];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%f", number);
	return buffer;
}

LPCTSTR NumToStr(double number)
{
	static wchar_t buffer[32];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%lf", number);
	return buffer;
}

LPCTSTR NumToStr(long double number)
{
	static wchar_t buffer[64];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%Lf", number);
	return buffer;
}

// ==================== 主界面和消息处理 ====================

int MainInterfaceInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Main Window Class";
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	mainHwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		CLASS_NAME,
		*mainUIName,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, mainUIwidth, mainUIheight,
		NULL, NULL, hInstance, NULL);
	
	if (mainHwnd == NULL) return 0;
	
	ShowWindow(mainHwnd, nCmdShow);
	UpdateWindow(mainHwnd);
	
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES | ICC_PROGRESS_CLASS | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	
	// 初始化异步系统
	InitAsyncSystem();
	
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	// 清理异步系统
	CleanupAsyncSystem();
	
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hComponent = (HWND)lParam;
	int id = GetDlgCtrlID(hComponent);
	int loword = LOWORD(wParam);
	
	switch (uMsg)
	{
		case WM_CREATE:
			InitLogic(hwnd, uMsg, wParam, lParam);
			break;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hwnd, &ps);
			break;
		}
		
		case WM_HSCROLL:
		case WM_VSCROLL:
			Logic(hwnd, uMsg, wParam, lParam, id);
			break;
			
		case WM_COMMAND:
			Logic(hwnd, uMsg, wParam, lParam, loword);
			break;
			
		case WM_NOTIFY:
			break;
			
		// ==================== 异步任务消息处理 (Alpha4新增) ====================
		case WM_ASYNC_CALLBACK:
		{
			int taskId = (int)(intptr_t)wParam;
			if (taskId >= 0 && taskId < MAX_ASYNC_TASKS && g_asyncTasks[taskId].active)
			{
				AsyncTask* task = &g_asyncTasks[taskId];
				if (!task->cancelled && task->callbackFunc != NULL)
				{
					task->callbackFunc(task->param);
				}
				
				EnterCriticalSection(&g_asyncCS);
				if (task->thread != NULL)
				{
					CloseHandle(task->thread);
					task->thread = NULL;
				}
				task->active = FALSE;
				g_asyncTaskCount--;
				LeaveCriticalSection(&g_asyncCS);
			}
			return 0;
		}
		
		case WM_ASYNC_PROGRESS:
		{
			int taskId = (int)(intptr_t)wParam;
			int progress = (int)(intptr_t)lParam;
			if (taskId >= 0 && taskId < MAX_ASYNC_TASKS && g_asyncTasks[taskId].active)
			{
				AsyncTask* task = &g_asyncTasks[taskId];
				if (!task->cancelled && task->progressFunc != NULL)
				{
					task->progressFunc(progress);
				}
			}
			return 0;
		}
		
		case WM_DELAY_CALLBACK:
		{
			int taskId = (int)(intptr_t)wParam;
			if (taskId >= 0 && taskId < MAX_ASYNC_TASKS && g_delayTasks[taskId].active)
			{
				DelayTask* task = &g_delayTasks[taskId];
				if (task->callbackFunc != NULL)
				{
					task->callbackFunc(task->param);
				}
				
				EnterCriticalSection(&g_asyncCS);
				if (task->thread != NULL)
				{
					CloseHandle(task->thread);
					task->thread = NULL;
				}
				task->active = FALSE;
				g_delayTaskCount--;
				LeaveCriticalSection(&g_asyncCS);
			}
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int MainWindow(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR pCmdLine, _In_ int nCmdShow)
{
	CreateMainInterface(mainUIName, &mainUIwidth, &mainUIheight);
	if (mainUIName[0] == NULL) mainUIName[0] = L"MainUI";
	if (mainUIwidth <= 0 || mainUIheight <= 0)
	{
		mainUIwidth = 640;
		mainUIheight = 360;
	}
	return MainInterfaceInit(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}
