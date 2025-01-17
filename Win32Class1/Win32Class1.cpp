﻿// Win32Class1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Win32Class1.h"
#include <windowsx.h>

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


#define MAX_COUNT	100


typedef struct _ObjectData {
	int x, y;
	int type;
} ObjectData;

ObjectData g_dataList[MAX_COUNT];
int g_dataCount;


HPEN g_hGreenPen, g_hRedPen, g_hOldPen;
HBRUSH g_hOldBrush;



// 클래스화 작업 : 기본(부모)추상클래스
class MyDC
{
protected:
	HDC m_hDC = NULL;
	HWND m_hWnd = NULL;

public:
	MyDC(HWND a_hWnd)
	{
		m_hWnd = a_hWnd;
	}
	virtual ~MyDC()
	{
	}

	HGDIOBJ SelectObject(HGDIOBJ a_hObj)
	{
		return ::SelectObject(m_hDC, a_hObj);
	}

	HGDIOBJ GetCurrentObject(UINT a_OBJ)
	{
		return ::GetCurrentObject(m_hDC, a_OBJ);
	}

	void Rectangle(int x1, int y1, int x2, int y2)
	{
		::Rectangle(m_hDC, x1, y1, x2, y2);
	}

	void Ellipse(int x1, int y1, int x2, int y2)
	{
		::Ellipse(m_hDC, x1, y1, x2, y2);
	}
};



// 클래스화 작업 : 파생(자식)클래스
// WM_LBUTTONDOWN 에서 그리기 객체 만들 때 사용
class MyClientDC : public MyDC
{
public:
	MyClientDC(HWND a_hWnd) : MyDC(a_hWnd)
	{
		m_hDC = GetDC(m_hWnd);
	}
	virtual ~MyClientDC()
	{
		ReleaseDC(m_hWnd, m_hDC);
	}
};



// 클래스화 작업 : 파생(자식)클래스
// WM_PAINT 에서 그리기 객체 만들 때 사용
class MyPaintDC : public MyDC
{
protected:
	PAINTSTRUCT m_ps;

public:
	MyPaintDC(HWND a_hWnd) : MyDC(a_hWnd)
	{
		m_hDC = BeginPaint(m_hWnd, &m_ps);
	}
	virtual ~MyPaintDC()
	{
		EndPaint(m_hWnd, &m_ps);
	}
};




//  함수: WndProc()
//  용도: 윈도 메시지 처리함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		g_hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 10));
		g_hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 10, 10));
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		
		
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		if (g_dataCount < MAX_COUNT) {
			g_dataList[g_dataCount].x = x;
			g_dataList[g_dataCount].y = y;
			g_dataList[g_dataCount].type = wParam & MK_CONTROL;		// 0(사각형) or 8(원)
			g_dataCount++;
		}

		
		// 클래스로 만든 dc
		MyClientDC dc(hWnd);

		g_hOldPen = (HPEN)dc.GetCurrentObject(OBJ_PEN);
		g_hOldBrush = (HBRUSH)dc.SelectObject(GetStockObject(NULL_BRUSH));

		// Ctrl키 + 클릭 = 원
		if (wParam & MK_CONTROL) {
			dc.SelectObject(g_hRedPen);
			dc.Ellipse(x - 15, y - 15, x + 15, y + 15);
		}
		// 그냥클릭 = 사각형
		else {
			dc.SelectObject(g_hGreenPen);
			dc.Rectangle(x - 15, y - 15, x + 15, y + 15);
		}
		
		dc.SelectObject(g_hOldPen);
		dc.SelectObject(g_hOldBrush);
	}
	else if (uMsg == WM_RBUTTONDOWN) {

	}
	else if (uMsg == WM_TIMER) {

	}
	else if (uMsg == WM_PAINT) {
		MyPaintDC dc(hWnd);

		g_hOldPen = (HPEN)dc.GetCurrentObject(OBJ_PEN);
		g_hOldBrush = (HBRUSH)dc.SelectObject(GetStockObject(NULL_BRUSH));

		ObjectData* p = g_dataList;

		for (int i = 0; i < g_dataCount; i++ ) {

			if (p->type == 0) {
				dc.SelectObject(g_hGreenPen);
				dc.Rectangle(p->x - 15, p->y - 15, p->x + 15, p->y + 15);
			}
			else {
				dc.SelectObject(g_hRedPen);
				dc.Ellipse(p->x - 15, p->y - 15, p->x + 15, p->y + 15);
			}
			p++;
		}

		dc.SelectObject(g_hOldPen);
		dc.SelectObject(g_hOldBrush);
		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		DeleteObject(g_hGreenPen);
		DeleteObject(g_hRedPen);
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32CLASS1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32CLASS1));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32CLASS1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW -2);
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32CLASS1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
