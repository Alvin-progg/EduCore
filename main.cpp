#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// Structure to hold course information
struct Course {
    string name;              // Name of the course (e.g., BSIT)
    vector<string> subjects;  // List of subjects for this course
};

// Class to manage Philippine grading system
class GradeSystem {
public:
    // Convert numeric grade to description based on Philippine grading system
    // 1.0 = Excellent, 1.0-1.75 = Very Good, 1.75-2.5 = Good, 2.5-3.0 = Satisfactory, 3.0-4.0 = Passing, >4.0 = Failed
    static string getDescription(double grade) {
        if (grade == 0) return "Not yet graded";
        else if (grade == 1.0) return "Excellent";
        else if (grade <= 1.75) return "Very Good";
        else if (grade <= 2.5) return "Good";
        else if (grade <= 3.0) return "Satisfactory";
        else if (grade <= 4.0) return "Passing";
        else return "Failed";
    }

    // Format grade to show 2 decimal places (e.g., 1.50 instead of 1.5)
    static string formatGrade(double grade) {
        char buffer[10];
        sprintf(buffer, "%.2f", grade);
        return string(buffer);
    }
};

// Student class to store and manage individual student data
class Student {
private:
    string code;                      // Student ID code (e.g., 24-49051)
    string name;                      // Student's full name
    double gwa;                       // General Weighted Average
    string courseName;                // Student's course (e.g., BSIT)
    map<string, double> subjectGrades; // Map of subject names to grades

public:
    // Constructor to initialize a new student
    // No initial GWA is provided, it's calculated from subject grades
    Student(string studentCode, string studentName, string course)
        : code(studentCode), name(studentName), gwa(0), courseName(course) {}

    // Getter methods for student properties
    string getCode() const { return code; }    // Returns student's ID code
    string getName() const { return name; }    // Returns student's name
    double getGWA() const { return gwa; }      // Returns student's GWA
    string getCourse() const { return courseName; } // Returns student's course

    // Setter methods for student properties
    void setCourse(string newCourse) { courseName = newCourse; } // Updates student's course
    void setGWA(double newGwa) { gwa = newGwa; }                 // Directly sets GWA (rarely used)

    // Add a grade for a subject and recalculate GWA
    void addSubjectGrade(string subject, double grade) {
        subjectGrades[subject] = grade;  // Add or update grade for the subject
        calculateGWA();                  // Recalculate GWA with the new grade
    }

    // Display basic student information
    void display() const {
        cout << "Code: " << code << ", Name: " << name;
        
        // Handle case when no grades have been entered yet
        if (gwa == 0) {
            cout << ", GWA: Not yet graded";
        } else {
            cout << ", GWA: " << GradeSystem::formatGrade(gwa) 
                 << " (" << GradeSystem::getDescription(gwa) << ")";
        }
        
        cout << ", Course: " << courseName << endl;
    }

    // Display detailed subject grades for the student
    void showSubjectGrades() const {
        // Check if student has any grades
        if (subjectGrades.empty()) {
            cout << "No grades recorded for this student." << endl;
            return;
        }

        // Display header for grades table
        cout << "\nGrades for " << name << " (Code: " << code << "):\n";
        cout << left << setw(30) << "Subject" << setw(10) << "Grade" << setw(15) << "Description" << endl;
        cout << string(55, '-') << endl;

        // Display each subject and its grade
        for (const auto& subject : subjectGrades) {
            cout << left << setw(30) << subject.first
                 << setw(10) << GradeSystem::formatGrade(subject.second)
                 << setw(15) << GradeSystem::getDescription(subject.second) << endl;
        }

        // Display the GWA at the bottom of the table
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

    // Calculate GWA by averaging all subject grades
    void calculateGWA() {
        // If no subjects yet, set GWA to 0
        if (subjectGrades.empty()) {
            gwa = 0;
            return;
        }

        // Calculate the sum of all grades
        double total = 0.0;
        for (const auto& subject : subjectGrades) {
            total += subject.second;
        }
        
        // Calculate average (GWA) by dividing total by number of subjects
        gwa = total / subjectGrades.size();
    }
};

// Main class to manage all students and courses
class StudentManager {
private:
    vector<Student> students;     // Vector to store all students
    map<string, Course> courses;  // Map of course codes to course information

    // Set up default courses with their subjects
    void setupCourses() {
        // BSIT Course
        Course bsit;
        bsit.name = "BSIT";
        bsit.subjects = {"CS 131", "GEd 109", "GEd 105",
                         "Fili 102", "CS 111", "PATHFit 2", "MATH 111", "NSTP 121CW"};
        courses["BSIT"] = bsit;
    }

public:
    // Constructor - initializes the available courses
    StudentManager() {
        setupCourses();
    }

