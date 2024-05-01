#ifndef JSON_H
#define JSON_H

#include <vector>
#include <filesystem>

std::vector<int> read_json(std::filesystem::path path);

#endif