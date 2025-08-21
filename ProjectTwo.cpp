//Mark Vincent Francisco
//CS 300: DSA Analysis and Design
//Computer Science Undergraduate Student
//August 13th, 2025


#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

//composition data type for a Course
struct Course {
    string code;
    string title;
    vector<string> prereqs;
};

//Composition data type definiing a Node of a BST
struct Node {
    Course course;
    Node* left;
    Node* right;
    explicit Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

//BST Class that defines the node, insert method, find method, alphanumeric order method, and clear data structure methods
class BinarySearchTree {
private:
    Node* root = nullptr;

    static Node* insertRec(Node* node, const Course& c) {
        if (!node) return new Node(c);
        if (c.code < node->course.code) node->left = insertRec(node->left, c);
        else if (c.code > node->course.code) node->right = insertRec(node->right, c);
        else node->course = c;
        return node;
    }

    static const Course* findRec(Node* node, const string& code) {
        if (!node) return nullptr;
        if (code == node->course.code) return &node->course;
        return code < node->course.code ? findRec(node->left, code)
            : findRec(node->right, code);
    }

    static void inorderRec(Node* node) {
        if (!node) return;
        inorderRec(node->left);
        cout << node->course.code << ", " << node->course.title << '\n';
        inorderRec(node->right);
    }

    static void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    ~BinarySearchTree() { 
        clear(); 
    }

    void insert(const Course& c) { 
        root = insertRec(root, c); 
    }
    const Course* find(const string& code) const { 
        return findRec(root, code); 
    }
    void printInorder() const { 
        inorderRec(root); 
    }
    void clear() { 
        clearRec(root); 
        root = nullptr; 
    }
    bool empty() const { 
        return root == nullptr; 
    }
};

//bst object
BinarySearchTree bst;

//trims a string to get rid of whitespace
static inline string trim(const string& s) {
    size_t b = 0, e = s.size();
    while (b < e && isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

//converts characters in a string to uppercase
static inline void to_upper_inplace(string& s) {
    for (char& ch : s) ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
}


//Input: Design code to correctly read the course data file. 
void loadDataStructure(string fileName) {
    //reads the course file that was inputted by the user
    ifstream file(fileName);
    stringstream buffer;
    string data;

    if (file.is_open()) {
        buffer << file.rdbuf();
        data = buffer.str();
        file.close();
    }
    //prints errors message when the method could not successfully open the file
    else {
        cerr << "Error: Could not open the " << fileName << ".csv file" << endl;
        return;
    }

    /*
    const string data = R"(MATH201,Discrete Mathematics,,
CSCI300,Introduction to Algorithms,CSCI200,MATH201
CSCI350,Operating Systems,CSCI300,
CSCI101,Introduction to Programming in C++,CSCI100,
CSCI100,Introduction to Computer Science,,
CSCI301,Advanced Programming in C++,CSCI101,
CSCI400,Large Software Development,CSCI301,CSCI350
CSCI200,Data Structures,CSCI101,)";
*/
    
    //clears the data structure so that a new file can be inputted
    bst.clear();

    //reads the data inside the file and parses each line and separates course information into fields, storing them into a vector<string>.
    istringstream in(data);
    string line;
    size_t rows = 0;

    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        vector<string> fields;
        string cell;
        stringstream ss(line);
        while (getline(ss, cell, ',')) fields.push_back(trim(cell));

        if (fields.size() < 2) continue;

        Course c;
        c.code = fields[0];
        c.title = fields[1];

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) c.prereqs.push_back(fields[i]);
        }

        to_upper_inplace(c.code);
        for (string& p : c.prereqs) to_upper_inplace(p);

        bst.insert(c);
        ++rows;
    }

    cout << "Successfully loaded " << rows << " rows into Binary Tree.\n\n";
}

//prints all course data in alphanumeric order
void printCourseList() {
    if (bst.empty()) {
        cout << "Catalog is empty. Load the data structure first (try option 1).\n\n";
        return;
    }
    cout << "Course List (alphanumerically sorted):\n\n";
    bst.printInorder();
    cout << '\n';
}

//prints data for a particular course after entering the course code
void printCourse() {
    //if you forgot to load the file into the BST
    if (bst.empty()) {
        cout << "Catalog is empty. Load the data structure first (try option 1).\n\n";
        return;
    }
    cout << "Enter course code (e.g., CSCI200): ";
    string code;
    cin >> code;
    to_upper_inplace(code);

    const Course* c = bst.find(code);
    if (!c) {
        cout << code << " not found.\n\n";
        return;
    }

    cout << c->code << ", " << c->title << '\n';
    cout << "Prerequisites: ";
    if (c->prereqs.empty()) {
        cout << "None\n\n";
    }
    else {
        for (size_t i = 0; i < c->prereqs.size(); ++i) {
            const string& pcode = c->prereqs[i];
            const Course* pc = bst.find(pcode); // looks up prerequisite in BST
            if (pc) {
                cout << pc->code << " (" << pc->title << ")";
            }
            else {
                // prerequisite code wasn’t found in the tree
                cout << pcode << " (not found)";
            }
            if (i + 1 < c->prereqs.size()) cout << ", ";
        }
        cout << "\n\n";
    }
}





//MAIN
int main()
{
    //Menu: Design code to create a menu that prompts a user for menu options. 
    int choice = 0;
    cout << "Welcome to the course planner.\n";

    auto showMenu = [] {
        cout << "\n"
            << "1. Load Data Structure.\n"
            << "2. Print Course List.\n"
            << "3. Print Course.\n"
            << "9. Exit\n\n"
            << "What would you like to do? ";
    };

    while (true) {
        showMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout <<"That input is not a valid option.\n\n";
            continue;
        }

        //switch statement to allow user to make a menu selection
        switch (choice) {
        case 1: {
            string filename;
            cout << "What is the name of the csv file? (e.g. courses.csv): ";
            cin >> ws;
            getline(cin, filename);
            cout << "Loading...\n\n";
            loadDataStructure(filename);
            break;
        }
        case 2:
            cout << "Printing course list...\n\n";
            printCourseList();
            break;
        case 3:
            cout << "printing course\n\n";
            printCourse();
            break;
        case 9:
            cout << "Thank you for using the course planner!\n";
            return 0; 
        default:
            cout << choice << " is not a valid option.\n\n";
            break;
        }
    }



}