    // Add a new student to the system
    void addStudent(string code, string name, string course) {
        // Check if student code already exists
        for (const auto& student : students) {
            if (student.getCode() == code) {
                cout << "Error: Student code already exists!" << endl;
                return;
            }
        }

        // Create and add the new student
        Student newStudent(code, name, course);
        students.push_back(newStudent);
        cout << "Student added successfully. Code: " << code << endl;
    }

    // Display all students in the system
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

    // Delete a student by their code
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

    // Update a student's course
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

    // Add a subject grade for a specific student
    void addSubjectGrade(string code) {
        // Find the student
        for (auto& student : students) {
            if (student.getCode() == code) {
                string courseName = student.getCourse();

                // Show available subjects for the student's course
                cout << "Available subjects for " << courseName << ":" << endl;
                if (courses.find(courseName) != courses.end()) {
                    vector<string>& subjects = courses[courseName].subjects;
                    for (int i = 0; i < subjects.size(); i++) {
                        cout << i+1 << ". " << subjects[i] << endl;
                    }

                    // Let user select a subject or enter one manually
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

                    // Get the grade for the subject
                    double grade;
                    cout << "Enter grade for " << subject << " (1.0-5.0): ";
                    cin >> grade;

                    // Validate grade
                    if (grade < 1.0 || grade > 5.0) {
                        cout << "Invalid grade. Must be between 1.0 and 5.0." << endl;
                        return;
                    }

                    // Add the grade and show updated grades
                    student.addSubjectGrade(subject, grade);
                    cout << "Grade added successfully." << endl;
                    student.showSubjectGrades();
                    return;
                } else {
                    // If course not found, enter subject manually
                    cout << "Course not found. Enter subject manually: ";
                    string subject;
                    cin.ignore();
                    getline(cin, subject);

                    double grade;
                    cout << "Enter grade for " << subject << " (1.0-5.0): ";
                    cin >> grade;

                    // Validate grade
                    if (grade < 1.0 || grade > 5.0) {
                        cout << "Invalid grade. Must be between 1.0 and 5.0." << endl;
                        return;
                    }

                    // Add the grade and show updated grades
                    student.addSubjectGrade(subject, grade);
                    cout << "Grade added successfully." << endl;
                    student.showSubjectGrades();
                    return;
                }
            }
        }
        cout << "Student not found." << endl;
    }

    // Display GWA for all students in tabular format
    void showGWA() const {
        if (students.empty()) {
            cout << "No students found." << endl;
            return;
        }

        // Display header for GWA report
        cout << "\n--- GWA Report ---" << endl;
        cout << left << setw(15) << "Student Code"
             << setw(20) << "Name"
             << setw(10) << "GWA"
             << setw(15) << "Description"
             << setw(10) << "Course" << endl;
        cout << string(70, '-') << endl;

        // Display each student's GWA information
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

    // Display detailed subject grades for a specific student
    void showStudentGrades(string code) {
        for (const auto& student : students) {
            if (student.getCode() == code) {
                student.showSubjectGrades();
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    // Display all available courses and their subjects
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

// Main function - entry point of the program
int main() {
    StudentManager manager;  // Create the student manager
    int choice = 0;          // User menu choice
    string code, name, course;

    // Main program loop
    while (choice != 8) {
        // Display menu
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

        // Process user choice
        switch (choice) {
            case 1:  // Add a student
                cout << "Enter Student Code (e.g., 24-49051): ";
                cin.ignore();
                getline(cin, code);
                cout << "Enter Student Name: ";
                getline(cin, name);
                cout << "Enter Student Course (BSIT/BSCS): ";
                getline(cin, course);
                manager.addStudent(code, name, course);
                break;

            case 2:  // Delete a student
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.deleteStudent(code);
                break;

            case 3:  // Display all students
                manager.showAllStudents();
                break;

            case 4:  // Update student course
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                cout << "Enter New Course: ";
                getline(cin, course);
                manager.updateCourse(code, course);
                break;

            case 5:  // Add subject grade
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.addSubjectGrade(code);
                break;

            case 6:  // Display GWA report
                manager.showGWA();
                break;

            case 7:  // Display student grades
                cout << "Enter Student Code: ";
                cin.ignore();
                getline(cin, code);
                manager.showStudentGrades(code);
                break;

            case 8:  // Exit the program
                cout << "Exiting system. Goodbye!" << endl;
                break;

            default:  // Invalid choice
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;  // End of program
}