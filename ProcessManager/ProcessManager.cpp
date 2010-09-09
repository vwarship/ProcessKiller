// ProcessManager.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessHandler.h"
#include "Blacklist.h"
#include <atlbase.h>
#include <string>

#define MAX_LOADSTRING 100
#define VK_P 0x50

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROCESSMANAGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL/* | MOD_NOREPEAT*/, VK_BACK);  //0x42 is 'b'
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_CONTROL, VK_P);
    RegisterHotKey(NULL, 3, MOD_ALT, VK_DELETE);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	
	MessageBox(NULL, _T("������֧�������ȼ���\n\
-------------------------------------------------------------------------------------------------\n\
ɱ�����õĽ���\t\tCtrl+Alt+Backspace(�˸��)\n\
��ӡ��ǰϵͳ����\tCtrl+Alt+P (�ļ�������C:\\CurrentProcesses.txt)\n\
�˳�������\t\tAlt+Delete"), 
		_T("����"), MB_OK);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCESSMANAGER));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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
