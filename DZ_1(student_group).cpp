/*Написать класс, описывающий группу студентов.
Студент также реализуется с помощью соответствующего класса.

1) Реализовать методы добавления, удаления, получения студентов в группе;
также методы для получени / изменения информации о студентах(getAAA / setAAA).

2) В функции main используя новые классы и методы создать 2 группы студетов,
отсортировать их по имени, вывести в стандартный поток(std::cout) информацию о каждой группе.
*/

#include <iostream>
#include <string>
#include <conio.h>

//Студент
class Student {
	private:
	std::string name;		// Имя
	std::string surname;	// Фамилия
	std::string patronymic;	// Отчество
	int age;				// Возраст

	public:
	Student() {
		surname = "";
		name = "";
		patronymic = "";
		age = 0;
	}

	Student(std::string surname, std::string name, std::string patronymic, int age) {
		this->surname= surname;
		this->name= name;
		this->patronymic = patronymic;
		this->age = age;
	}

	Student(Student& student) {
		name = student.name;
		surname = student.surname;
		patronymic = student.patronymic;
		age = student.age;
	}
	
	std::string getSurname() { return surname; }
	void setSurname(std::string surname) { this->surname= surname; }

	std::string getName() { return name; }
	void setName(std::string name) { this->name= name; }

	std::string getPatronymic() { return patronymic; }
	void setPatronymic(std::string patronymic) { this->patronymic= patronymic; }

	int getAge() { return age; }
	void setAge(int age) { this->age = age; }

	/*Изменить информацию о студенте*/
	void changeInfo(std::string surname, std::string name, std::string patronymic, int age) {
		this->surname = surname;
		this->name = name;
		this->patronymic = patronymic;
		this->age = age;
	}

	Student& operator=(const Student& cpyStudent) {
		surname = cpyStudent.surname;
		name = cpyStudent.name;
		patronymic = cpyStudent.patronymic;
		age = cpyStudent.age;
		return *this;
	}
};

std::ostream& operator<<(std::ostream & os, Student student) {
	os << student.getSurname();
	os << " " << student.getName();
	os << " " << student.getPatronymic();
	os << " (" << student.getAge() << " yo)";
	return os;
}

/*Группа студентов*/
class StudentGroup {
	private:
	std::string groupName;	// Название группы
	int studentCount;		// Колличество студентов
	Student* students;		// Список студентов

	/*Перемещение списка студентов в другой*/
	void moveGroup(Student* catcherGroup, Student* sourceGroup, int groupCount) {
		for (int i = 0; i < groupCount; i++) 
			catcherGroup[i] = sourceGroup[i];
		delete[] sourceGroup;
	}

	/*Перестановка местами студентов*/
	void replace(Student& student1, Student& student2) {
		Student tmpStudent = student1;
		student1 = student2;
		student2 = tmpStudent;
	}

	public:
	StudentGroup(std::string groupName = "") {
		this->groupName= groupName;
		studentCount = 0;
		students = nullptr;
	};

	StudentGroup(std::string groupName, int studentCount, Student* studentList) {
		this->groupName= groupName;
		this->studentCount = studentCount;

		students = new Student[studentCount];
		for (int i = 0; i < studentCount; i++) 
			students[i] = studentList[i];
	};

	std::string getName() { return groupName; }
	void setName(std::string groupName) { this->groupName= groupName; }

	int getStudentCount() { return studentCount; }
	void setStudentCount(int studentCount) {this->studentCount = studentCount; }

	Student* getStudentList() { return students; }

	/*Добавить студента в группу*/
	void addStudent(Student& newStudent) {
		if (studentCount > 0) {
			Student* tmpGrope = new Student[studentCount];
			moveGroup(tmpGrope, students, studentCount);
			students = new Student[studentCount + 1];
			moveGroup(students, tmpGrope, studentCount);
		} else 
			students = new Student[studentCount+1];
		students[studentCount++] = newStudent;
	}
	
	/*Добавить студента в группу*/
	void addStudent(std::string surname = "", std::string name = "", std::string patronymic = "", int age = 0) {
		if (studentCount > 0) {
			Student* tmpGrope = new Student[studentCount];
			moveGroup(tmpGrope, students, studentCount);
			students = new Student[studentCount + 1];
			moveGroup(students, tmpGrope, studentCount);
		} else 
			students = new Student[studentCount+1];
		students[studentCount++].changeInfo(surname, name, patronymic, age);
	}

