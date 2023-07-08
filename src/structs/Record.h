//
// Created by p3dr4 on 2023-07-08.
//

#ifndef INC_8042_PROJECT_RECORD_H
#define INC_8042_PROJECT_RECORD_H

#include <vector>
#include <string>

struct Record {
    int offset{};
    std::string line;
    std::vector<std::string> row;

    Record() = default;

    explicit Record(std::string line) : line(std::move(line)) {
        row = split();
    }

    Record(int offset, std::string line) : offset(offset), line(std::move(line)) {
        row = split();
    }

    // split string by delimiter
    std::vector<std::string> split() {
        std::string temp;
        for (char i: line) {
            if (i == '|') {
                row.push_back(temp);
                temp = "";
            } else {
                temp += i;
            }
        }
        return row;
    }

    std::string str() const {
        // restore the line to original format
        std::string temp;
        for (const auto &i: row) {
            temp += i + "|";
        }
        temp.pop_back();
        return temp;
    }

};

#endif //INC_8042_PROJECT_RECORD_H
