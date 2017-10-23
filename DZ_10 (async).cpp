/*
Напишите параллельный алгоритм вычисления суммы всех площадей многогранников.
Площадь многоугольника - замкнутой ломаной без самопересечений, заданной своими вершинами в порядке обхода,
вычисляется по формуле из статьи http://algolist.manual.ru/maths/geom/polygon/area.php

Используйте не менее 1000 вершин и не менее 1000 многогранников.
Посчитайте и выведете время выполнения для разных типов запуска задач (std::launch::async, std::launch::deferred и по умолчанию).
Не забудьте написать проверочный код.
*/

#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <future>
#include <chrono>
#include <cstdlib>

enum {
	max_coordinate = 100,
	polygons_count = 1000,
	points_count = 100,
};

// Точка
class Point {
	double x, y;
public:
	Point(double X = 0, double Y = 0) {
		x = X;
		y = Y;
	}
	double get_x() const { return x; }
	double get_y() const { return y; }

	void set_x(double X) { x = X; }
	void set_y(double Y) { y = Y; }

	Point& operator = (Point& p) {
		x = p.get_x();
		y = p.get_y();
		return *this;
	}
};
inline bool operator==(const Point& lhs, const Point& rhs) {
	return (lhs.get_x() == rhs.get_x()) && (lhs.get_y() == rhs.get_y());
}
inline bool operator!=(const Point& lhs, const Point& rhs) { return !operator==(lhs, rhs); }

std::ostream& operator<<(std::ostream & os, const Point & point) {
	os << point.get_x() << "\t" << point.get_y();
	return os;
}

/* Многоугольник */
class Polygon {
	std::list<Point> points;

	/* Расстояние между двумя точками */
	double distance(const Point& p1, const Point& p2) const {
		return sqrt(pow(p2.get_x() - p1.get_x(), 2) + pow(p2.get_y() - p1.get_y(), 2));
	}
	/* Проверка на пересечение с гранями многоугольника */
	bool is_intersection(const Point& a1, const Point& a2) {
		double v1, v2, v3, v4;
		for (auto b2 = points.begin(); b2 != points.end(); ++b2) {
			auto b1 = b2;
			b1 = (b2 == points.begin()) ? points.end() : b2;
			std::advance(b1, -1);

			v1 = ((*b2).get_x() - (*b1).get_x())*(a1.get_y() - (*b1).get_y()) - ((*b2).get_y() - (*b1).get_y())*(a1.get_x() - (*b1).get_x());
			v2 = ((*b2).get_x() - (*b1).get_x())*(a2.get_y() - (*b1).get_y()) - ((*b2).get_y() - (*b1).get_y())*(a2.get_x() - (*b1).get_x());
			v3 = (a2.get_x() - a1.get_x())*((*b1).get_y() - a1.get_y()) - (a2.get_y() - a1.get_y())*((*b1).get_x() - a1.get_x());
			v4 = (a2.get_x() - a1.get_x())*((*b2).get_y() - a1.get_y()) - (a2.get_y() - a1.get_y())*((*b2).get_x() - a1.get_x());

			if ((v1*v2 < 0) && (v3*v4 < 0))
				return true;
		}
		return false;
	}
public:
	/* Возвращает список вершин многоугольника */
	std::list<Point>& get_points() { return points; }

