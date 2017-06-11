/*
Написать класс описывающий многоугольник, который должен:
1) хранить произвольное кол-во вершин (реализовать возможность удаления и добавления вершин);
2) вершины должны определяться 2-мя координатами X и Y.
3) перегружать операторы ==, !=; operator bool(), проверяющий что многоугольник не пуст.
4) содержать мотод перемещения многоугольник на плоскости на заданное число(X, Y).
5) содержать методы кторые подсчитывают длину всех сторон, среднюю длину стороны, проверяют является ли многоугольник правильным.
6) поддерживать внешние операторы вывода (<<) с информацией о многоуголнике (кол-во углов, является ли правильным и пр).

Напишите код, который будет проверять правильность работы ваших классов.
*/

#include <iostream>
#include <list>
#include <cmath>

// Точка
class Point {
	double x;		// координата х
	double y;		// координата y

	public:
	/* Конструктор по умолчанию */
	Point() {
		x = 0;
		y = 0;
	}

	/* Конструктор принимающий координаты (x, y) точки */
	Point(double X, double Y) {
		x = X;
		y = Y;
	}

	/* Конструктор копирования */
	Point(const Point& point) {
		x = point.get_x();
		y = point.get_y();
	}

	double get_x() const { return x; }
	double get_y() const { return y; }

	void set_x(double X) { x = X; }
	void set_y(double Y) { y = Y; }

	/* Сместить точку по оси X на dx */
	void move_x(double dx) { x += dx; }
	/* Сместить точку по оси Y на dy */
	void move_y(double dy) { x += dy; }
	/* Сместить точку dx по оси Х и на dy по оси Y */
	void move(double dx, double dy) { x += dx; y += dy; }

	/* Переопределение оператора "=" */
	Point& operator = (Point& p) {
		x = p.get_x();
		y = p.get_y();
		return *this;
	}
};
/* Переопределение оператора "==" для точки*/
inline bool operator==(const Point& lhs, const Point& rhs) {
	return (lhs.get_x() == rhs.get_x()) && (lhs.get_y() == rhs.get_y());
}
/* Переопределение оператора "!=" для точки*/
inline bool operator!=(const Point& lhs, const Point& rhs) { return !operator==(lhs, rhs); }

/* Переопределение оператора "<<" для точки*/
std::ostream& operator<<(std::ostream & os, const Point & point) {
	os << '(' << point.get_x() << ", " << point.get_y() << ')';
	return os;
}
/* Многоугольник */
class Polygon {
	/* Список вершин многоугольника */
	std::list<Point> points;

	/* Расстояние между двумя точками */
	double distance(const Point& p1, const Point& p2) const {
		return sqrt(pow(p2.get_x() - p1.get_x(), 2) + pow(p2.get_y() - p1.get_y(), 2));
	}

	/* Инициализация многоугольника состоящего из произвольного числа точек */
	template<typename T, typename... Args>
	void init(const T& point, const Args&... args) {
		add_point(point);
		init(args...);
	}

	void init() {}

	public:
	/* Конструктор по умолчанию */
	Polygon() {}
	/* Конструктор принимающий произвольное колличество аргументов*/
	template<typename... Args>
	Polygon(const Args&... args) {
		init(args...);
	}
	/* Возвращает колличество вершин многоугольника */
	int get_angle_count() const { return points.size(); }
	/* Возвращает список вершин многоугольника */
	std::list<Point> get_points_list() const { return points; }

