// ProcessManager.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessHandler.h"
#include "Blacklist.h"
#include <atlbase.h>
#include <string>

#define MAX_LOADSTRING 100
#define VK_P 0x50

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void DispatchHotKey(UINT uVirKey);
bool IsProgramRuning();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (IsProgramRuning())
	{	
		return 0;
	}

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROCESSMANAGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL/* | MOD_NOREPEAT*/, VK_BACK);  //0x42 is 'b'
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_CONTROL, VK_P);
    RegisterHotKey(NULL, 3, MOD_ALT, VK_DELETE);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	
	MessageBox(NULL, _T("本工具支持以下热键：\n\
-------------------------------------------------------------------------------------------------\n\
杀掉配置的进程\t\tCtrl+Alt+Backspace(退格键)\n\
打印当前系统进程\tCtrl+Alt+P (文件保存在C:\\CurrentProcesses.txt)\n\
退出本工具\t\tAlt+Delete"), 
		_T("帮助"), MB_OK);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCESSMANAGER));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_HOTKEY)
        {
			UINT uVirtKey = (UINT) HIWORD(msg.lParam);
			DispatchHotKey(uVirtKey);
        }

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROCESSMANAGER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROCESSMANAGER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool IsProgramRuning()
{
	TCHAR buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	
	std::string exe_name(CT2A(buf).m_psz);
	std::string::size_type pos = exe_name.find_last_of("\\");
	if (pos != std::string::npos)
	{
		exe_name = exe_name.substr(pos+1);
	}

	int count = 0;
	ProcessHandler ph;
	std::vector<Process> processes = ph.GetCurrentProcesses();
	std::vector<Process>::iterator iter = processes.begin();
	for (; iter != processes.end(); ++iter)
	{
		if (_stricmp(iter->ExeFile().c_str(), exe_name.c_str()) == 0)
		{
			++count;
		}
	}

	if (count >= 2)
	{
		return true;
	}

	return false;
}

void DispatchHotKey(UINT uVirtKey)
{
	ProcessHandler ph;

	if (uVirtKey == VK_P)
	{
		ph.WriteProcessesInfoToFile("CurrentProcesses.txt");
	}
	else if (uVirtKey == VK_BACK)
	{
		Blacklist blacklist;
		blacklist.ReadConfigureFile("config.ini");
		ph.KillProcesses(blacklist);
	}
	else if (uVirtKey == VK_DELETE)
	{
		PostQuitMessage(0);
	}
}
