#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// Structure to hold course information
struct Course {
    string name;
    vector<string> subjects;s
};

class GradeSystem {
public:
    // Convert string grade to description
    static string getDescription(double grade) {
        if (grade == 0) return "Not yet graded";
        else if (grade == 1.0) return "Excellent";
        else if (grade <= 1.75) return "Very Good";
        else if (grade <= 2.5) return "Good";
        else if (grade <= 3.0) return "Satisfactory";
        else if (grade <= 4.0) return "Passing";
        else return "Failed";
    }

    // Format grade to show 2 decimal places
    static string formatGrade(double grade) {
        char buffer[10];
        sprintf(buffer, "%.2f", grade);
        return string(buffer);
    }
};

// Student class
class Student {
private:
    string code;
    string name;
    double gwa;
    string courseName;
    map<string, double> subjectGrades;

public:
    // Modified constructor to initialize GWA to 0
    Student(string studentCode, string studentName, string course)
        : code(studentCode), name(studentName), gwa(0), courseName(course) {}

    string getCode() const { return code; }
    string getName() const { return name; }
    double getGWA() const { return gwa; }
    string getCourse() const { return courseName; }

    void setCourse(string newCourse) { courseName = newCourse; }
    void setGWA(double newGwa) { gwa = newGwa; }

    void addSubjectGrade(string subject, double grade) {
        subjectGrades[subject] = grade;
        calculateGWA();
    }

    void display() const {
        cout << "Code: " << code << ", Name: " << name;

        if (gwa == 0) {
            cout << ", GWA: Not yet graded";
        } else {
            cout << ", GWA: " << GradeSystem::formatGrade(gwa)
                 << " (" << GradeSystem::getDescription(gwa) << ")";
        }

        cout << ", Course: " << courseName << endl;
    }

    void showSubjectGrades() const {
        if (subjectGrades.empty()) {
            cout << "No grades recorded for this student." << endl;
            return;
        }

        cout << "\nGrades for " << name << " (Code: " << code << "):\n";
        cout << left << setw(30) << "Subject" << setw(10) << "Grade" << setw(15) << "Description" << endl;
        cout << string(55, '-') << endl;

        for (const auto& subject : subjectGrades) {
            cout << left << setw(30) << subject.first
                 << setw(10) << GradeSystem::formatGrade(subject.second)
                 << setw(15) << GradeSystem::getDescription(subject.second) << endl;
        }

        cout << string(55, '-') << endl;

        if (gwa == 0) {
            cout << left << setw(30) << "General Weighted Average"
                 << setw(10) << "N/A"
                 << setw(15) << "Not yet graded" << endl;
        } else {
            cout << left << setw(30) << "General Weighted Average"
                 << setw(10) << GradeSystem::formatGrade(gwa)
                 << setw(15) << GradeSystem::getDescription(gwa) << endl;
        }
    }

    // Calculate GWA from subject grades
    void calculateGWA() {
        if (subjectGrades.empty()) {
            gwa = 0;
            return;
        }

        double total = 0.0;
        for (const auto& subject : subjectGrades) {
            total += subject.second;
        }
        gwa = total / subjectGrades.size();
    }
};

// Main class to manage students
class StudentManager {
private:
    vector<Student> students;
    map<string, Course> courses;

    // Set up default courses
    void setupCourses() {
        // BSIT Course
        Course bsit;
        bsit.name = "BSIT";
        bsit.subjects = {"CS 131", "GEd 109", "GEd 105",
                         "Fili 102", "CS 111", "PATHFit 2", "MATH 111", "NSTP 121CW"};
        courses["BSIT"] = bsit;
    }

public:
    StudentManager() {
        setupCourses();
    }

    // Modified to remove GWA parameter
    void addStudent(string code, string name, string course) {
        for (const auto& student : students) {
            if (student.getCode() == code) {
                cout << "Error: Student code already exists!" << endl;
                return;
            }
        }

        Student newStudent(code, name, course);
        students.push_back(newStudent);
        cout << "Student added successfully. Code: " << code << endl;
    }

    // Show all students
    void showAllStudents() const {
        if (students.empty()) {
            cout << "No students found." << endl;
            return;
        }

        cout << "\n--- Student List ---" << endl;
        for (const auto& student : students) {
            student.display();
        }
        cout << endl;
    }