	/* Добавление вершины в многоугольник (с учетом недопущения пересечения сторон) */
	void add_point(const Point& point) {
		if (points.size() > 3) {
			double min_distance = distance(points.front(), point); // Наименьшее расстояние до одной из вершин многоугольника
			double distance_to_point;					// Расстояние до вершины многоугольника
			auto point_poz = points.begin();			// Номер текущей вершины
			std::list<Point>::iterator current_point;	// Номер ближайшей вершины

														/* Нахождение ближайшей вершины к добавляемой */
			for (auto p : points) {
				distance_to_point = distance(p, point);
				if (distance_to_point <= min_distance) {
					min_distance = distance_to_point;
					current_point = point_poz;
				}
				++point_poz;
			}
			std::list<Point>::iterator poz_befor, poz_after; // номера вершин до и после ближайшей к добавляемой

			if (current_point == points.begin()) {
				poz_befor = points.end();
				std::advance(poz_befor, -1);
				poz_after = points.begin();
				std::advance(poz_after, 1);
			} else if (current_point == --points.end()) {
				poz_befor = points.end();
				std::advance(poz_befor, -2);
				poz_after = points.begin();
			} else {
				poz_befor = poz_after = current_point;
				std::advance(poz_befor, -1);
				std::advance(poz_after, 1);
			}
			// Добавление вершины в список 
			if (distance(point, *poz_befor) < distance(point, *poz_after))
				points.insert(current_point, point);	// вставка точки перед ближайшей
			else
				points.insert(poz_after, point);		// вставка точки после ближайшей
		} else
			points.push_back(point);
		points.unique();
	}
	/* Удаление вершины */
	void del_point(Point p) {
		points.remove(p);
	}
	/* Смещение вершины по осям X и Y */
	void move(double dx, double dy) {
		for (auto i = points.begin(); i != points.end(); ++i)
			(*i).move(dx, dy);
	}
	/* Периметр многоугольника */
	double perimeter() const {
		double summ = 0;
		Point p1 = points.front();
		for (auto p : points)
			if (p != points.front()) {
				summ += distance(p1, p);
				p1 = p;
			}
		summ += distance(p1, points.front());
		return summ;
	}
	/* Средняя длина стороны */
	double middle_side_length() const {
		return perimeter() / points.size();
	}
	/* Правильный ли многоугольник? (равны стороны и углы => расстояния между вершинами "через одну" */
	bool is_correct() const {
		if (points.size() < 3)
			return false;

		auto p_2 = points.end();		// итератор, указывающий на вершину с номером на 2 меньшим чем у текущей
		std::advance(p_2, -1);			// смещаем итератор на "последнюю" вершину

		auto p_1 = points.begin();		// итератор, указывающий на предыдущую вершину
		std::advance(p_1, 1);			// смещаем итератор на "вторую" вершину

		double side = distance(points.front(), *p_1);	// расстояние от "первой" до "второй" вершины
		double diagonal = distance(*p_2, *p_1);			// расстояние от "последней" до "второй" вершины

		p_2 = points.begin();

		auto p = points.begin();
		std::advance(p, 2);		// перемещаем указатель на третью вершину
		for (p; p != points.end(); ++p, ++p_1, ++p_2)
			if ((distance(*p, *p_1) != side) || (distance(*p, *p_2) != diagonal))
				return false;

		if ((distance(points.front(), *p_1) != side) || (distance(points.front(), *p_2) != diagonal))
			return false;

		return true;
	}
	/* переопределение оператора приведения к bool */
	operator bool() const {
		return points.size() ? true : false;
	}
	/* Вывод списка точек на консоль */
	void print_points() {
		for (auto i : points)
			std::cout << "(" << i.get_x() << ", " << i.get_y() << "), ";
	}
};

/* Переопределение оператора сравнения "==" */
inline bool operator==(const Polygon& lhs, const Polygon& rhs) {
	if (lhs.get_angle_count() != rhs.get_angle_count())
		return false;
	auto left_list_point = lhs.get_points_list().begin();
	auto right_list_point = lhs.get_points_list().begin();

	while (left_list_point != lhs.get_points_list().end())
		if (*(left_list_point++) != *(right_list_point++))
			return false;
	return true;
}

/* Переопределение оператора сравнения "!=" */
inline bool operator!=(const Polygon& lhs, const Polygon& rhs) { return !operator==(lhs, rhs); }

/* переопределение оператора вывода << */
std::ostream& operator<<(std::ostream & os, const Polygon & poly) {
	os << "Poligon have " << poly.get_angle_count() << " angers:\n";
	for (auto p : poly.get_points_list())
		os << p << ", ";
	os << "\nIt's " << (poly.is_correct() ? "correct" : "incorrect") << '\n';
	os << "Perimetr = " << ceil(poly.perimeter() * 100) / 100 << '\n';
	os << "The midle length of side = " << ceil(poly.middle_side_length() * 100) / 100 << '\n';
	return os;
}

void main() {
	std::cout << "*** Create new polygon with points (2.5, 2.5), (2.5, 5), (5, 5), (5, 2.5) ***\n\n";
	Polygon poly(Point{2.5, 2.5}, Point{2.5, 5}, Point{5, 5}, Point{5, 2.5});		// Создание квадрата с длиной стороны 2,5
	std::cout << "Perimeter (correct rezult 10): " << poly.perimeter() << "\t\t\t" << (poly.perimeter() == 10 ? "true" : "ERROR") << '\n';
	std::cout << "The midle length of side (correct rezult 2.5): " << poly.middle_side_length() << '\t' << (poly.middle_side_length() == 2.5 ? "true" : "ERROR") << '\n';
	std::cout << "Check on correct (+ check operator bool() redefinition)\n";
	std::cout << "(correct rezult \"true\" or 1): " << poly.is_correct() << "\t\t\t\t" << (poly ? "true" : "ERROR") << '\n';
	std::cout << "\n*** Operator << ***\n";
	std::cout << poly;

	std::cout << "\n*** Move polygon on dX=0.5, dy=1 *** \n";
	poly.move(0.5, 1);
	std::cout << poly;

	std::cout << "\n*** Add point (2, 4) *** \n";
	poly.add_point(Point{2, 4});
	std::cout << poly;

	std::cout << "\n*** Delete point (2, 4) *** \n";
	poly.del_point(Point{2, 4});
	std::cout << poly;
}