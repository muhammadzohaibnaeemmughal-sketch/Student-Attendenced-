#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Base Class: Person (Demonstrating Encapsulation & Abstraction)
class Person {
protected:
    string name;
    int id;
public:
    Person(string n = "", int i = 0) : name(n), id(i) {}
    virtual void displayDetails() const = 0; // Pure Virtual Function
    int getId() const { return id; }
    string getName() const { return name; }
};

// Derived Class: Student (Demonstrating Inheritance)
class Student : public Person {
private:
    string cinNumber; // Course Identification Number
    string courseName;
    float attendancePercentage;
    int totalClasses;
    int attendedClasses;
    float quizMarks, midtermMarks, finalMarks;
    float totalMarks;
    char grade;
    float gpa;

public:
    Student(string n = "", int i = 0, string cinNum = "", string cName = "", int tc = 0, int ac = 0, float q = 0, float m = 0, float f = 0)
        : Person(n, i), cinNumber(cinNum), courseName(cName), totalClasses(tc), attendedClasses(ac), quizMarks(q), midtermMarks(m), finalMarks(f) {
        calculateAttendance();
        calculateGrade();
    }

    void calculateAttendance() {
        if (totalClasses > 0) {
            attendancePercentage = ((float)attendedClasses / totalClasses) * 100;
        } else {
            attendancePercentage = 0;
        }
    }

    void calculateGrade() {
        // Weighted Evaluation: Quizzes (20%), Midterm (30%), Final Exam (50%)
        totalMarks = (quizMarks * 0.2) + (midtermMarks * 0.3) + (finalMarks * 0.5);

        if (totalMarks >= 85) { grade = 'A'; gpa = 4.0; }
        else if (totalMarks >= 75) { grade = 'B'; gpa = 3.5; }
        else if (totalMarks >= 65) { grade = 'C'; gpa = 3.0; }
        else if (totalMarks >= 50) { grade = 'D'; gpa = 2.0; }
        else { grade = 'F'; gpa = 0.0; }
    }

    void displayDetails() const override {
        cout << "\n---------------------------------------------\n";
        cout << "ID: " << id << " | Name: " << name << endl;
        cout << "Course: " << courseName << " (CIN: " << cinNumber << ")\n";
        cout << "Attendance: " << attendedClasses << "/" << totalClasses
             << " (" << fixed << setprecision(2) << attendancePercentage << "%)\n";
        cout << "Marks -> Quiz: " << quizMarks << ", Midterm: " << midtermMarks << ", Final: " << finalMarks << endl;
        cout << "Total Weighted Marks: " << totalMarks << " | Grade: " << grade << " | GPA: " << gpa << endl;
        if (attendancePercentage < 75) {
            cout << "⚠️ WARNING: Attendance is below the mandatory 75% threshold!\n";
        }
        cout << "---------------------------------------------\n";
    }

    // Getters for Data Serialization (File Handling)
    string getCinNumber() const { return cinNumber; }
    string getCourseName() const { return courseName; }
    int getTotalClasses() const { return totalClasses; }
    int getAttendedClasses() const { return attendedClasses; }
    float getQuiz() const { return quizMarks; }
    float getMid() const { return midtermMarks; }
    float getFinal() const { return finalMarks; }
};

// System Management Class
class AttendanceAndGradeSystem {
private:
    string filename;

public:
    AttendanceAndGradeSystem(string fname = "student_records.txt") : filename(fname) {}

    void addStudent() {
        string name, cinNumber, courseName;
        int id, totalCls, attCls;
        float q, m, f;

        cout << "\nEnter Student ID: "; cin >> id;
        cin.ignore(); // Clears the input buffer stream
        cout << "Enter Student Name: "; getline(cin, name);
        cout << "Enter Course Identification Number (CIN): "; getline(cin, cinNumber);
        cout << "Enter Course Name: "; getline(cin, courseName);
        cout << "Enter Total Classes Scheduled: "; cin >> totalCls;
        cout << "Enter Attended Classes: "; cin >> attCls;
        cout << "Enter Quiz Marks (Scale 0-100): "; cin >> q;
        cout << "Enter Midterm Marks (Scale 0-100): "; cin >> m;
        cout << "Enter Final Exam Marks (Scale 0-100): "; cin >> f;

        Student s(name, id, cinNumber, courseName, totalCls, attCls, q, m, f);

        ofstream outFile(filename, ios::app);
        if (outFile.is_open()) {
            outFile << s.getId() << "," << s.getName() << "," << s.getCinNumber() << ","
                    << s.getCourseName() << "," << s.getTotalClasses() << ","
                    << s.getAttendedClasses() << "," << s.getQuiz() << "," << s.getMid() << ","
                    << s.getFinal() << "\n";
            outFile.close();
            cout << "\n✔️ Student record successfully saved to database!\n";
        } else {
            cout << "\n❌ Error: Failed to access database storage file!\n";
        }
    }

    void displayAllStudents() {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cout << "\n⚠️ No records found or database file does not exist.\n";
            return;
        }

        string line;
        cout << "\n========= DISPLAYING ALL REGISTERED STUDENT RECORDS =========";
        while (getline(inFile, line)) {
            size_t pos = 0;
            vector<string> tokens;
            while ((pos = line.find(',')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line); // Adds the last remaining component

            if (tokens.size() == 9) {
                Student s(tokens[1], stoi(tokens[0]), tokens[2], tokens[3], stoi(tokens[4]),
                          stoi(tokens[5]), stof(tokens[6]), stof(tokens[7]), stof(tokens[8]));
                s.displayDetails();
            }
        }
        inFile.close();
    }

    void searchStudent(int searchId) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cout << "\n⚠️ Database is empty. No records available.\n";
            return;
        }

        string line;
        bool found = false;
        while (getline(inFile, line)) {
            size_t pos = 0;
            vector<string> tokens;
            while ((pos = line.find(',')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);

            if (tokens.size() == 9 && stoi(tokens[0]) == searchId) {
                Student s(tokens[1], stoi(tokens[0]), tokens[2], tokens[3], stoi(tokens[4]),
                          stoi(tokens[5]), stof(tokens[6]), stof(tokens[7]), stof(tokens[8]));
                cout << "\n🎯 Match Found:";
                s.displayDetails();
                found = true;
                break;
            }
        }
        inFile.close();
        if (!found) cout << "\n❌ Search Error: Student with ID " << searchId << " not found in the system.\n";
    }
};

// Main Entry Point
int main() {
    AttendanceAndGradeSystem system("students_database.txt");
    int choice;

    do {
        cout << "\n=========================================";
        cout << "\n   STUDENT ATTENDANCE & GRADE SYSTEM   ";
        cout << "\n=========================================";
        cout << "\n1. Add New Student Record";
        cout << "\n2. Display All Records";
        cout << "\n3. Search Student by ID";
        cout << "\n4. Exit Application";
        cout << "\nChoose an option (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.addStudent();
                break;
            case 2:
                system.displayAllStudents();
                break;
            case 3: {
                int id;
                cout << "\nEnter student identification token (ID) to search: ";
                cin >> id;
                system.searchStudent(id);
                break;
            }
            case 4:
                cout << "\nExiting application. System shutdown complete.\n";
                break;
            default:
                cout << "\n❌ Invalid operation index! Please enter a value between 1 and 4.\n";
        }
    } while (choice != 4);

    return 0;
}
QR CODE BANA