    // Delete a student
    void deleteStudent(string code) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getCode() == code) {
                students.erase(it);
                cout << "Student deleted. Code: " << code << endl;
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    // Update student course
    void updateCourse(string code, string newCourse) {
        for (auto& student : students) {
            if (student.getCode() == code) {
                student.setCourse(newCourse);
                cout << "Course updated for student code: " << code << endl;
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    // Add subject grade for a student
    void addSubjectGrade(string code) {
        for (auto& student : students) {
            if (student.getCode() == code) {
                string courseName = student.getCourse();

                // Show course subjects
                cout << "Available subjects for " << courseName << ":" << endl;
                if (courses.find(courseName) != courses.end()) {
                    vector<string>& subjects = courses[courseName].subjects;
                    for (int i = 0; i < subjects.size(); i++) {
                        cout << i+1 << ". " << subjects[i] << endl;
                    }

                    // Get subject choice
                    int choice;
                    cout << "Select subject (1-" << subjects.size() << ") or 0 to enter manually: ";
                    cin >> choice;

                    string subject;
                    if (choice == 0) {
                        cout << "Enter subject name: ";
                        cin.ignore();
                        getline(cin, subject);
                    } else if (choice > 0 && choice <= subjects.size()) {
                        subject = subjects[choice-1];
                    } else {
                        cout << "Invalid choice." << endl;
                        return;
                    }

                    // Get grade
                    double grade;
                    cout << "Enter grade for " << subject << " (1.0-5.0): ";
                    cin >> grade;

                    if (grade < 1.0 || grade > 5.0) {
                        cout << "Invalid grade. Must be between 1.0 and 5.0." << endl;
                        return;
                    }

                    student.addSubjectGrade(subject, grade);
                    cout << "Grade added successfully." << endl;

                    // Show updated grades
                    student.showSubjectGrades();
                    return;
                } else {
                    cout << "Course not found. Enter subject manually: ";
                    string subject;
                    cin.ignore();
                    getline(cin, subject);

                    double grade;
                    cout << "Enter grade for " << subject << " (1.0-5.0): ";
                    cin >> grade;

                    if (grade < 1.0 || grade > 5.0) {
                        cout << "Invalid grade. Must be between 1.0 and 5.0." << endl;
                        return;
                    }

                    student.addSubjectGrade(subject, grade);
                    cout << "Grade added successfully." << endl;

                    // Show updated grades
                    student.showSubjectGrades();
                    return;
                }
            }
        }
        cout << "Student not found." << endl;
    }

    // Show GWA for all students
    void showGWA() const {
        if (students.empty()) {
            cout << "No students found." << endl;
            return;
        }

        cout << "\n--- GWA Report ---" << endl;
        cout << left << setw(15) << "Student Code"
             << setw(20) << "Name"
             << setw(10) << "GWA"
             << setw(15) << "Description"
             << setw(10) << "Course" << endl;
        cout << string(70, '-') << endl;

        for (const auto& student : students) {
            cout << left << setw(15) << student.getCode()
                 << setw(20) << student.getName();

            if (student.getGWA() == 0) {
                cout << setw(10) << "N/A"
                     << setw(15) << "Not yet graded";
            } else {
                cout << setw(10) << GradeSystem::formatGrade(student.getGWA())
                     << setw(15) << GradeSystem::getDescription(student.getGWA());
            }

            cout << setw(10) << student.getCourse() << endl;
        }
        cout << endl;
    }

    // Show subject grades for a student
    void showStudentGrades(string code) {
        for (const auto& student : students) {
            if (student.getCode() == code) {
                student.showSubjectGrades();
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    // Show available courses
    void showCourses() const {
        cout << "\n--- Available Courses ---" << endl;
        for (const auto& course : courses) {
            cout << "Course: " << course.first << endl;
            cout << "Subjects:" << endl;
            for (const auto& subject : course.second.subjects) {
                cout << "  - " << subject << endl;
            }
            cout << endl;
        }
    }
};

int main() {
    StudentManager manager;
    int choice = 0;
    string code, name, course;

    while (choice != 8) {
        cout << "\n########### EduCore ##########" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Delete Student" << endl;
        cout << "3. Display All Students" << endl;
        cout << "4. Update Student Course" << endl;
        cout << "5. Add Subject Grade" << endl;
        cout << "6. Display GWA Report" << endl;
        cout << "7. Display Student Subject Grades" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Student Code (e.g., 24-49051): ";
                cin.ignore();
                getline(cin, code);
                cout << "Enter Student Name: ";
                getline(cin, name);
                cout << "Enter Student Course (BSIT): ";
                getline(cin, course);
                // Removed GWA input
                manager.addStudent(code, name, course);
                break;

            case 2:
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.deleteStudent(code);
                break;

            case 3:
                manager.showAllStudents();
                break;

            case 4:
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                cout << "Enter New Course: ";
                getline(cin, course);
                manager.updateCourse(code, course);
                break;

            case 5:
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.addSubjectGrade(code);
                break;

            case 6:
                manager.showGWA();
                break;

            case 7:
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.showStudentGrades(code);
                break;

            case 8:
                cout << "Exiting system. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}