	/*Получить номер студента в группе*/
	int getStudentNumber(std::string surname) {
		for (int i = 0; i < studentCount; i++)
			if (students[i].getSurname() == surname) 
				return i;
	}

	/*Удалить студента из группы*/
	void deleteStudent(int studentNumber) {
		if (studentCount) {
			Student* tmpGrope = new Student[studentCount];
			moveGroup(tmpGrope, students, studentCount);
			students = new Student[studentCount - 1];
			int j = 0;
			for (int i = 0; i < studentCount; i++)
				if (i != studentNumber)
					students[j++] = tmpGrope[i];
			delete[] tmpGrope;
			studentCount--;
		}
	}
	
	/*Сортировка студентов*/
	void sort() {
		for (int i = studentCount / 2; i > 0; i /= 2)
			for (int j = i; j < studentCount; j++)
				for (int k = j - i; k >= 0; k -= i)
					if (students[k].getSurname() > students[k + i].getSurname())
						replace(students[k], students[k + i]);
	}

	/*Печать списка студентов группы*/
	void printStudentList() {
		for (int i = 0; i < studentCount; i++) 
			std::cout << i+1<<": "<< students[i]<<'\n';
	}
	
	Student& operator [](int studentNumber) {return students[studentNumber];}
	
	StudentGroup& operator=(StudentGroup& cpyGroup) {
		groupName = cpyGroup.getName();
		studentCount = cpyGroup.getStudentCount();
		delete[] students;
		students = new Student[studentCount];
		for (int i = 0; i < studentCount; i++)
			students[i] = cpyGroup[i];
		return *this;
	}
};
std::ostream& operator<<(std::ostream & os, StudentGroup group) {
	os << group.getName();
	return os;
}

class University {
	private:
	int groupCount;			//Колличество групп
	StudentGroup* groups;	//Список групп
	
	/*Переместить один список групп в другой*/
	void moveGroups(StudentGroup* inGroups, StudentGroup* outGroups, int groupsCount) {
		for (int i = 0; i < groupsCount; i++) {
			inGroups[i] = outGroups[i];
		}
		delete[] outGroups;
	}
	
	public:
	University() {
		groupCount = 0;
		groups = nullptr;
	};

	University(int groupCount) {
		this->groupCount = groupCount;
		groups = new StudentGroup[groupCount];
	}

	University(int groupCount, StudentGroup* groupList) {
		this->groupCount = groupCount;
		groups = new StudentGroup[groupCount];
		for (int i = 0; i < groupCount; i++) 
			groups[i] = groupList[i];
	}

	/*Получить номер группы*/
	int getGroupNumber(std::string groupName) {
		for (int i = 0; i < groupCount; i++)
			if (groups[i].getName() == groupName)
				return i;
	}
	/*Получить колличество групп*/
	int getGroupCount() { return groupCount; }

	/*Добавить новую группу*/
	void addGroup(std::string groupName) {
		if (groupCount > 0) {
			StudentGroup* tmpGroup = new StudentGroup[groupCount];
			moveGroups(tmpGroup, groups, groupCount);
			groups = new StudentGroup[groupCount + 1];
			moveGroups(groups, tmpGroup, groupCount);
		} else 
			groups = new StudentGroup[groupCount+1];
		groups[groupCount++].setName(groupName);
	}
	
	/*Добавить новую группу*/
	void addGroup(StudentGroup& addedGroup) {
		if (groupCount > 0) {
			StudentGroup* tmpGroup = new StudentGroup[groupCount];
			moveGroups(tmpGroup, groups, groupCount);
			groups = new StudentGroup[groupCount + 1];
			moveGroups(groups, tmpGroup, groupCount);
		} else
			groups = new StudentGroup[groupCount+1];
		groups[groupCount++] = addedGroup;
	}
	
	/*Удалить группу*/
	void deleteGroup(int groupNumber) {
		if (groupCount) {
			StudentGroup* tmpGroup = new StudentGroup[groupCount];
			moveGroups(tmpGroup, groups, groupCount);
			groups = new StudentGroup[groupCount - 1];
			int j = 0;
			for (int i = 0; i < groupCount; i++)
				if (i != groupNumber)
					groups[j++] = tmpGroup[i];
			delete[] tmpGroup;
			groupCount--;
		}
	}

