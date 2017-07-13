/*
Задача:
Написать функцию с переменным числом параметров,
которая считает значение полинома вида : c0 + c1*x + c2*x ^ 2 + c3*x ^ 3 + ...
Напишите код, который будет проверять правильность работы вашего кода.
*/

/*
Пояснения:
Преобразовав полином к виду: ((c3*x+c2)x+c1)x+c0
результат можно представить в общем виде (summ*x+c),
где summ - результат вычисления на предыдущей итерации
*/

#include <iostream>

/* Нахождение значения полинома вида c0 + c1*x + c2*x ^ 2 + c3*x ^ 3 + ...
Входные данные: (Х, c1, с2, с3 и т.д.) */
template<typename T, typename... Args>
double poly(const double& x, const T& c, const Args&... args) { return poly(x, args...)*x + c; }
double poly(const double& x) { return 0; }

void main() {
	double rezult = poly(3, 1, 2);
	std::cout << "1+2*(3)=7: rezult= " << rezult;
	std::cout << "\t\t\t\t\t(" << (rezult == 7 ? "true" : "false") << ")\n";

	rezult = poly(3, 1, 2, 3);
	std::cout << "1+2*(3)+3*(3^2)=34: rezult= " << rezult;
	std::cout << "\t\t\t\t(" << (rezult == 34 ? "true" : "false") << ")\n";

	rezult = poly(5, 1, 2, 3, 4);
	std::cout << "1+2*(5)+3*(5^2)+4*(5^3)=586: rezult= " << rezult;
	std::cout << "\t\t(" << (rezult == 586 ? "true" : "false") << ")\n";

	rezult = poly(2, 1, 2, 3, 4, 5);
	std::cout << "1+2*(2)+3*(2^2)+4*(2^3)+5*(2^4)=129: rezult= " << rezult;
	std::cout << "\t(" << (rezult == 129 ? "true" : "false") << ")\n";

	rezult = poly(4, 5, 0, 0, 2, 0, 0, -8);
	std::cout << "5+2*(4^3)-8(4^6)=-32635: rezult= " << rezult;
	std::cout << "  \t\t(" << (rezult == -32635 ? "true" : "false") << ")\n";

	rezult = poly(0, 5, 0, 0, 2, 0, 0, -8);
	std::cout << "5+2*(0^3)-8(0^6)=5: rezult= " << rezult;
	std::cout << "\t\t\t\t(" << (rezult == 5 ? "true" : "false") << ")\n";

	rezult = poly(2.3, 2.5, 3.1, 4.8);
	std::cout << "2.5+3.1*(2.3)+4.8(2.3^2)=35.022: rezult= " << rezult;
	std::cout << "\t\t(" << (rezult > 35.021 && rezult < 35.023 ? "true" : "false") << ")\n";
}