// Win32APIwindows.cpp : Defines the entry point for the application.
//


#include "stdafx.h"

#include "Win32APIwindows.h"


// Global Variables

HINSTANCE			hInst;										// current instance
TCHAR				szTitle[] = _T("Paint");			// The title bar text
TCHAR				szWindowClass[] = _T("WIN32APIWINDOWS");	// the main window class name


// Forward declarations of functions included in this code module

ATOM				MyRegisterClass(HINSTANCE);
ATOM				MyRegisterClassChild(HINSTANCE);//--------------------------
BOOL				InitInstance(HINSTANCE, int);
LRESULT	CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT	CALLBACK	WndProcChild(HWND, UINT, WPARAM, LPARAM);//-------------------------------
INT_PTR	CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hChild;//------------------------------

//
//  FUNCTION: _tWinMain
//
//  PURPOSE: Program entry point.
//



int	APIENTRY		_tWinMain
(
	_In_     HINSTANCE			hInstance,
	_In_opt_ HINSTANCE			hPrevInstance,
	_In_     LPTSTR				lpCmdLine,
	_In_     int				nCmdShow
)
{
	MSG					msg;

	// Register window classes

	MyRegisterClass(hInstance);

	// Perform application initialization

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	// Main message loop

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// The End

	return (int)msg.wParam;
}


//
//  FUNCTION: MyRegisterClass
//
//  PURPOSE: Registers the window class.
//

ATOM				MyRegisterClass
(
	HINSTANCE			hInstance
)
{
	WNDCLASSEX			wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32APIWINDOWS));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32APIWINDOWS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS: In this function, we save the instance handle in a global variable and
//             create and display the main program window.
//

BOOL				InitInstance
(
	HINSTANCE			hInstance,
	int					nCmdShow
)
{
	HWND				hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//
//  FUNCTION: WndProc
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//

LRESULT	CALLBACK	WndProc
(
	HWND				hWnd,
	UINT				message,
	WPARAM				wParam,
	LPARAM				lParam
)
{
	int					wmId, wmEvent;
	PAINTSTRUCT			ps;
	HDC					hdc;

	RECT rc;
	static POINT p = { 10, 10 };
	static POINT points[500];
	static int index = 0;
	static int show = 0;
	static int showLastEight = 0;

	static unsigned int total = 0;
	static unsigned int cursor = 0;
	static unsigned int leftBtn = 0;
	static unsigned int middleBtn = 0;
	static unsigned int rightBtn = 0;

	static unsigned int isClicked = 0;
	static unsigned int colour = 0;

	char buffer[500];

	total++;

	switch (message)
	{
	case WM_CREATE:
		total = 0;
		cursor = 0;
		leftBtn = 0;
		middleBtn = 0;
		rightBtn = 0;


		WNDCLASSEX			wcexChild;

		wcexChild.cbSize = sizeof(WNDCLASSEX);

		wcexChild.style = CS_HREDRAW | CS_VREDRAW;
		wcexChild.lpfnWndProc = WndProcChild;
		wcexChild.cbClsExtra = 0;
		wcexChild.cbWndExtra = 0;
		wcexChild.hInstance = hInst;
		wcexChild.hIcon = NULL;
		wcexChild.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcexChild.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcexChild.lpszMenuName = NULL;
		wcexChild.lpszClassName = _T("ChildWindow");
		wcexChild.hIconSm = NULL;

		RegisterClassEx(&wcexChild);


		GetClientRect(hWnd, &rc);
		/*
			hChild = CreateWindow(
				_T("ChildWindow"),
				szTitle,
				WS_CHILD | WS_BORDER | WS_CAPTION | WS_VISIBLE,
				200, 200, rc.right / 4, rc.bottom / 4,
				hWnd,
				NULL,
				hInst,
				NULL);
			ShowWindow(hChild, SW_SHOW);
			UpdateWindow(hChild);
		*/
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, colour, 0));
		SelectObject(hdc, GetStockObject(DC_PEN));
		SetDCPenColor(hdc, RGB(255, colour, 0));

		Ellipse(hdc, p.x - 9, p.y + 9, p.x + 9, p.y - 9);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		isClicked = 1;
		break;
	case WM_LBUTTONUP:
		isClicked = 0;
		break;
	case WM_RBUTTONUP:
		colour = 0;
		break;
	case WM_RBUTTONDOWN:
		colour = 255;
		break;
	case WM_MOUSEMOVE:
		if (isClicked)
		{
			p.x = (LONG)LOWORD(lParam);
			p.y = (LONG)HIWORD(lParam);

			points[index % 8].x = p.x;
			points[index % 8].y = p.y;
			index++;

			InvalidateRect(hWnd, NULL, FALSE);
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

LRESULT	CALLBACK	WndProcChild
(
	HWND				hWnd,
	UINT				message,
	WPARAM				wParam,
	LPARAM				lParam
)
{
	int					wmId, wmEvent;
	PAINTSTRUCT			ps;
	HDC					hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections
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
		// Add any drawing code here
		EndPaint(hWnd, &ps);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//
//  FUNCTION: About
//
//  PURPOSE: Message handler for about box.
//

INT_PTR	CALLBACK	About
(
	HWND				hDlg,
	UINT				message,
	WPARAM				wParam,
	LPARAM				lParam
)
{
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


