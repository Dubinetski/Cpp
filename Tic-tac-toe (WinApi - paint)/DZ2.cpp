/*
Написать игру "крестики-нолики".
Используйте GDI рисование (обработчик WM_PAINT
и функции рисования MoveTo, LineTo, FillRect, PtInRect и др.).
*/

#include "stdafx.h"
#include "DZ2.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

enum {
	cade_count = 9,
	line_width = 4,
};
int current_rect = -1;
int player = 1;
int game_fild[cade_count] = { 0 };
int motion_count = 0;
int fill = 0;

RECT rc[9] = {
	RECT { 20, 20, 120, 120 }, RECT { 120, 20, 220, 120 }, RECT { 220, 20, 320, 120 },
RECT { 20, 120, 120, 220 }, RECT { 120, 120, 220, 220 }, RECT { 220, 120, 320, 220 },
RECT { 20, 220, 120, 320 }, RECT { 120, 220, 220, 320 }, RECT { 220, 220, 320, 320 } };

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
void press_rectangle(HWND window);
int check_victory(int fild[cade_count]);
void new_game(HWND window);
void set_current_rectangle(POINT pt);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DZ2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DZ2));

	while (GetMessage(&msg, NULL, 0, 0))
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DZ2));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd;

	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, L"Tic-tac-toe",
						WS_OVERLAPPED | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
						700, 300, 360, 380, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		set_current_rectangle(pt);
		press_rectangle(hWnd);
	}
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			HPEN hpen = CreatePen(PS_DOT, line_width, RGB(0, 0, 0));
			HGDIOBJ hOldPen = SelectObject(hdc, hpen);

			Line(hdc, 20, 120, 320, 120);
			Line(hdc, 20, 220, 320, 220);
			Line(hdc, 120, 20, 120, 320);
			Line(hdc, 220, 20, 220, 320);

			SelectObject(hdc, hOldPen);
			DeleteObject(hpen);
		}
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

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);	// установить перо в точку (x1;y1)
	return LineTo(hdc, x2, y2);		// провести линию до точки (x2;y2)
}

/* Обработка нажатия на игровое поле */
void press_rectangle(HWND window) {
	if (current_rect >= 0 && !game_fild[current_rect]) {
		HFONT hf, hNf;
		HDC hdc = GetDC(window);
		hNf = CreateFont(50, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman Cyr"); // создаем шрифт для Х, О
		hf = (HFONT)SelectObject(hdc, hNf);

		COLORREF rect_color = (player == 1) ? RGB(255, 128, 0) : RGB(0, 128, 0);	// выбираем цвет фона для каждого игрока
		SetTextColor(hdc, RGB(0, 0, 0));	//цвет текста
		SetBkColor(hdc, rect_color);		//устанавливаем цвет фона

		LPCWSTR ch = (player == 1) ? _T("X") : _T("O");

		HBRUSH hBrush = CreateSolidBrush(rect_color);

		RECT rt; // устанавливаем размеры прямоугольника, чтобы они не перекрывали игровую сетку
		rt.left = rc[current_rect].left + line_width / 2;
		rt.right = rc[current_rect].right - line_width / 2;
		rt.top = rc[current_rect].top + line_width / 2;
		rt.bottom = rc[current_rect].bottom - line_width / 2;

		FillRect(hdc, &rt, hBrush);		// закрашиваем выбранное поле
		TextOut(hdc, rc[current_rect].left + 35, rc[current_rect].top + 30, ch, 2); // наносим Х или О на поле

		ReleaseDC(window, hdc);		// применяем изменения к окну
		++motion_count;
		game_fild[current_rect] = player;
		player *= -1;
		fill = 1;

		DeleteObject(hBrush);

		switch (check_victory(game_fild)) {
		case 0:
			MessageBox(window, L"Draw !", L"Result", MB_OK);		// ничья
			new_game(window);
			break;
		case 1:
			MessageBox(window, L"Player 1 is won !", L"Result", MB_OK);	// победил игрок 1
			new_game(window);
			break;
		case 2:
			MessageBox(window, L"Player 2 is won !", L"Result", MB_OK);	// победил игрок 2
			new_game(window);
			break;
		}
	}
}

/* Проверка победителя */
int check_victory(int fild[cade_count]) {
	if (
		(fild[0] + fild[1] + fild[2]) == 3 || (fild[3] + fild[4] + fild[5]) == 3 ||
		(fild[6] + fild[7] + fild[8]) == 3 || (fild[0] + fild[3] + fild[6]) == 3 ||
		(fild[1] + fild[4] + fild[7]) == 3 || (fild[2] + fild[5] + fild[8]) == 3 ||
		(fild[0] + fild[4] + fild[8]) == 3 || (fild[2] + fild[4] + fild[6]) == 3)
		return 1;
	else if (
		(fild[0] + fild[1] + fild[2]) == -3 || (fild[3] + fild[4] + fild[5]) == -3 ||
		(fild[6] + fild[7] + fild[8]) == -3 || (fild[0] + fild[3] + fild[6]) == -3 ||
		(fild[1] + fild[4] + fild[7]) == -3 || (fild[2] + fild[5] + fild[8]) == -3 ||
		(fild[0] + fild[4] + fild[8]) == -3 || (fild[2] + fild[4] + fild[6]) == -3)
		return 2;
	else if (motion_count == cade_count)
		return 0;
	else
		return -1;
}

/* "Обнуление" игрового поля */
void new_game(HWND window) {
	player = 1;
	motion_count = 0;
	current_rect = -1;
	for (auto i = 0; i < cade_count; ++i)
		game_fild[i] = 0;
	InvalidateRect(window, NULL, TRUE); // перерисовываем окно
}

/* Установить номер выбранного поля */
void set_current_rectangle(POINT pt) {
	if (PtInRect(&rc[0], pt))
		current_rect = 0;
	else if (PtInRect(&rc[1], pt))
		current_rect = 1;
	else if (PtInRect(&rc[2], pt))
		current_rect = 2;
	else if (PtInRect(&rc[3], pt))
		current_rect = 3;
	else if (PtInRect(&rc[4], pt))
		current_rect = 4;
	else if (PtInRect(&rc[5], pt))
		current_rect = 5;
	else if (PtInRect(&rc[6], pt))
		current_rect = 6;
	else if (PtInRect(&rc[7], pt))
		current_rect = 7;
	else if (PtInRect(&rc[8], pt))
		current_rect = 8;
}