	StudentGroup& operator [](int groupNumber) { return groups[groupNumber]; }
};

std::ostream& operator<<(std::ostream & os, University groupList) {
	for (int i = 0; i < groupList.getGroupCount(); i++)
		os << i + 1 << ": " << groupList[i] << '\n';
	return os;
}


enum MenuMain {
	Create = '1',
	View,
	Edit,
	Sort,
	Delete = '5',
	Esc = '\x1b',
	Enter = '\r'
};

enum MenuEdit {
	Surname = '1',
	Name,
	Patronymic,
	Age,
};

int groupChoice(University* groupList, std::string actionType);
int studentChoice(StudentGroup studentList);
void getTestGroups(University* groupList);
void createNewGroup(University* groupList);
void editGroup(StudentGroup* group);
std::string getStrFild(std::string fildName);
int getIntFild(std::string fildName);
void showMenuEdit();
char menuEditChoice();
void showMainMenu();
char menuChoice();

void main() {
	University* groupList = new University;
	
	getTestGroups(groupList);
	
	int choicedGroup = 0;
	
	while (true) {
		showMainMenu();
		switch (menuChoice()) {
			case Create:
				createNewGroup(groupList);
				break;
			case View:
				choicedGroup = groupChoice(groupList, "VIEW");
				(*groupList)[choicedGroup].printStudentList();
				break;
			case Edit:
				choicedGroup = groupChoice(groupList, "EDIT");
				editGroup(&(*groupList)[choicedGroup]);
				break;
			case Sort:
				choicedGroup = groupChoice(groupList, "SORT");
				(*groupList)[choicedGroup].sort();
				(*groupList)[choicedGroup].printStudentList();
				break;
			case Delete:
				choicedGroup = groupChoice(groupList, "DELETE");
				(*groupList).deleteGroup(choicedGroup);
				(*groupList)[choicedGroup].printStudentList();
				break;
			case Esc:
				return;
			default:
				break;
		}
		std::cout << "\nPress eny key to return to main menu";
		getch();
	}
}

/*ДОБАВЛЕНИЕ ДВУХ ТЕСТОВЫХ ГРУПП*/
void getTestGroups(University* groupList) {
	(*groupList).addGroup("First group");
	(*groupList)[0].addStudent("Ivanov", "Ivan", "Ivanovich", 25);
	(*groupList)[0].addStudent("Petrov", "Petr", "Petrovich", 22);
	(*groupList)[0].addStudent("Sidorov", "Sidor", "Sidorovich", 27);

	Student Rogov("Rogov", "Vasya", "Pupkin", 32);
	Student Chapaev("Chapaev", "Vasiliy", "Ivanovich", 30);
	Student Isaev("Isaev", "Petr", "Semenovich", 25);

	(*groupList).addGroup("Second group");
	(*groupList)[1].addStudent(Rogov);
	(*groupList)[1].addStudent(Chapaev);
	(*groupList)[1].addStudent(Isaev);
}

/*Создать новую группу*/
void createNewGroup(University* groupList) {
	std::cout << "\t\t\t***Creation the new group***\n\n";
	std::cout << "New group name: ";
	std::string newGroupName;
	std::cin >> newGroupName;
	
	(*groupList).addGroup(newGroupName);
	
	int groupCount = (*groupList).getGroupCount();
	
	std::cout << "Count of students: ";
	int studentCount;
	std::cin >> studentCount;

	std::string surnameStudent;
	std::string nameStudent;
	std::string patronymicStudent;
	int ageStudent;

	for (int i = 0; i < studentCount; i++) {
		std::cout << "\n***Student #" << i + 1 << "***\n";
		std::cout << "Surname: ";
		std::cin >> surnameStudent;
		std::cout << "Name: ";
		std::cin >> nameStudent;
		std::cout << "Patronymic: ";
		std::cin >> patronymicStudent;
		std::cout << "Age: ";
		std::cin >> ageStudent;
		(*groupList)[groupCount-1].addStudent(surnameStudent, nameStudent, patronymicStudent, ageStudent);
	}
	std::cout << "\n\nNew group "<< newGroupName<<"is added\n";
}

