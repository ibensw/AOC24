#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <ranges>
#include <utility>
#include <vector>

std::pair<std::vector<unsigned>, std::vector<unsigned>> parseInput(const std::filesystem::path &path)
{
    std::vector<unsigned> left;
    std::vector<unsigned> right;

    std::ifstream inputFile(path);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open input file");
    }

    while (!inputFile.eof()) {
        unsigned l;
        unsigned r;
        inputFile >> l >> r;
        if (!inputFile.fail()) {
            left.push_back(l);
            right.push_back(r);
        }
    }

    return std::make_pair(left, right);
}

unsigned findTotalDiff(std::pair<std::vector<unsigned>, std::vector<unsigned>> &input)
{
    auto diff = [](unsigned a, unsigned b) { return a > b ? a - b : b - a; };

    auto &[left, right] = input;
    std::ranges::sort(left);
    std::ranges::sort(right);

    unsigned totalDiff = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        totalDiff += diff(left[i], right[i]);
    }
    return totalDiff;
}

int main(int argc, char *argv[])
{
    auto input = parseInput(argv[1]);
    fmt::print("Total difference: {}\n", findTotalDiff(input));
}
