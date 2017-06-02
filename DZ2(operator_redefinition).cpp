/*
Написать класс для хранения времени Time, который должен:
1) содержать конструктор по умолчанию, конструктор копирования, конструктор перемещения, конструктор принимающий минуты и другие (подумайте сами какие);
объявите некоторые конструкторы как explicit;
2) содержать методы получения часов, минут и пр.
3) перегружать операторы +, -, +=, -=, ++, --, =.
4) поддерживать внешние операторы ввода и вывода (<<, >>) в 12-часовой формате времени HH:mm AM/PM.

Напишите код, который будет проверять правильность работы вашего класса.
*/

#include <iostream>
#include <iomanip>
#include <string>

class Time {
	private:

	int seconds;
	int minutes;
	int hours;

	// Инициализирующая функция для исключеия дублирования кода в конструкторах
	void initial(int sec) {
		hours = sec / 3600;
		minutes = sec / 60 - hours * 60;
		seconds = sec - minutes * 60 - hours * 3600;
	}
	public:

	// конструктор по умолчанию
	Time() { initial(0); }

	// конструктор принимающий секунды
	explicit Time(int sec) { initial(sec); }

	// конструктор принимающий минуты и секунды
	Time(int minutes, int seconds) { initial(minutes * 60 + seconds); };

	// конструктор принимающий часы, минуты и секунды
	Time(int hours, int minutes, int seconds) { initial(hours * 3600 + minutes * 60 + seconds); }

	// конструктор копирования
	Time(Time& t) {
		seconds = t.get_seconds();
		minutes = t.get_minutes();
		hours = t.get_hours();
	};
	// конструктор перемещения
	Time(Time&& t) {
		seconds = t.get_seconds();
		minutes = t.get_minutes();
		hours = t.get_hours();
		std::cout << "Move\n";
	};

	int get_seconds() const { return seconds; }
	int get_minutes() const { return minutes; }
	int get_hours() const { return hours; }

	int get_time_in_seconds() const { return hours * 3600 + minutes * 60 + seconds; }
	int get_time_in_minutes() const { return hours * 60 + minutes; }

	void set_seconds(int seconds) { initial(hours * 3600 + minutes * 60 + seconds); }
	void set_minutes(int minutes) { initial(hours * 3600 + minutes * 60 + seconds); }
	void set_hours(int hours) { initial(hours * 3600 + minutes * 60 + seconds); }

	void set_time(int hours = 0, int minutes = 0, int seconds = 0) {
		initial(hours * 3600 + minutes * 60 + seconds);
	}
	void set_time_in_seconds(int seconds) { initial(seconds); }
	void set_time_in_minutes(int minutes) { initial(minutes * 60); }

	void add_seconds(int seconds) { initial(get_time_in_seconds() + seconds); }
	void add_minutes(int minutes) { initial(get_time_in_seconds() + minutes * 60); }
	void add_hours(int hours) { initial(get_time_in_seconds() + hours * 3600); }

	// перегрузка оператора =
	Time& operator=(Time& t) {
		initial(t.get_time_in_seconds());
		return *this;
	}
	// перегрузка оператора =
	Time& operator=(int seconds) {
		initial(seconds);
		return *this;
	}

	// перегрузка оператора +
	Time& operator+(int i) {
		initial(get_time_in_seconds() + i);
		return *this;
	}
	// перегрузка оператора -
	Time& operator-(int i) {
		initial(get_time_in_seconds() - i);
		return *this;
	}
	// перегрузка оператора +=
	Time& operator+=(int i) {
		initial(get_time_in_seconds() + i);
		return *this;
	}
	// перегрузка оператора -=
	Time& operator-=(int i) {
		initial(get_time_in_seconds() - i);
		return *this;
	}
	// перегрузка оператора ++ (префиксальный)
	Time& operator++() {
		initial(get_time_in_seconds() + 1);
		return *this;
	}
	// перегрузка оператора ++ (постфиксальный)
	Time operator++(int) {
		Time tmp(get_time_in_seconds());
		initial(get_time_in_seconds() + 1);
		return tmp;
	}
	// перегрузка оператора -- (префиксальный)
	Time& operator--() {
		initial(get_time_in_seconds() - 1);
		return *this;
	}
	// перегрузка оператора -- (постфиксальный)
	Time operator--(int) {
		Time tmp(get_time_in_seconds());
		initial(get_time_in_seconds() - 1);
		return tmp;
	}
};
// переопределение оператора вывода << (формат HH:mm AM/PM)
std::ostream& operator<<(std::ostream & os, const Time & t) {
	os << std::setw(2) << std::setfill('0') << ((t.get_hours() < 12) ? t.get_hours() : t.get_hours() - 12);
	os << ':' << std::setw(2) << std::setfill('0') << t.get_minutes();
	os << ':' << std::setw(2) << std::setfill('0') << t.get_seconds();
	os << " " << ((t.get_hours() < 12) ? "AM" : "PM");
	return os;
}
// переопределение оператора вывода << (формат HH:mm AM/PM)
std::istream& operator>>(std::istream& is, Time& t) {
	std::string seconds;
	std::cout << "Enter time in seconds: ";
	std::cin >> seconds;
	t.set_time_in_seconds(atoi(seconds.c_str()));
	return is;
}