/*Редактироване группы*/
void editGroup(StudentGroup* group) {
	int choicedStudent = 0;
	std::cout << (*group) << '\n';
	(*group).printStudentList();
	choicedStudent = studentChoice(*group);
	system("cls");
	std::cout << (*group) << '\n';
	std::cout << (*group)[choicedStudent] << "\n\n";
	showMenuEdit();
	switch (menuEditChoice()) {
		case Surname:
			(*group)[choicedStudent].setSurname(getStrFild("surname"));
			break;
		case Name:
			(*group)[choicedStudent].setName(getStrFild("name"));
			break;
		case Patronymic:
			(*group)[choicedStudent].setPatronymic(getStrFild("patronymic"));
			break;
		case Age:
			(*group)[choicedStudent].setAge(getIntFild("age"));
			break;
		case Delete:
			(*group).deleteStudent(choicedStudent);
			break;
		case Esc:
			break;
		default:
			break;
	}
	(*group).printStudentList();
}

/*Выбор номера группы*/
int groupChoice(University* groupList, std::string actionType) {
	std::cout << "Groups list:\n";
	std::cout << *groupList;
	int groupNumber;
	std::string tmpStr;
	while (true) {
		std::cout << "\nNumber of group for " << actionType << ": ";
		std::cin >> tmpStr;
		groupNumber = atoi(tmpStr.c_str());
		if ((groupNumber >= 1) && (groupNumber <= (*groupList).getGroupCount()))
			break;
		else 
			std::cout << "Incorrect number. Pleace try again.\n";
	}
	std::cout << "\n";
	return groupNumber-1;
}

/*Выбор номера студента*/
int studentChoice(StudentGroup studentList) {
	std::cout << "\nNumber of student: ";
	std::string tmpStr;
	int studentNumber;
	while (true) {
		std::cin >> tmpStr;
		studentNumber = atoi(tmpStr.c_str());
		if ((studentNumber >= 1) && (studentNumber <= studentList.getStudentCount()))
			break;
		else
			std::cout << "Incorrect number. Pleace try again.\n";
	}

	std::cout << "\n";
	return studentNumber - 1;
}

/*Получить строковое значение поля*/
std::string getStrFild(std::string fildName) {
	std::cout << "New " << fildName << ": ";
	std::string rezult;
	std::cin >> rezult;
	std::cout << "\n";
	return rezult;
}

/*Получение целочисленного значения поля*/
int getIntFild(std::string fildName) {
	std::cout << "New " << fildName << ": ";
	std::string tmpStr;
	int rezult;

	while (true) {
		std::cin >> tmpStr;
		rezult = atoi(tmpStr.c_str());
		if (rezult !=  0)
			break;
		else
			std::cout << "Incorrect number. Pleace try again.\n";
	}
	std::cout << "\n";
	return rezult;
}

/*Отобрзить меню редактирования*/
void showMenuEdit() {
	printf("EDIT\n");
	printf("1   Surname\n");
	printf("2   Name\n");
	printf("3   Patronymic\n");
	printf("4   Age\n");
	printf("5   Delete\n");
	printf("ESC EXIT.\n\n");
}

/*Выбор пункта меню редактирования*/
char menuEditChoice() {
	while (true) {
		char pressKey = getch(); //нажатая клавиша
		switch (pressKey) {
			case Surname:
			case Name:
			case Patronymic:
			case Age:
			case Delete:
			case Esc:
				return pressKey;
			default:
				break;
		}
	}
}

/*Отобрзить главное меню*/
void showMainMenu() {
	system("cls");
	printf("Choise the point of menu...\n");
	printf("1   Create new group\n");
	printf("2   View group\n");
	printf("3   Edit group\n");
	printf("4   Sort group\n");
	printf("5   Delete group\n");
	printf("ESC EXIT.\n\n");
}


/*Выбор пункта главного меню*/
char menuChoice() {
	while (true) {
		char pressKey = getch(); //нажатая клавиша
		switch (pressKey) {
			case Create:
			case View:
			case Edit:
			case Sort:
			case Delete:
			case Esc:	
				system("cls");
				return pressKey;
			default:
				break;
		}
	}
}
