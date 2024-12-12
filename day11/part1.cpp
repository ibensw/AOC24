#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <list>
#include <stdexcept>

using Stone = unsigned long;

std::list<Stone> readFile(const std::filesystem::path &filePath)
{
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::list<Stone> stones;

    while (!file.eof()) {
        Stone s;
        file >> s;
        if (!file.fail()) {
            stones.push_back(s);
        }
    }
    return stones;
}

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

void blinkStones(std::list<Stone> &stones)
{
    for (auto stone = stones.begin(); stone != stones.end(); ++stone) {
        if (*stone == 0) {
            *stone = 1;
        } else if (auto [split, left, right] = splitStone(*stone); split) {
            *stone = left;
            stone++;
            stone = stones.insert(stone, right);
        } else {
            *stone *= 2024;
        }
    }
}

int main(int argc, char *argv[])
{
    auto stones = readFile(argv[1]);
    fmt::print("Stones: {}\n", stones);
    for (int blink = 0; blink < 25; ++blink) {
        blinkStones(stones);
    }
    fmt::print("Number of stones after 25 blinks: {}\n", stones.size());

    return 0;
}
