﻿// Project16SetTimer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// 타이머 설정/제거 : SetTimer/KillTimer

#include "pch.h"
#include "framework.h"
#include "Project16SetTimer.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


UINT g_timer1;
UINT g_timer2;


void CALLBACK MyTimerProc(HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD dwTime) 
{
	if (nIDEvent == 1) {
		g_timer1++;
	}
	else {
		g_timer2++;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE) {
		SetTimer(hWnd, 1, 10, MyTimerProc);
		SetTimer(hWnd, 2, 30, MyTimerProc);
		return 0;
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);

		HGDIOBJ h_oldBrush = GetCurrentObject(h_dc, OBJ_BRUSH);
		HGDIOBJ h_newBrush = GetStockObject(DC_BRUSH);
		SetDCBrushColor(h_dc, RGB(0, 100, 200));
		SelectObject(h_dc, h_newBrush);

		int x_direction = (int)(g_timer1 / 100) % 2;		// 나머지가 0=오른쪽, 1=왼쪽
		int x_step = (x_direction == 0) ? g_timer1 % 100 : 100 - (g_timer1 % 100);
		/*if (x_direction == 0) {
			x_step = g_timer1 % 100;
		}
		else {
			x_step = 100 - (g_timer1 % 100);
		}*/
		Ellipse(h_dc, 80 + 8*x_step, 50, 130 + 8*x_step, 100);

		SetDCBrushColor(h_dc, RGB(255, 100, 0));
		SelectObject(h_dc, h_newBrush);

		x_direction = (int)(g_timer2 / 100) % 2;		// 나머지가 0=오른쪽, 1=왼쪽
		x_step = (x_direction == 0) ? g_timer2 % 100 : 100 - (g_timer2 % 100);
		Ellipse(h_dc, 80 + 8*x_step, 150, 130 + 8*x_step, 200);

		SelectObject(h_dc, h_oldBrush);
		DeleteObject(h_newBrush);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT16SETTIMER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT16SETTIMER));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT16SETTIMER));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT16SETTIMER);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  500, 300, 1000, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// 정보 대화 상자의 메시지 처리기입니다.
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
