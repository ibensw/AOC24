#include <algorithm>
#include <ctre.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

std::vector<char> readFile(const std::filesystem::path &filePath)
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
    std::vector<char> data(fileSize);
    if (!file.read(data.data(), fileSize)) {
        throw std::runtime_error("Failed to read file: " + filePath.string());
    }

    return data;
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);

    auto regex = ctre::search<"mul\\((?<oper1>\\d{1,3}),(?<oper2>\\d{1,3})\\)">;
    unsigned total = 0;
    std::string_view in = {input.data(), input.size()};

    for (std::string_view in = {input.data(), input.size()}; auto match = regex.search(in); match) {
        auto oper1 = match.get<"oper1">().to_number<int>();
        auto oper2 = match.get<"oper2">().to_number<int>();
        int result = oper1 * oper2;
        total += result;
        fmt::print("{} = {}\n", match.str(), result);
        in = in.substr(match.end() - in.begin());
    }

    fmt::print("Total = {}\n", total);
}