void main() {

	std::cout << "\n***   default designer and Operator << rezult: 00:00:00 AM   ***\n";
	Time t1;
	std::cout << t1 << "\n";

	std::cout << "\n***   designer getting seconds   t(5555) rezult: 01:32:35 AM   ***\n";
	Time t2(5555);
	std::cout << t2 << "\n";

	std::cout << "\n***   designer getting minutes and seconds  t(55, 30) rezult: 00:55:30 AM   ***\n";
	Time t3(55, 30);
	std::cout << t3 << "\n";

	std::cout << "\n***   designer getting hours, minutes and seconds  t(5, 12, 2)\n";
	std::cout << "(rezult: 05:12:02 AM)   ***\n";
	Time t4(5, 12, 2);
	std::cout << t4 << "\n";

	std::cout << "\n***   coping designer Time(Time& t) rezult: 05:12:02 AM   ***\n";
	Time t5(t4);
	std::cout << t5 << "\n";

	std::cout << "\n***   move designer Time(Time&& t) rezult: 00:02:03 AM   ***\n";
	Time t6(std::move(Time(123)));
	std::cout << t6 << "\n";

	std::cout << "\n***   Operator = 35000 (receive seconds) rezult: 09:43:20 AM  ***\n";
	t1 = 35000;
	std::cout << t1 << "\n";

	std::cout << "\n***   Operator + 12000 (Operator= receive object) rezult: 01:03:20 PM  ***\n";
	t1 = t1 + 12000;
	std::cout << t1 << "\n";

	std::cout << "\n***   Operator += 321  (rezult: 01:08:41 PM)  ***\n";
	t1 += 321;
	std::cout << t1 << "\n";

	std::cout << "\n***   Operator - 7000  (rezult: 11:12:01 AM)  ***\n";
	t1 = t1 - 7000;
	std::cout << t1 << "\n";

	std::cout << "\n***   Operator -= 1234  (rezult: 10:51:27 AM)  ***\n";
	t1 -= 1234;
	std::cout << t1 << "\n";

	std::cout << "\n***   Operator ++ (prefics) (rezult: 10:51:28 AM)  ***\n";
	std::cout << ++t1 << "\n";

	std::cout << "\n***   Operator ++ (postfics) (rezult: 10:51:28 AM)  ***\n";
	std::cout << "actuate move designer \n";
	std::cout << t1++ << "\n";
	std::cout << "after (rezult: 10:51:29 AM): " << t1 << "\n";

	std::cout << "\n***   Operator -- (prefics) (rezult: 10:51:28 AM)  ***\n";
	std::cout << --t1 << "\n";

	std::cout << "\n***   Operator -- (postfics) (rezult: 10:51:28 AM)  ***\n";
	std::cout << t1-- << "\n";
	std::cout << "after (rezult: 10:51:27 AM): " << t1 << "\n";

	std::cout << "\n***  get_hours() (rezult: 10)  ***\n";
	std::cout << t1.get_hours() << "\n";

	std::cout << "\n***  get_minutes() (rezult: 51)  ***\n";
	std::cout << t1.get_minutes() << "\n";

	std::cout << "\n***  get_seconds() (rezult: 27)  ***\n";
	std::cout << t1.get_seconds() << "\n";

	std::cout << "\n***  get_time_in_seconds() (rezult: 39087)  ***\n";
	std::cout << t1.get_time_in_seconds() << "\n";

	std::cout << "\n***  get_time_in_minutes() (rezult: 651)  ***\n";
	std::cout << t1.get_time_in_minutes() << "\n";

	std::cout << "\n***  set_seconds(45) (rezult: 10:51:45 AM)  ***\n";
	t1.set_seconds(45);
	std::cout << t1 << "\n";

	std::cout << "\n***  set_minutes(12) (rezult: 10:12:45 AM)  ***\n";
	t1.set_minutes(12);
	std::cout << t1 << "\n";

	std::cout << "\n***  set_hours(15) (rezult: 03:12:45 PM)  ***\n";
	t1.set_hours(15);
	std::cout << t1 << "\n";

	std::cout << "\n***  set_time(17, 15, 10) (rezult: 05:15:10 PM)  ***\n";
	t1.set_time(17, 15, 10);
	std::cout << t1 << "\n";

	std::cout << "\n***  set_time_in_seconds(9874) (rezult: 02:44:34 AM)  ***\n";
	t1.set_time_in_seconds(9874);
	std::cout << t1 << "\n";

	std::cout << "\n***  set_time_in_minutes(75) (rezult: 01:15:00 AM)  ***\n";
	t1.set_time_in_minutes(75);
	std::cout << t1 << "\n";

	std::cout << "\n***  add_seconds(15) (rezult: 01:15:15 AM)  ***\n";
	t1.add_seconds(15);
	std::cout << t1 << "\n";

	std::cout << "\n***  add_minutes(15) (rezult: 01:30:15 AM)  ***\n";
	t1.add_minutes(15);
	std::cout << t1 << "\n";

	std::cout << "\n***  add_hours(15) (rezult: 04:30:15 PM)  ***\n";
	t1.add_hours(15);
	std::cout << t1 << "\n";

	std::cout << "***   Operator >>   ***\n";
	std::cin >> t1;
	std::cout << t1 << "\n";
}