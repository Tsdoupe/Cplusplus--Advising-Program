//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Tyler Doupe
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : CS300 Project Two
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Define Course structure
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Node for handling hash table
struct Node {
    Course course;
    unsigned int key;
    Node* next;

    // Constructor to initialize empty node
    Node() {
        key = UINT_MAX;
        next = nullptr;
    }

    // Constructor to initialize a node with a course and a key
    Node(Course course, unsigned int key) : Node() {
        this->course = course;
        this->key = key;
    }
};

// Implement Hash Table
class HashTable {
private:
    vector<Node> nodes;
    unsigned int tableSize = 179;  // Default size

    // Hash functionto convert course number into a key
    unsigned int hash(string courseNumber) {
        unsigned int key = 0;
        for (char c : courseNumber) {
            key += c;
        }
        return key % tableSize;
    }

public:
    // Constructor to initialize hash table with given size
    HashTable() {
        nodes.resize(tableSize);
    }

    // Function to insert a course into the hash table
    void insertCourse(Course course) {
        unsigned int key = hash(course.courseNumber);
        Node* newNode = new Node(course, key);  // Creates new node for course
        Node* current = &nodes[key];  // Find bucket using hash key

        // If bucket is empty, insert course
        if (current->key == UINT_MAX) {
            current->key = key;
            current->course = course;
            current->next = nullptr;
        } else {
            // Else, add new node to end of the list
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Function to search for a course by course number
    Course searchCourse(string courseNumber) {
        unsigned int key = hash(courseNumber);
        Node* current = &nodes[key];

        // Traverse chain for course number and return course if found
        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }
            // Move to next node in chain
            current = current->next;
        }

        return Course();  // Returns empty Course if not found
    }
    
    // Function to get all course numbers from the hash table
    vector<string> getAllCourseNumbers() {
        vector<string> keys;

        // Iterate through all buckets
        for (const auto& node : nodes) {
            Node* current = const_cast<Node*>(&node);
            // Traverse the chain and collect all course numbers
            while (current != nullptr && current->key != UINT_MAX) {
                keys.push_back(current->course.courseNumber);
                current = current->next;
            }
        }

        return keys;  // Return the vector of course numbers
    }
};

// Function to convert all letters to uppercase
string toUpperCase(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

// Function to load courses from file
void loadCoursesFromFile(const string& filePath, HashTable& courseHashTable) {
    ifstream file(filePath);
    string line;

    // Check if file opens correctly
    if (!file) {
        cout << "Error: Could not open file " << filePath << endl;
        return;
    }

    // Read file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Course course;

        // Parse course number and course name
        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseName, ',');

        // Parse prerequisites
        while (getline(ss, token, ',')) {
            course.prerequisites.push_back(token);
        }

        // Insert the course into the hash table
        courseHashTable.insertCourse(course);
    }

    // State that courses were successfully loaded
    cout << "Courses loaded successfully!" << endl;
}

// Function to print all courses in alphanumeric order
void printAllCourses(HashTable& courseHashTable) {
    // Get all course numbers
    vector<string> keys = courseHashTable.getAllCourseNumbers();

    // Sort course numbers
    sort(keys.begin(), keys.end());

    cout << "Here is a sample schedule: " << endl;

    // Print each course number in sorted order
    for (const string& key : keys) {
        Course course = courseHashTable.searchCourse(key);
        cout << course.courseNumber << ", " << course.courseName << endl;
    }
}

// Function to print course information
void printCourseInfo(HashTable& courseHashTable, const string& courseNumber) {
    // Search for the course
    Course course = courseHashTable.searchCourse(toUpperCase(courseNumber));

    // Check if course is found
    if (!course.courseNumber.empty()) {
        cout << course.courseNumber << ", " << course.courseName << endl;

        // Print prerequisites if available
        if (!course.prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                cout << course.prerequisites[i];
                if (i < course.prerequisites.size() - 1) {
                    cout << ", ";  // Print commas between prerequisites
                }
            }
            cout << endl;
        } else {
            cout << "No prerequisites." << endl;
        }
    } else {
        // If course not found, display a not found message
        cout << "Course number not found." << endl;
    }
}

// Function to display a menu and prompt user for an input
void displayMenu() {
    int choice;
    string filePath, courseNumber;
    HashTable courseHashTable;

    // Display menu options
    while (true) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        // Handles user input
        switch (choice) {
            // Load courses from file
            case 1:
                cin.ignore();
                while(true) {
                    cout << "Enter file path: ";
                    getline (cin, filePath);
                    if (ifstream(filePath)) {
                        loadCoursesFromFile(filePath, courseHashTable);
                        break;
                    } else {
                        cout << "Error: Could not open the file. Please try again." << endl;
                    }
                }
                break;

            // Print all courses in alphanumeric order
            case 2:
                printAllCourses(courseHashTable);
                break;

            // Print specific course information
            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourseInfo(courseHashTable, courseNumber);
                break;

            // Exit application
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                return;
                
            // Handle invalid input
            default:
                cout << choice << " is not a valid option." << endl;
                break;
        } 
    }
}

// Main function to run the application
int main() {
    cout << "Welcome to the course planner." << endl;

    displayMenu();
    return 0;
}
