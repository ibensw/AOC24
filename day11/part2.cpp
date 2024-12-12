#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <map>
#include <stdexcept>
#include <vector>

using Stone = unsigned long;
using StoneRow = std::map<Stone, unsigned long>; // stone -> count

std::tuple<bool, Stone, Stone> splitStone(Stone stone)
{
    int count = 1;
    Stone base = 10;
    Stone half = 1;
    while (stone >= base) {
        base *= 10;
        ++count;
        if (count % 2 == 0) {
            half *= 10;
        }
    }
    if (count & 1) { // odd
        return {false, 0, 0};
    }
    return {true, stone / half, stone % half};
}

StoneRow blinkStones(const StoneRow &stones)
{
    StoneRow newrow;
    for (auto &[stone, count] : stones) {
        if (stone == 0) {
            newrow[1] += count;
        } else if (auto [split, left, right] = splitStone(stone); split) {
            newrow[left] += count;
            newrow[right] += count;
        } else {
            newrow[stone * 2024] += count;
        }
    }
    return newrow;
}

StoneRow readFile(const std::filesystem::path &filePath)
{
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    StoneRow stones;

    while (!file.eof()) {
        Stone s;
        file >> s;
        if (!file.fail()) {
            stones[s]++;
        }
    }
    return stones;
}

unsigned long countStones(const StoneRow &stones)
{
    unsigned long result = 0;
    for (auto &[_, count] : stones) {
        auto backup = result;
        result += count;
        if (backup > result) {
            throw std::runtime_error("Overflow detected");
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    auto stones = readFile(argv[1]);
    auto iterations = argc > 2 ? std::atoi(argv[2]) : 75;
    fmt::print("Stones: {}\n", stones);
    for (int i = 0; i < iterations; ++i) {
        stones = blinkStones(stones);
        fmt::print("Number of stones after {} blinks: {}\n", i + 1, countStones(stones));
    }

    return 0;
}
