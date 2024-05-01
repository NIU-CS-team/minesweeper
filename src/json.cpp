#include "json.h"

#include <fstream>
#include <string>

std::vector<int> read_json(std::filesystem::path path) {
    std::vector<int> result;
    std::ifstream fin(path);
    if (!fin.is_open()) {
        return result;
    }

    std::string temp;
    while (fin >> temp) {
        if (temp == "\"width\":") {
            fin >> temp;
            result.push_back(std::stoi(temp));
        } else if (temp == "\"height\":") {
            fin >> temp;
            result.push_back(std::stoi(temp));
        } else if (temp == "\"mine\":") {
            fin >> temp;
            result.push_back(std::stoi(temp));
        }
    }

    return result;
}