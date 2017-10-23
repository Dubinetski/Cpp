/*
Напишите функцию, которая будет расчитывать выражение которое будет передаваться в функцию в виде строки.
В случае ошибок функция должна генерировать исключения.
Реализовать поддержку бинарных операций '+', '-', '*', '/' без учета приоритета.
Поддерживать числа в 10тичной, 16ричной и 8ричной сичтеме исчисления (16, 0x10, 020).
Написать не менее 10 вариантов проверочного кода.

Варианты усложнения:
1) поддержка тех же операций но с учетом приоритета
2) поддержка скобок ')', '('
3) построение дерева выражения и поддежка 'x', вычисление произвольной формулы с 'x'
*/

/* Реализован 3й вариант усложнения, но без построения дерева выражения */

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

double calculate(std::string, double);
double number(std::string&, double&);
double calc_high_priority(std::string&, double&);
double calc(std::string&, double&);
double brackets(std::string&, double&);
char get_last_char(std::string&);

void test_calc(std::string str, double x, double correct_result);
void test_calc(std::string str, double correct_result);

using namespace std::string_literals;

void main() {
	freopen("Result.txt", "w", stdout);

	test_calc("12+ 11+ 16*2"s, 55);
	test_calc("12+ 11+ 020*2"s, 55);
	test_calc("12+ 11+ 0x10*2"s, 55);
	test_calc("07+ 3 * 0xF"s, 52);
	test_calc("1.5+ 2.2/2-0.1"s, 2.5);
	test_calc("(x+2) * x"s, 2, 8);
	test_calc("2.5+1.5*3"s, 7);
	test_calc("2+3-(8+4/2)"s, -5);
	test_calc("2+3-(8+4/2))"s, 0);
	test_calc("4/(2-2)"s, 0);
	test_calc("4dfg/(2-2)"s, 0);
}

void test_calc(std::string str, double correct_result) {
	test_calc(str, 0, correct_result);
}
void test_calc(std::string str, double x, double correct_result) {
	std::cout << str << " = ";
	double result;
	try {
		result = calculate(str, x);
		std::cout << result;
		std::cout << "\t\t( " << ((result == correct_result) ? "true" : "!!! FALSE !!!") << " )\n";
	} catch (const std::string exept_str) {
		std::cout << exept_str;
		std::cout << "\t\t( " << ((correct_result == 0) ? "true" : "!!! FALSE !!!") << " )\n";
	}
}

/* Возвращает последний символ из строки либо '\0', если строка пустая */
char get_last_char(std::string& str) {
	return str.empty() ? '\0' : str.back();
}

/* Вычисление выражения с учетом приоритета операторов и скобок */
double calculate(std::string str, double x = 0) {
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());		// удаляем все пробелы в строке
	std::reverse(str.begin(), str.end());								// "переворачиваем" строку для иммитации стека
	double result = calc(str, x);
	if (!str.empty())  // если после вычисления в строке остались символы - exception
		throw "Incorrect expression format !"s;
	return result;
}

/* Вычисление операций с низким преоритетом ( + или - ) */
double calc(std::string& str, double& x) {
	double result = calc_high_priority(str, x);
	char c;

	while (true) {
		c = get_last_char(str);

		switch (c) {
		case '+':
			str.pop_back();
			result += calc_high_priority(str, x);
			break;
		case '-':
			str.pop_back();
			result -= calc_high_priority(str, x);
			break;
		default:
			return result;
		}
	}
}

/* Вычисление операций с высоким приоритетом ( * и / ) */
double calc_high_priority(std::string& str, double& x) {
	double result = brackets(str, x);
	double temp;
	char c;

	while (true) {
		c = get_last_char(str);

		switch (c) {
		case '*':
			str.pop_back();
			result *= brackets(str, x);
			break;
		case '/':
			str.pop_back();
			temp = brackets(str, x);
			if (temp == 0.0) // деление на ноль - exception
				throw "Division by zero !!!"s;
			result /= temp;
			break;
		default:
			return result;
		}
	}
}

/* Вычисление выражения в скобках */
double brackets(std::string& str, double& x) {
	double result;
	int sign = 1;		//множитель для учета знака выражения
	char c = get_last_char(str);


	if (c == '-') {
		str.pop_back();
		sign = -1;
		c = get_last_char(str);
	}

	if (c == '(') {
		str.pop_back();
		result = sign * calc(str, x);

		c = get_last_char(str);

		if (c != ')')
			throw "Incorrect bracket placing !!!"s; // ) сразу после ( - exception
		str.pop_back();
		return result;
	} else
		return sign * number(str, x);
}

/* Получение числа из строки */
double number(std::string& str, double& x) {			// считывает число
	double result = 0.0;
	int sign = 1;
	char c = get_last_char(str);

	std::string n;

	if (c == '-') {
		str.pop_back();
		sign = -1;
	}

	c = get_last_char(str);
	while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
		(c >= 'a' && c <= 'f') || (c == '.') || (c == 'x')) {
		str.pop_back();
		n += c;
		c = get_last_char(str);
	}
	std::size_t pos = 1; // если первый символ 'x', иначе будет присвоено значение при вызове std::stod()

	if (n.front() == 'x')
		result = x;
	else
		result = (n.find('.') == std::string::npos) ? std::stoi(n, &pos, 0) : std::stod(n, &pos);

	if (pos != n.length())
		while (pos != n.length()) {
			if (n[pos] == 'x') {
				pos += 1;
				result *= x;
			} else // если после после преобразования строки в число остались символы, но это не 'x' - exception
				throw "Incorrect format of number !!!"s;
		}
	return sign * result;
}