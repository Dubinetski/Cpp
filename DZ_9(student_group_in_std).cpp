/* 
Реализовать хранение информации о студентах и группах используя ТОЛЬКО стандартные типы и алгоритмы.
Разрешено использовать любые простые типы (int, double и др.) а также любые классы из std::.
Реализовать функции вывода в консоль, сортировки и поиска студентов по имени, по фамилии, по возрасту.
Написать проверочный код.
*/

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include <string>

enum fild_student {
	surname = 0,
	name,
	patronymic,
	age
};

typedef std::tuple<std::string, std::string, std::string, int> student;

/* Вывод информации о студенте */
std::ostream& operator<<(std::ostream & os, student& stud) {
	os << std::get<surname>(stud).c_str() << '\t';
	os << std::get<name>(stud).c_str() << '\t';
	os << std::get<patronymic>(stud).c_str() << '\t';
	os << std::get<age>(stud);
	return os;
}

/* Вывод списка студентов группы */
std::ostream& operator<<(std::ostream & os, std::vector<student>& group) {
	for (auto& i : group)
		os << i << '\n';
	return os;
}

bool by_surname(student st1, student st2) {				// сравнение двух фамилий студентов
	return std::get<surname>(st1) < std::get<surname>(st2);
}
bool by_name(student st1, student st2) {				// сравнение двух имен студентов
	return std::get<name>(st1) < std::get<name>(st2);
}
bool by_patronymic(student st1, student st2) {			// сравнение двух отчеств студентов
	return std::get<patronymic>(st1) < std::get<patronymic>(st2);
}
bool by_age(student st1, student st2) {					// сравнение возраста двух студентов
	return std::get<age>(st1) < std::get<age>(st2);
}
/* Поиск студента по маске */
std::vector<student> find_students(std::vector<student>const& group, int const fild_number, std::string const &mask) {
	std::string fild_value = "";
	std::shared_ptr<std::vector<student>> rezult_vector (new std::vector<student>);
	for (auto& i : group) {
		switch (fild_number) {
		case surname:
			fild_value = std::get<surname>(i);
			break;
		case name:
			fild_value = std::get<name>(i);
			break;
		case patronymic:
			fild_value = std::get<patronymic>(i);
			break;
		default:
			break;
		}
		if (fild_value.substr(0, mask.length()) == mask)
			rezult_vector->push_back(i);
	}
	return *rezult_vector;
}
std::vector<student> find_students(std::vector<student>const& group, int const fild_number, int const &mask) {
	std::shared_ptr<std::vector<student>> rezult_vector(new std::vector<student>);
	for (auto& i : group) {
		switch (fild_number) {
		case age:
			if (std::get<age>(i) == mask)
				rezult_vector->push_back(i);
			break;
		default:
			*rezult_vector = find_students(group, fild_number, std::to_string(mask));
			break;
		}
	}
	return *rezult_vector;
}

int main() {
	std::unique_ptr<std::vector<student>> student_group(new std::vector<student>);

	student_group->push_back(std::make_tuple("Ivanov", "Ivan", "Ivanovich", 25));
	student_group->push_back(std::make_tuple("Petrov", "Petr", "Petrovich", 22));
	student_group->push_back(std::make_tuple("Sidorov", "Sidor", "Sidorovich", 27));
	student_group->push_back(std::make_tuple("Chapaev", "Vasiliy", "Ivanovich", 30));
	student_group->push_back(std::make_tuple("Isaev", "Petr", "Semenovich", 27));

	std::cout << "Print group: \n";
	std::cout << *student_group << '\n';

	std::cout << "Print student: \n";
	std::cout << (*student_group)[0] << "\n\n";

	std::sort(student_group->begin(), student_group->end(), by_surname);
	std::cout << "Sort group by surname: \n";
	std::cout << *student_group << '\n';

	std::sort(student_group->begin(), student_group->end(), by_name);
	std::cout << "Sort group by name: \n";
	std::cout << *student_group << '\n';

	std::sort(student_group->begin(), student_group->end(), by_patronymic);
	std::cout << "Sort group by patronymic: \n";
	std::cout << *student_group << '\n';

	std::sort(student_group->begin(), student_group->end(), by_age);
	std::cout << "Sort group by age: \n";
	std::cout << *student_group << '\n';

	std::cout << "Find student with filtr \"I\" by surname: \n";
	std::cout << find_students(*student_group, surname, "I") << '\n';

	std::cout << "Find student with filtr \"P\" by name: \n";
	std::cout << find_students(*student_group, name, "P") << '\n';

	std::cout << "Find student with filtr \"S\" by patronymic: \n";
	std::cout << find_students(*student_group, patronymic, "S") << '\n';

	std::cout << "Find student with age 30: \n";
	std::cout << find_students(*student_group, age, 30) << '\n';
}