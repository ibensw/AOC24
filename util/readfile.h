#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

using RawData = std::vector<char>;

RawData readFile(const std::filesystem::path &filePath)
{
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("File does not exist: " + filePath.string());
    }

    std::ifstream file(filePath, std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory and read the file
    RawData data(fileSize);
    if (!file.read(data.data(), fileSize)) {
        throw std::runtime_error("Failed to read file: " + filePath.string());
    }

    return data;
}
