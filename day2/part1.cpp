#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ranges>
#include <vector>

std::vector<std::vector<int>> parseInput(const std::filesystem::path &path)
{
    auto parseLine = [](const std::string &line) {
        std::istringstream iss(line);
        std::vector<int> result;
        int num;
        while (iss >> num) {
            result.push_back(num);
        }
        return result;
    };
    std::vector<std::vector<int>> input;

    if (std::ifstream file{path}) {
        for (std::string line; std::getline(file, line);) {
            input.push_back(parseLine(line));
        }
    } else {
        throw std::runtime_error("Could not open input file.");
    }

    return input;
}

int main(int argc, char *argv[])
{
    auto input = parseInput(argv[1]);

    auto isSafe = [](const std::vector<int> &row) {
        const int sign = row[0] < row[1] ? 1 : -1;
        auto isBad = [sign](int a, int b) {
            auto diff = (b - a) * sign;
            return diff <= 0 || diff > 3;
        };
        auto good = std::ranges::adjacent_find(row, isBad) == std::ranges::cend(row);
        fmt::print("Report {}: {}\n", row, good);
        return good;
    };

    fmt::print("Safe reports = {}\n", std::ranges::count_if(input, isSafe));
}
