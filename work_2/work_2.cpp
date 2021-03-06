// work_2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "work_2.h"
#include "use.h"

#define MAX_LOADSTRING 100


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WORK2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WORK2));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORK2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WORK2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
#define SHOW 223
#define OPEN 998
#define SAVE 4684
#define WIDTH 79
#define HEIGHT 140
#define GREY 18

HWND show_win;
HWND hopen;
HWND hshow;
HWND hsave;
HWND hwidth;
HWND hheight;
HWND hgrey;
HWND hx;
HWND h1;
HWND h2;

char fileName[100] = { 0 }, saveName[100] = { 0 };
int zero, h, w, g;
NODE *buf;
BITMAPFILEHEADER bf;
BITMAPINFOHEADER bi;
FILE *fp;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		650, 200, 500, 300, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	show_win = CreateWindow(szWindowClass, TEXT("显示"), WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 10, 800, 600, NULL, NULL, hInst, NULL);
	hopen = CreateWindow(TEXT("button"), TEXT("打开"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 150, 80, 50, hWnd, (HMENU)OPEN, hInst, NULL);
	hshow = CreateWindow(TEXT("button"), TEXT("显示"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		200, 150, 80, 50, hWnd, (HMENU)SHOW, hInst, NULL);
	hsave = CreateWindow(TEXT("button"), TEXT("保存"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		350, 150, 80, 50, hWnd, (HMENU)SAVE, hInst, NULL);
	hwidth = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
		80, 80, 80, 20, hWnd, (HMENU)WIDTH, hInst, NULL);
	hheight = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
		180, 80, 80, 20, hWnd, (HMENU)HEIGHT, hInst, NULL);
	hx = CreateWindow(TEXT("edit"), TEXT("X"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY,
		160, 80, 20, 20, hWnd, (HMENU)888, hInst, NULL);
	hgrey = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
		280, 80, 80, 20, hWnd, (HMENU)GREY, hInst, NULL);
	h1 = CreateWindow(TEXT("edit"), TEXT("分辨率"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY,
		140, 40, 60, 20, hWnd, (HMENU)888, hInst, NULL);
	h2 = CreateWindow(TEXT("edit"), TEXT("灰度级"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY,
		290, 40, 60, 20, hWnd, (HMENU)888, hInst, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case OPEN: {
			TCHAR path[1000];
			OPENFILENAME ofn = { sizeof(OPENFILENAME) };
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = TEXT("bmp\0*.BMP\0");
			lstrcpy(path, TEXT(""));
			ofn.lpstrFile = path;
			ofn.nMaxFile = sizeof(path);
			ofn.lpstrTitle = TEXT("选择文件");
			BOOL bok = GetOpenFileName(&ofn);
			if (bok) {
				TcharToChar(path, fileName);
			}
			fp = fopen(fileName, "rb+");
			fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
			fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
			if (bf.bfReserved1 == 0) {
				bf.bfReserved1 = (WORD)256;
			}
			if (bf.bfReserved2 == 0) {
				bf.bfReserved2 = (WORD)bi.biWidth;
			}
			SetWindowTextA(hwidth, to_String(bf.bfReserved2).c_str());
			SetWindowTextA(hheight, to_String(bi.biHeight*bf.bfReserved2 / bi.biWidth).c_str());
			SetWindowTextA(hgrey, to_String(bf.bfReserved1).c_str());
			int w_real = (bi.biWidth * 24 + 31) / 8 / 4 * 4;
			zero = w_real - bi.biWidth * 3;
			buf = readBMP(fp, bi.biWidth, bi.biHeight, zero);
			break;
		}
		case SHOW:
		{
			MoveWindow(show_win, 10, 10, bi.biWidth + 100, bi.biHeight + 100, TRUE);
			NODE *p;
			char w_temp[10] = { 0 }, h_temp[10] = { 0 }, g_temp[10] = { 0 };
			GetWindowTextA(hwidth, w_temp, 10);
			GetWindowTextA(hheight, h_temp, 10);
			GetWindowTextA(hgrey, g_temp, 10);
			w = charToint(w_temp);
			h = charToint(h_temp);
			g = charToint(g_temp);
			bf.bfReserved2 = (WORD)w;
			bf.bfReserved1 = (WORD)g;
			p = buf;
			changeGrey(g, p, bi);
			p = buf;
			changePixel(w, h, p, bi);
			p = buf;
			showPic(p, show_win, bi.biWidth, bi.biHeight);
			break;
		}
		case SAVE: {
			TCHAR path[1000];
			OPENFILENAME ofn = { sizeof(OPENFILENAME) };
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = TEXT("bmp\0*.BMP\0");
			lstrcpy(path, TEXT(""));
			ofn.lpstrFile = path;
			ofn.nMaxFile = sizeof(path);
			ofn.lpstrTitle = TEXT("选择文件");
			ofn.hwndOwner = hWnd;
			BOOL bok = GetSaveFileName(&ofn);
			if (bok) {
				TcharToChar(path, saveName);
			}
			FILE *fpw = fopen(saveName, "wb");
			fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw);
			fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw);
			for (int i = 0; i < bi.biWidth*bi.biHeight; i++) {
				fwrite(&buf[i], sizeof(NODE), 1, fpw);
				if (i % bi.biWidth == (bi.biWidth - 1) && zero != 0) {
					char temp = 0;
					for (int j = 0; j < zero; j++)
						fwrite(&temp, sizeof(char), 1, fpw);
				}
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
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
