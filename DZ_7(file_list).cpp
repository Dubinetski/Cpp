/*
Написать функцию, которая сканирует заданный каталог и сохраняет информацию о всех файлах и папках по возрастанию размера в текстовый файл.
Формат записи в файле: <размер> : <путь_к_файлу_или_папке>
Попробуйте использовать std::filesystem (по возможности).
Напишите код, который будет проверять правильность работы вашего кода.
*/

/* Реализовано без рекурсии за счет ее имитации стеком */

#include <stack>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>

namespace fs = std::experimental::filesystem;

class file {
	std::string path;		// путь к файлу
	int size;				// размер
	int level;				// уровень вложенности по отношению к заданной папке
public:
	file(std::string _path = "", int _size = 0, int _level = 0) {
		path = _path;
		size = _size;
		level = _level;
	}
	int get_size() { return size; }
	void set_size(int _size) { size = _size; }

	std::string get_path() { return path; }
	std::string set_path(std::string _path) { path = _path; }

	int get_level() { return level; }
	void set_level(int _level) { level = _level; }
};

void main(int argc, char* argv[]) {
	char* root_folder = (argc > 1) ? argv[1] : "..";
	setlocale(0, "rus");
	freopen("file_list.txt", "w", stdout);

	std::stack<file> folders_stack;						// стек папок
	file folder(root_folder);							// корневая директория
	folders_stack.push(folder);							//добавляем корневую директорию в стек

	std::vector<file> files;							// все найденные файлы и папки (обработанные)
	std::vector<int> folders_size;						// для хранения размеров верхних уровней папок

	while (!folders_stack.empty()) {					// пока стек не пуст
		folder = folders_stack.top();					// берем верхнюю папку со стека

		if ((folders_size.size() - 1) == folder.get_level()) {	// равенство может быть только если в данную папку уже заходили
			folder.set_size(folders_size.back());		// устанавливаем размер текущей папки равной последнему элементу в массиве размеров папок
			folders_size.pop_back();					// удаляем последний элемент в массиве размеров папок

			if (folder.get_level())						// если папка не корневая
				folders_size[folder.get_level() - 1] += folder.get_size();	// прибавляем размер текущей паки к размеру родительской

			files.push_back(folder);					// добавляем текущую папку в список обработанных папок
			folders_stack.pop();						// удаляем папку из стека
			continue;									// переходим на следующий круг цикла
		}
		folders_size.push_back(0);						// добавляем новый уровень (размер текущей папки) в массив размеров

		for (auto& i : fs::directory_iterator(folder.get_path())) {		// проходим по всем объектам (файлам и папкам) внутри данной (без захода во вложенные)
			file current_file(i.path().string(), 0, folder.get_level() + 1);	// под каждый найденный элемент создаем новый объект "file" с размером 0 и уровнем на 1 больше уровня текущей папки
			if (fs::is_directory(i)) {					// если объект является директорией
				folders_stack.push(current_file);		// добавляем в стек
			} else {									// если файл
				current_file.set_size(fs::file_size(i));//находим размер
				folders_size[folder.get_level()] += current_file.get_size();	// увеличиваем размер родительской паки на размер файла
				files.push_back(current_file);			// добавляем файл в массив обработанных файлов
			}
		}
	}
	struct {
		bool operator()(file file1, file file2) const {
			return file1.get_size() > file2.get_size();			// критерий сортировки файлов
		}
	} descending;

	std::sort(files.begin(), files.end(), descending);

	for (auto& i : files) {
		std::cout << std::setw(std::to_string(files[0].get_size()).length()) << std::left << i.get_size() << '\t';		// вывод размера файла с форматированием по самому "длинному размеру"
		std::cout << i.get_path() << '\n';
	}
}