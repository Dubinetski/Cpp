/*
Написать игру "крестики-нолики".
Используйте диалоговое окно и кнопки.
*/

#include "stdafx.h"
#include "DZ1.h"

enum {cade_count = 9};

int player = 1;
int game_fild[cade_count] = { 0 };
int motion_count = 0;

void press_button(HWND window, int button);
int check_victory(int fild[cade_count]);
void new_game(HWND window);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	// создаём главное окно приложения на основе немодального диалога
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_GAME), NULL, DlgProc);
	// Отображаем окно  
	ShowWindow(hDialog, nCmdShow);
	//Запускаем цикл обработки сообщений
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT mes, WPARAM wp, LPARAM lp) {
	switch (mes) {
	case WM_CLOSE:				// закрываем немодальный диалог
		DestroyWindow(hDlg);	// разрушаем окно
		PostQuitMessage(0);		// останавливаем цикл обработки сообщений
		return TRUE;
	case WM_COMMAND:
		// обрабатываем нажатие на кнопки игрового поля
		switch (LOWORD(wp)) {
		case IDC_BUTTON1:
			press_button(hDlg, IDC_BUTTON1);
			break;
		case IDC_BUTTON2:
			press_button(hDlg, IDC_BUTTON2);
			break;
		case IDC_BUTTON3:
			press_button(hDlg, IDC_BUTTON3);
			break;
		case IDC_BUTTON4:
			press_button(hDlg, IDC_BUTTON4);
			break;
		case IDC_BUTTON5:
			press_button(hDlg, IDC_BUTTON5);
			break;
		case IDC_BUTTON6:
			press_button(hDlg, IDC_BUTTON6);
			break;
		case IDC_BUTTON7:
			press_button(hDlg, IDC_BUTTON7);
			break;
		case IDC_BUTTON8:
			press_button(hDlg, IDC_BUTTON8);
			break;
		case IDC_BUTTON9:
			press_button(hDlg, IDC_BUTTON9);
			break;
		}
		// Проверяем не появился ли победитель
		switch (check_victory(game_fild)) {
		case 0:
			MessageBox(hDlg, L"Draw !", L"Result", MB_OK);		// ничья
			new_game(hDlg);
			break;
		case 1:
			MessageBox(hDlg, L"Player 1 is won !", L"Result", MB_OK);	// победил игрок 1
			new_game(hDlg);
			break;
		case 2:
			MessageBox(hDlg, L"Player 2 is won !", L"Result", MB_OK);	// победил игрок 2
			new_game(hDlg);
			break;
		}
	}
	return FALSE;
}

/* Обработка нажатия на кнопку игрового поля */
void press_button(HWND window, int button) {
	TCHAR str[2];
	GetDlgItemText(window, button, str, 2);
	if (wcscmp(str, L"X") && wcscmp(str, L"O")) {
		game_fild[button - IDC_BUTTON1] = player;
		if (player == 1) {
			SetDlgItemText(window, button, _T("X"));
			player = -1;
		} else {
			SetDlgItemText(window, button, _T("O"));
			player = 1;
		}
		++motion_count;
	}
}

/* Проверка победителя */
int check_victory(int fild[cade_count]) {
	if (motion_count == cade_count)
		return 0;
	else if (
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
	else
		return -1;
}

/* "Обнуление" игрового поля */
void new_game(HWND window) {
	for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; i++) {
		SetDlgItemText(window, i, _T(""));
		player = 1;
		motion_count = 0;
		for (auto i = 0; i < cade_count; ++i)
			game_fild[i] = 0;
	}
}