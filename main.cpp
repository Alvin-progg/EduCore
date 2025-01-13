#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Student {
private:
    int id;
    string name;
    string grade;

public:
    Student(int studentID, string studentName, string studentGrade)
        : id(studentID), name(studentName), grade(studentGrade) {}

    int getId() const {
        return id;
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name << ", Grade: " << grade << endl;
    }

    void setGrade(string newGrade) {
        grade = newGrade;
    }
};

class StudentManager {
private:
    vector<Student> students;
    int nextStudentID = 1;

public:
    void addStudent(string name, string grade) {
        int id = nextStudentID++;
        Student newStudent(id, name, grade);
        students.push_back(newStudent);
        cout << "New Student Added, ID: " << id << endl;
    }

    void displayAllStudents() const {
        for (const auto& student : students) {
            student.display();
        }
    }

    void updateStudentGrade(int id, string newGrade) {
        for (auto& student : students) {
            if (student.getId() == id) {
                student.setGrade(newGrade);
                cout << "Student Grade Updated, ID: " << id << endl;
                return;
            }
        }
        cout << "Student not found!" << endl;
    }

    void deleteStudent(int id) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                students.erase(it);
                cout << "Student Deleted, ID: " << id << endl;
                return;
            }
        }
        cout << "Student not found!" << endl;
    }
};

int main() {
    StudentManager manager;
    int choice = 0;
    string name, grade;
    int id;

    while (choice != 5) {
        cout << "########### STUDENT MANAGEMENT ##########" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Delete Student" << endl;
        cout << "3. Update Student Grade" << endl;
        cout << "4. Display All Students" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Student Name: ";
                cin >> name;
                cout << "Enter Student Grade: ";
                cin >> grade;
                manager.addStudent(name, grade);
                break;
            case 2:
                cout << "Enter Student ID: ";
                cin >> id;
                manager.deleteStudent(id);
                break;
            case 3:
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter New Grade: ";
                cin >> grade;
                manager.updateStudentGrade(id, grade);
                break;
            case 4:
                manager.displayAllStudents();
                break;
            case 5:
                cout << "Exiting the system..." << endl;
                break;
            default:
                cout << "Invalid Choice!" << endl;
        }
    }

    return 0;
}