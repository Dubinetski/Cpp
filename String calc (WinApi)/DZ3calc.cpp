/*
Ќаписать приложение "калькул¤тор".
»спользуйте диалоговое окно.
*/

#include "stdafx.h"
#include "DZ3calc.h"

std::wstring str = L"";		// отображаема¤ строка
bool isCalculated = true;	// признак завершенности вычислени¤

double calculate(std::wstring);
double number(std::wstring&);
double calc_high_priority(std::wstring&);
double calc(std::wstring&);
double brackets(std::wstring&);
TCHAR get_last_char(std::wstring&);

void delZiro();
void putCipher(wchar_t c);
void putAction(wchar_t c);

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

using namespace std::string_literals;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	// создаЄм главное окно приложени¤ на основе немодального диалога
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CALC), NULL, DlgProc);

	HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_DZ3CALC));

	// ќтображаем окно  
	ShowWindow(hDialog, nCmdShow);
	//«апускаем цикл обработки сообщений
	while (GetMessage(&msg, 0, 0, 0)) {
		if (!TranslateAccelerator(hDialog, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT mes, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (mes) {
		case WM_CLOSE:				// закрываем немодальный диалог
			DestroyWindow(hDlg);	// разрушаем окно
			PostQuitMessage(0);		// останавливаем цикл обработки сообщений
			return TRUE;
		case WM_COMMAND: {
			if (LOWORD(wp) == IDC_BTN0)				putCipher(L'0');
			else if (LOWORD(wp) == IDC_BTN1)		putCipher(L'1');
			else if (LOWORD(wp) == IDC_BTN2)		putCipher(L'2');
			else if (LOWORD(wp) == IDC_BTN3)		putCipher(L'3');
			else if (LOWORD(wp) == IDC_BTN4)		putCipher(L'4');
			else if (LOWORD(wp) == IDC_BTN5)		putCipher(L'5');
			else if (LOWORD(wp) == IDC_BTN6)		putCipher(L'6');
			else if (LOWORD(wp) == IDC_BTN7)		putCipher(L'7');
			else if (LOWORD(wp) == IDC_BTN8)		putCipher(L'8');
			else if (LOWORD(wp) == IDC_BTN9)		putCipher(L'9');
			else if (LOWORD(wp) == IDC_BtnSUMM)		putAction(L'+');
			else if (LOWORD(wp) == IDC_BtnMinus)	putAction(L'-');
			else if (LOWORD(wp) == IDC_BtnMulti)	putAction(L'*');
			else if (LOWORD(wp) == IDC_BtnDiv)		putAction(L'/');
			else if (LOWORD(wp) == IDC_BtnClr)		str = L"";
			else if (LOWORD(wp) == IDC_BtnBRL) {
				str += L" (";
				isCalculated = false;
			} else if (LOWORD(wp) == IDC_BtnBRR) {
				str += L") ";
				isCalculated = false;
			} else if (LOWORD(wp) == IDC_BtnPoint) {
				if (str.back() != L'.')
					str += L'.';
				isCalculated = false;
			} else if (LOWORD(wp) == IDC_BtnDel) {
				if (str.size())	str.pop_back();
			} else if (LOWORD(wp) == IDC_BtnCALC) {
				try {
					str = std::to_wstring(calculate(str));
					delZiro();
					isCalculated = true;
				} catch (const std::wstring& exept_str) {
					MessageBox(hDlg, LPCTSTR(exept_str.c_str()), NULL, MB_OK);
					str = L"";
				}
			}
			InvalidateRect(hDlg, NULL, 0);
			break;
		}
		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);

			SetDlgItemText(hDlg, IDC_TextFild, LPCTSTR(str.c_str()));

			EndPaint(hDlg, &ps);
			break;
	}
	return FALSE;
}

/* ”даление незначимых нулей в дробной части */
void delZiro() {
	if (str.find(L'.') != std::wstring::npos) {
		while (str.back() != L'.') {
			if (str.back() == L'0')
				str.pop_back();
			else
				return;
		}
		str.pop_back();
	}
}

/* ƒобавление нажатой цифры в строку */
void putCipher(wchar_t c) {
	isCalculated ? str = c : str += c;
	isCalculated = false;
}

/* ƒобавление символа математической операции в строку */
void putAction(wchar_t c) {
	switch (str.back()) {
		case L'+':
		case L'-':
		case L'*':
		case L'/':
			str.pop_back();
			break;
	}
	str += c;
	isCalculated = false;
}


/* ¬ычисление выражени¤ с учетом приоритета операторов и скобок */
double calculate(std::wstring str) {
	if (str == L"\0") return 0;
	str.erase(std::remove(str.begin(), str.end(), L' '), str.end());		// удал¤ем все пробелы в строке
	std::reverse(str.begin(), str.end());								// "переворачиваем" строку дл¤ иммитации стека

	double result = calc(str);
	if (!str.empty())  // если после вычислени¤ в строке остались символы - exception
		throw L"Incorrect expression format !"s;
	return result;
}

/* ¬ычисление операций с низким преоритетом ( + или - ) */
double calc(std::wstring& str) {
	double result = calc_high_priority(str);
	TCHAR c;

	while (true) {
		c = get_last_char(str);

		switch (c) {
			case L'+':
				str.pop_back();
				result += calc_high_priority(str);
				break;
			case L'-':
				str.pop_back();
				result -= calc_high_priority(str);
				break;
			default:
				return result;
		}
	}
}

/* ¬ычисление операций с высоким приоритетом ( * и / ) */
double calc_high_priority(std::wstring& str) {
	double result = brackets(str);
	double temp;
	TCHAR c;

	while (true) {
		c = get_last_char(str);

		switch (c) {
			case L'*':
				str.pop_back();
				result *= brackets(str);
				break;
			case L'/':
				str.pop_back();
				temp = brackets(str);
				if (temp == 0.0) // деление на ноль - exception
					throw L"Division by zero !!!"s;
				result /= temp;
				break;
			default:
				return result;
		}
	}
}

/* ¬ычисление выражени¤ в скобках */
double brackets(std::wstring& str) {
	double result;
	int sign = 1;		//множитель дл¤ учета знака выражени¤
	TCHAR c = get_last_char(str);


	if (c == L'-') {
		str.pop_back();
		sign = -1;
		c = get_last_char(str);
	}

	if (c == L'(') {
		str.pop_back();
		result = sign * calc(str);

		c = get_last_char(str);

		if (c != L')')
			throw L"Incorrect bracket placing !!!"s; // ) сразу после ( - exception
		str.pop_back();
		return result;
	} else
		return sign * number(str);
}

/* ѕолучение числа из строки */
double number(std::wstring& str) {			// считывает число
	double result = 0.0;
	int sign = 1;
	TCHAR c = get_last_char(str);

	std::wstring n;

	if (c == L'-') {
		str.pop_back();
		sign = -1;
	}

	c = get_last_char(str);
	while ((c >= L'0' && c <= L'9') || (c >= L'A' && c <= L'F') ||
		(c >= L'a' && c <= L'f') || (c == L'.') || (c == L'x')) {
		str.pop_back();
		n += c;
		c = get_last_char(str);
	}
	std::size_t pos = 1;

	if (n.length())
		result = (n.find(L'.') == std::wstring::npos) ? std::stoi(n, &pos, 0) : std::stod(n, &pos);

	if ((pos != n.length()) || (!n.length()))
		throw L"Incorrect format of number !!!"s;

	return sign * result;
}

/* ¬озвращает последний символ из строки либо '\0', если строка пуста¤ */
TCHAR get_last_char(std::wstring& str) {
	return str.empty() ? L'\0' : str.back();
}