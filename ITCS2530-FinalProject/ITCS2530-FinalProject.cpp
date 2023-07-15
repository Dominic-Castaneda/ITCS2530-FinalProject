// ITCS2530-FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Student Grade Management System
// Written by: Dominic Castaneda

// Student Grade Management System
// Written by: Dominic Castaneda

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

// Define a structure for the student's class
struct StudentClass {
    std::string name;
    int credits;
    double grade;
};

// Function to get grade letter
char getGradeLetter(double grade) {
    if (grade >= 94) return 'A';
    if (grade >= 90) return 'A-';
    if (grade >= 87) return 'B+';
    if (grade >= 84) return 'B';
    if (grade >= 80) return 'B-';
    if (grade >= 77) return 'C+';
    if (grade >= 74) return 'C';
    if (grade >= 70) return 'C-';
    if (grade >= 67) return 'D+';
    if (grade >= 64) return 'D';
    if (grade >= 61) return 'D-';
    return 'E';
}

// Function to create student account
int createStudentAccount() {
    // Increment the ID number based on the number of existing accounts
    int newID = std::ifstream("StudentAccounts.txt").tellg() + 1;
    std::cout << "ID Number: " << std::setfill('0') << std::setw(4) << newID << "\n";
    std::ofstream accountFile("StudentAccounts.txt", std::ios::app);
    accountFile << newID << "\n";
    accountFile.close();
    return newID;
}

// Function to handle class operations
void classOperations(int studentID) {
    std::vector<StudentClass> classes;
    char action;
    do {
        std::cout << "Add (A) / Delete (D) / Edit (E) a class or Quit (Q): ";
        std::cin >> action;
        if (action == 'A' || action == 'a') {
            StudentClass newClass;
            std::cout << "Class Name: ";
            std::cin >> newClass.name;
            std::cout << "Class Credits: ";
            std::cin >> newClass.credits;
            std::cout << "Class Grade: ";
            std::cin >> newClass.grade;
            classes.push_back(newClass);
        }
        else if (action == 'D' || action == 'd') {
            int classNum;
            std::cout << "Which #: ";
            std::cin >> classNum;
            classes.erase(classes.begin() + classNum - 1);
        }
        else if (action == 'E' || action == 'e') {
            int classNum;
            std::cout << "Which #: ";
            std::cin >> classNum;
            std::cout << "Class Name: ";
            std::cin >> classes[classNum - 1].name;
            std::cout << "Class Credits: ";
            std::cin >> classes[classNum - 1].credits;
            std::cout << "Class Grade: ";
            std::cin >> classes[classNum - 1].grade;
        }
        // Display the classes
        for (size_t i = 0; i < classes.size(); i++) {
            std::cout << i + 1 << ". " << classes[i].name << " " << classes[i].grade << "% (" << getGradeLetter(classes[i].grade) << ") " << classes[i].credits << "\n";
        }
    } while (action != 'Q' && action != 'q');
}

int main() {
    std::cout << "Grade Book System\n";
    std::cout << "Log in (L) / Create Account (C): ";
    char choice;
    std::cin >> choice;
    int studentID = 0;
    if (choice == 'L' || choice == 'l') {
        std::cout << "Enter ID: ";
        std::cin >> studentID;
    }
    else if (choice == 'C' || choice == 'c') {
        std::cout << "Full Name: ";
        std::string name;
        std::cin >> name;
        studentID = createStudentAccount();
        std::cout << "Welcome " << name << "!\n";
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
