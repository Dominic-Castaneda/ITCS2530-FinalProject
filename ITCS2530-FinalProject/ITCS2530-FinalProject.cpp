// Student Grade Management System
// Dominic Castaneda, Tajwar Rahman, Gabriella Stickney
// This program allows students to create accounts, add/delete/modify classes, and calculate their GPA. The system uses numerical grades and equivalent letter grades, with GPA calculated based on credit hours for each class.


/////////////////////////////////////
#include <iostream>   // standard input/output streams
#include <fstream>    // file handling
#include <string>     // string handling
#include <list>       // list data structure
#include <iomanip>    // input/output manipulations
#include <filesystem> // filesystem operations
#include <limits>     // data type limits
/////////////////////////////////////


// Definition of the StudentClass structure
// Written by Dominic Castaneda
struct StudentClass {
    std::string name;
    double grade;
    int credits;
};


// Function to convert a numerical grade to a letter grade
// Written by Gabriella Stickney
std::string getGradeLetter(double grade) {
    // Mapping numerical grades to their corresponding letter grades
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


// Function to convert a letter grade to grade points
// Written by Gabriella Stickney
double getGradePoint(const std::string& gradeLetter) {
    // Mapping letter grades to their corresponding grade points
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


// Function to calculate the GPA from a list of classes
// Written by Gabriella Stickney
double calculateGPA(std::list<StudentClass>& classes) {
    // Initialize variables to keep track of total grade points and credits
    double totalGradePoints = 0;
    int totalCredits = 0;

    // Iterate through the classes list
    for (const auto& cls : classes) {
        // Add the product of grade points and credits for each class to the total grade points
        totalGradePoints += getGradePoint(getGradeLetter(cls.grade)) * cls.credits;
        // Add the credits of each class to the total credits
        totalCredits += cls.credits;
    }

    // If there are any credits, calculate GPA, else return 0
    return totalCredits ? totalGradePoints / totalCredits : 0;
}


// Function to create a new student account
// Written by Gabriella Stickney
int createStudentAccount() {
    // Initialize ID for new account
    int newID = 1000;
    std::string line;
    std::ifstream accountFile("StudentAccounts.txt");

    // Iterate through the existing accounts to find the last ID used
    while (std::getline(accountFile, line))
        ++newID;
    accountFile.close();

    // Display the new ID
    std::cout << "Your new ID is: " << std::setfill('0') << std::setw(4) << newID << "\n";


    // Prompt the user for a new password
    std::cout << "Enter new password: ";
    std::string password;
    std::cin >> password;


    // Append the new account (ID and password) to the existing accounts file
    std::ofstream accountFileOut("StudentAccounts.txt", std::ios::app);
    accountFileOut << std::setfill('0') << std::setw(4) << newID << " " << password << "\n";
    accountFileOut.close();


    // Return the new ID
    return newID;
}


// Function to load student data from a file
// Written by Dominic Castaneda
std::list<StudentClass> loadStudentData(int studentID) {
    // Initialize a list to store the classes
    std::list<StudentClass> classes;
    std::ifstream file(std::to_string(studentID) + ".txt");
    StudentClass tmp;

    // Read the class data from the file and store it in the list
    while (file >> tmp.name >> tmp.credits >> tmp.grade) {
        classes.push_back(tmp);
    }
    file.close();

    // Return the list of classes
    return classes;
}


// Function to save student data to a file
// Written by Dominic Castaneda
void saveStudentData(int studentID, std::list<StudentClass>& classes) {

    // Open the student's file
    std::ofstream file(std::to_string(studentID) + ".txt");

    // Write the class data to the file
    for (const auto& cls : classes) {
        file << cls.name << " " << cls.credits << " " << cls.grade << "\n";
    }
    file.close();
}


// Function to display student data
// Written by Tajwar Rahman
void displayStudentData(int studentID) {

    // Load the student's data
    std::list<StudentClass> classes = loadStudentData(studentID);

    // Display table headers
    std::cout << std::left << std::setw(5) << "#"
        << std::setw(15) << "Class"
        << std::setw(15) << "Grade"
        << "Credit Hours" << "\n";

    // Underline headers
    std::cout << std::string(50, '_') << "\n";


    // Display each class
    int classNum = 1;
    for (const auto& cls : classes) {
        std::cout << std::left << std::setw(5) << classNum++
            << std::setw(15) << cls.name
            << std::fixed << std::setprecision(2) << cls.grade << "%" << " (" << getGradeLetter(cls.grade) << ")"
            << std::right << std::setw(5) << cls.credits << "\n";
    }
}


// Function to handle operations related to classes
// Written by Tajwar Rahman
void classOperations(int studentID) {
    // Load the student's data
    std::list<StudentClass> classes = loadStudentData(studentID);
    char action;

    // Keep prompting the user for an action until they choose to quit
    do {

        // Display the available options
        std::cout << "1. Class List\n2. GPA\n3. Edit\nQuit (Q)\nChoose an Option: ";
        std::cin >> action;

        // Display the class list
        if (action == '1') {
            displayStudentData(studentID);
            std::cout << "\n";
        }
        else if (action == '2') {
            // Calculate and display the GPA
            double gpa = calculateGPA(classes);
            std::cout << "GPA: " << std::fixed << std::setprecision(4) << gpa << "\n";
        }
        else if (action == '3') {
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
        else if (action == 'Q' || action == 'q') {
            return;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}


// Function to validate login credentials
// Written by Dominic Castaneda
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


// Main function
// Written by Tajwar Rahman
int main() {
    std::cout << "***********************\n";
    std::cout << "** Grade Book System **\n";
    std::cout << "***********************\n\n";
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
