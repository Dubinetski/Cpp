/* Напишите удаление всех элементов из std::map<int, double>, где значения меньше чем 0,01.
Реализовать нужно за один проход по контейнеру. */

#include <map>
#include <iostream>
#include <ctime>

void main() {
	std::map<int, double> my_map;
	typedef std::pair <int, double> d_pair;
	double min_value = 0.01;				// минимальное значение элемента

	/* Заполнение std::map<int, double> случайными элементами */
	srand(time(0));
	for (int i = 0; i < 10; ++i)
		my_map.insert(d_pair(i, double(rand() % 200 - 100) / 1000));

	/* Вывод элементов до удаления */
	std::cout << "Initial Map elements: \n";
	for (auto i : my_map)
		std::cout << i.first << "\t" << i.second << '\n';

	/* Удаление всех элементов std::map со значениями меньшими 0,01 (за один проход цикла) */
	for (auto i = my_map.begin(); i != my_map.end();)
		if (i->second < min_value)
			i = my_map.erase(i);
		else
			++i;
		
	/* Вывод элементов после удаления */
	std::cout << "\n\nMap after deleted elements with value less then 0,01: \n";
	for (auto i : my_map) {
		std::cout << i.first << "\t" << i.second << '\n';
	}
}