// ITCS2530-FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iomanip>
#include <filesystem>
#include <limits>

struct StudentClass {
    std::string name;
    double grade;
    int credits;
};

std::string getGradeLetter(double grade) {
    if (grade >= 94) return "A";
    if (grade >= 90) return "A-";
    if (grade >= 87) return "B+";
    if (grade >= 84) return "B";
    if (grade >= 80) return "B-";
    if (grade >= 77) return "C+";
    if (grade >= 74) return "C";
    if (grade >= 70) return "C-";
    if (grade >= 67) return "D+";
    if (grade >= 64) return "D";
    if (grade >= 61) return "D-";
    return "E";
}

double getGradePoint(const std::string& gradeLetter) {
    if (gradeLetter == "A") return 4.0;
    if (gradeLetter == "A-") return 3.7;
    if (gradeLetter == "B+") return 3.3;
    if (gradeLetter == "B") return 3.0;
    if (gradeLetter == "B-") return 2.7;
    if (gradeLetter == "C+") return 2.3;
    if (gradeLetter == "C") return 2.0;
    if (gradeLetter == "C-") return 1.7;
    if (gradeLetter == "D+") return 1.3;
    if (gradeLetter == "D") return 1.0;
    if (gradeLetter == "D-") return 0.7;
    return 0.0;
}

double calculateGPA(std::list<StudentClass>& classes) {
    double totalGradePoints = 0;
    int totalCredits = 0;

    for (const auto& cls : classes) {
        totalGradePoints += getGradePoint(getGradeLetter(cls.grade)) * cls.credits;
        totalCredits += cls.credits;
    }

    return totalCredits ? totalGradePoints / totalCredits : 0;
}

int createStudentAccount() {
    int newID = 1000;
    std::string line;
    std::ifstream accountFile("StudentAccounts.txt");
    while (std::getline(accountFile, line))
        ++newID;
    accountFile.close();

    std::cout << "Your new ID is: " << std::setfill('0') << std::setw(4) << newID << "\n";

    std::cout << "Enter new password: ";
    std::string password;
    std::cin >> password;

    std::ofstream accountFileOut("StudentAccounts.txt", std::ios::app);
    accountFileOut << std::setfill('0') << std::setw(4) << newID << " " << password << "\n";
    accountFileOut.close();

    return newID;
}

std::list<StudentClass> loadStudentData(int studentID) {
    std::list<StudentClass> classes;
    std::ifstream file(std::to_string(studentID) + ".txt");
    StudentClass tmp;
    while (file >> tmp.name >> tmp.credits >> tmp.grade) {
        classes.push_back(tmp);
    }
    file.close();
    return classes;
}

void saveStudentData(int studentID, std::list<StudentClass>& classes) {
    std::ofstream file(std::to_string(studentID) + ".txt");
    for (const auto& cls : classes) {
        file << cls.name << " " << cls.credits << " " << cls.grade << "\n";
    }
    file.close();
}

void displayStudentData(int studentID) {
    std::list<StudentClass> classes = loadStudentData(studentID);

    // Table headers
    std::cout << std::left << std::setw(5) << "#"
        << std::setw(15) << "Class"
        << std::setw(15) << "Grade"
        << "Credit Hours" << "\n";

    // Underline headers
    std::cout << std::string(50, '_') << "\n";

    int classNum = 1;
    for (const auto& cls : classes) {
        std::cout << std::left << std::setw(5) << classNum++
            << std::setw(15) << cls.name
            << std::fixed << std::setprecision(2) << cls.grade << "%" << " (" << getGradeLetter(cls.grade) << ")"
            << std::right << std::setw(5) << cls.credits << "\n";
    }
}

void classOperations(int studentID) {
    std::list<StudentClass> classes = loadStudentData(studentID);
    int action;
    do {
        std::cout << "1. GPA\n2. Edit\n3. Quit\nChoose an Option: ";
        std::cin >> action;
        if (action == 1) {
            double gpa = calculateGPA(classes);
            std::cout << "GPA: " << std::fixed << std::setprecision(4) << gpa << "\n";
        }
        else if (action == 2) {
            char subAction;
            std::cout << "Add (A) / Delete (D) / Edit (E) a class or Quit (Q): ";
            std::cin >> subAction;
            if (subAction == 'A' || subAction == 'a') {
                StudentClass newClass;
                std::cout << "Class Name: ";
                std::cin.ignore();
                std::getline(std::cin, newClass.name);
                std::cout << "Class Credits: ";
                std::cin >> newClass.credits;
                std::cout << "Class Grade: ";
                std::cin >> newClass.grade;
                classes.push_back(newClass);
            }
            else if (subAction == 'D' || subAction == 'd') {
                int classNum;
                std::cout << "Which #: ";
                std::cin >> classNum;
                auto iter = classes.begin();
                std::advance(iter, classNum - 1);
                classes.erase(iter);
            }
            else if (subAction == 'E' || subAction == 'e') {
                int classNum;
                std::cout << "Which #: ";
                std::cin >> classNum;
                auto iter = classes.begin();
                std::advance(iter, classNum - 1);
                std::cout << "Class Name: ";
                std::cin.ignore();
                std::getline(std::cin, iter->name);
                std::cout << "Class Credits: ";
                std::cin >> iter->credits;
                std::cout << "Class Grade: ";
                std::cin >> iter->grade;
            }
            saveStudentData(studentID, classes);
        }
    } while (action != 3);
}

bool isValidCredentials(int id, std::string password) {
    std::string line, filePassword;
    int fileID;
    std::ifstream accountFile("StudentAccounts.txt");

    while (accountFile >> fileID >> filePassword) {
        if (fileID == id && filePassword == password) {
            accountFile.close();
            return true;
        }
    }
    accountFile.close();
    return false;
}

int main() {
    std::cout << "Grade Book System\n";
    std::cout << "Log in (L) / Create Account (C): ";
    char choice;
    std::cin >> choice;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int studentID = 0;
    std::string password;
    bool validLogin = false;

    if (choice == 'L' || choice == 'l') {
        std::cout << "Enter ID: ";
        std::cin >> studentID;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (!validLogin) {
            std::cout << "Enter password: ";
            std::cin >> password;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (isValidCredentials(studentID, password)) {
                validLogin = true;
                displayStudentData(studentID);
                std::cout << "\n";
            }
            else {
                std::cout << "Invalid password, try again." << "\n";
            }
        }
    }
    else if (choice == 'C' || choice == 'c') {
        studentID = createStudentAccount();
    }

    classOperations(studentID);
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