	/* Добавление вершины в многоугольник (с учетом недопущения пересечения сторон) */
	void add_point(const Point& point) {
		if (points.size() >= 3) {
			double min_distance;						// Наименьшее расстояние до одной из вершин многоугольника
			double distance_to_point;					// Расстояние до вершины многоугольника
			std::list<Point>::iterator nearest_point;	// Ближайшая вершина
			std::list<Point>::iterator previous_point;	// Вершина перед ближайшей
			std::list<Point>::iterator next_point;		// Вершина после ближайшей
			std::vector<Point> improper_points;			// Неподходящие вершины
			bool is_nearest_occupied, is_previous_occupied, is_next_occupied; // на пути к ближайшей/предыдущей/следующей точкам есть пересеченя 

			improper_points.clear();

			/* Нахождение ближайшей вершины к добавляемой */
			while (true) {
				min_distance = max_coordinate;
				nearest_point = points.begin();
				for (auto p = points.begin(); p != points.end(); ++p) {
					if (std::find(improper_points.begin(), improper_points.end(), *p) != improper_points.end())
						continue;

					distance_to_point = distance(*p, point);
					if (distance_to_point <= min_distance) {
						min_distance = distance_to_point;
						nearest_point = p;
					}
				}

				is_nearest_occupied = is_intersection(point, *nearest_point);
				if (is_nearest_occupied) {
					improper_points.push_back(*nearest_point);
					continue;
				}

				if (nearest_point == points.begin()) {
					previous_point = points.end();
					std::advance(previous_point, -1);
					next_point = points.begin();
					std::advance(next_point, 1);
				} else if (nearest_point == --points.end()) {
					previous_point = points.end();
					std::advance(previous_point, -2);
					next_point = points.begin();
				} else {
					previous_point = next_point = nearest_point;
					std::advance(previous_point, -1);
					std::advance(next_point, 1);
				}

				is_previous_occupied = is_intersection(point, *previous_point);
				is_next_occupied = is_intersection(point, *next_point);

				if (is_previous_occupied && is_next_occupied) {
					improper_points.insert(improper_points.end(), { *nearest_point, *previous_point,*next_point });
					continue;
				}
				break;
			}

			// Добавление вершины в список 
			if (is_next_occupied || (!is_previous_occupied && (distance(point, *previous_point) + distance(*nearest_point, *next_point)) < (distance(*nearest_point, *previous_point) + distance(point, *next_point))))
				(nearest_point != points.begin()) ? points.insert(nearest_point, point) : points.push_back(point);	// вставка точки перед ближайшей
			else
				(next_point != points.begin()) ? points.insert(next_point, point) : points.push_back(point);// вставка точки после ближайшей
		} else
			points.push_back(point);

		points.unique();
	}
};

std::ostream& operator<<(std::ostream & os, Polygon & poly) {
	for (auto p : poly.get_points())
		os << p << "\n";
	os << poly.get_points().front() << "\n";
	return os;
}

/* Вычисление площади многоугольника */
double area(Polygon& polygon) {
	double rezult = 0;
	auto a = polygon.get_points().begin();
	auto b = polygon.get_points().end();

	for (auto& i = a; i != b; ++i) {
		auto j = i;
		j = (i == a) ? b : i;
		std::advance(j, -1);

		rezult += (i->get_x() + j->get_x())*(i->get_y() + j->get_y());
	}
	return std::abs(rezult) / 2;
}

/* Генерирование случайного многоугольника */
Polygon make_random_polygon(int points_count) {
	std::srand(std::time(0));
	double x, y;
	auto polygon = std::make_shared<Polygon>();
	for (int i = 0; i < points_count; ++i) {
		x = std::rand() % max_coordinate;
		y = std::rand() % max_coordinate;
		polygon->add_point(Point{ x,y });
	}
	return *polygon;
}

/* Генерирование вектора многоугольников */
std::vector<std::shared_ptr<std::shared_future<Polygon>>> get_polygons(int polygons_count, int points_count, std::launch policy = std::launch::async | std::launch::deferred) {
	std::vector<std::shared_ptr<std::shared_future<Polygon>>> polygons;

	for (int i = 0; i < polygons_count; ++i)
		polygons.push_back(std::make_shared<std::shared_future<Polygon>>(std::async(policy, make_random_polygon, points_count)));

	return polygons;
}

/* Нахождение плодащей многоугольников */
std::vector<std::shared_future<double>> areas_calculate(std::vector<std::shared_ptr<std::shared_future<Polygon>>> poly, std::launch policy = std::launch::async | std::launch::deferred) {
	std::vector<std::shared_future<double>> v_fut;
	for (auto& i : poly)
		v_fut.push_back(std::async(policy, area, i->get()));

	return v_fut;
}

/* Вычисление суммы площадей многоугольников */
double areas_summ(std::vector<std::shared_future<double>> areas, std::launch policy = std::launch::async | std::launch::deferred) {
	double summ_areas = 0;
	for (auto& i : areas) {
		summ_areas += i.get();
	}
	return summ_areas;
}

void calculate(int polygons_count, int points_count, std::launch policy = std::launch::async | std::launch::deferred) {
	auto start_time = std::chrono::high_resolution_clock::now();
	auto polygons = get_polygons(polygons_count, points_count, policy);
	auto areas = areas_calculate(polygons, policy);
	std::cout << "\tAreas summ: " << areas_summ(areas, policy) << "\n";
	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> fp_s = end_time - start_time;
	std::cout << "\tDuration: " << fp_s.count() << " sec \n\n";
}

void main() {
	//freopen("Polygons_out.txt", "w", stdout);

	std::cout << "Calculate summ areas from " << polygons_count << " polygons whith " << points_count << " random points:\n";

	std::cout << "Async:\n";
	calculate(polygons_count, points_count, std::launch::async);

	std::cout << "Deferred:\n";
	calculate(polygons_count, points_count, std::launch::deferred);

	std::cout << "Default:\n";
	calculate(polygons_count, points_count);
}