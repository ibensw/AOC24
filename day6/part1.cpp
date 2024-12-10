#include "readfile.h"
#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <set>
#include <utility>

struct Guard {
    std::pair<int, int> location{0, 0};
    std::pair<int, int> direction{0, 0};
};

// don't have access to cpp23 compiler for mdspan
struct Table {
    Guard load(const std::vector<char> &d)
    {
        auto newline = std::ranges::find(d, '\n');
        width = std::distance(d.begin(), newline);
        height = d.size() / (width + 1);
        fmt::print("{}x{}\n", width, height);
        data.reserve(d.size());
        std::for_each(d.begin(), d.end(), [this](char c) {
            if (c != '\n')
                data.push_back(c == '#');
        });

        Guard guard;
        for (std::size_t pos = 0; pos < d.size(); ++pos) {
            guard.location = {pos % (width + 1), pos / (width + 1)};
            switch (d[pos]) {
                case '^':
                    guard.direction = {0, -1};
                    return guard;
                case '>':
                    guard.direction = {1, 0};
                    return guard;
                case '<':
                    guard.direction = {-1, 0};
                    return guard;
                case 'v':
                    guard.direction = {0, 1};
                    return guard;
            }
        }
        return guard;
    }

    std::vector<bool> data;
    std::size_t width;
    std::size_t height;
    inline bool get(std::size_t x, std::size_t y) const noexcept
    {
        return data[y * width + x];
    }
};

bool step(const Table &table, Guard &guard)
{
    int newX = guard.location.first + guard.direction.first;
    int newY = guard.location.second + guard.direction.second;
    if (newX < 0 || newX >= table.width || newY < 0 || newY >= table.height) {
        return false;
    }

    if (table.get(newX, newY)) {
        guard.direction = {-guard.direction.second, guard.direction.first};
    } else {
        guard.location = {newX, newY};
    }
    return true;
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table;
    auto guard = table.load(input);
    int totalLocations = 0;
    std::set<std::pair<int, int>> visited;

    bool valid = true;

    for (std::size_t y = 0; y < table.height; ++y) {
        for (std::size_t x = 0; x < table.width; ++x) {
            fmt::print("{} ", table.get(x, y) ? "#" : ".");
        }
        fmt::print("\n");
    }
    do {
        fmt::print("Guard at {},{} facing {},{}\n", guard.location.first, guard.location.second, guard.direction.first,
                   guard.direction.second);
        visited.insert(guard.location);
    } while (step(table, guard));

    fmt::print("Total locations: {}\n", visited.size());
}
