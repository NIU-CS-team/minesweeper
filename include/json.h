#ifndef JSON_H
#define JSON_H

#include <filesystem>
#include <vector>

std::vector<int> read_json(std::filesystem::path path);

#endif