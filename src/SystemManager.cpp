#include "SystemManager.h"
#include <stdexcept>
#include <fstream>
#include <sstream>


using namespace std;

DMS SystemManager::fillDMS(string value) {
    DMS dms{};

    int degreeSize;
    int minSecSize = 2;
    int charSize = 1;

    if (value.length() == 8) {
        degreeSize = 3;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(3, minSecSize));
        dms.seconds = stoi(value.substr(5, minSecSize));
        dms.direction = (char) ((value.substr(7, charSize))[0]);

    } else if (value.length() == 7) {
        degreeSize = 2;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(2, minSecSize));
        dms.seconds = stoi(value.substr(4, minSecSize));
        dms.direction = (char) ((value.substr(6, charSize))[0]);
    } else {
        throw invalid_argument("Invalid DMS format");
    }

    return dms;
}

vector<vector<string>> SystemManager::readScript(const string &filename) {
    ifstream input(filename);
    string line;
    vector<vector<string>> commands;
    while (getline(input, line)) {
        istringstream iss(line);
        vector<string> arguments;
        string argument;

        // split line by tab
        while (getline(iss, argument, '\t')) {
            arguments.push_back(argument);
        }

        if (arguments.empty() || arguments[0].empty() || arguments[0][0] == ';') {
            // empty or comment line, skip
            continue;
        }

        commands.push_back(arguments);
    }
    return commands;
}

//void read_record()
//{
//
//    // File pointer
//    fstream fin;
//
//    // Open an existing file
//    fin.open("reportcard.csv", ios::in);
//
//    // Get the roll number
//    // of which the data is required
//    int rollnum, roll2, count = 0;
//    cout << "Enter the roll number "
//         << "of the student to display details: ";
//    cin >> rollnum;
//
//    // Read the Data from the file
//    // as String Vector
//    vector<string> row;
//    string line, word, temp;
//
//    while (fin >> temp) {
//
//        row.clear();
//
//        // read an entire row and
//        // store it in a string variable 'line'
//        getline(fin, line);
//
//        // used for breaking words
//        stringstream s(line);
//
//        // read every column data of a row and
//        // store it in a string variable, 'word'
//        while (getline(s, word, ', ')) {
//
//            // add all the column data
//            // of a row to a vector
//            row.push_back(word);
//        }
//
//        // convert string to integer for comparision
//        roll2 = stoi(row[0]);
//
//        // Compare the roll number
//        if (roll2 == rollnum) {
//
//            // Print the found data
//            count = 1;
//            cout << "Details of Roll " << row[0] << " : \n";
//            cout << "Name: " << row[1] << "\n";
//            cout << "Maths: " << row[2] << "\n";
//            cout << "Physics: " << row[3] << "\n";
//            cout << "Chemistry: " << row[4] << "\n";
//            cout << "Biology: " << row[5] << "\n";
//            break;
//        }
//    }
//    if (count == 0)
//        cout << "Record not found\n";
//}
