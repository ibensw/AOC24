#include "readfile.h"
#include "table.h"
#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <set>

struct GameState {
    Table map;
    Point<> start;
    Point<> end;
};

GameState parseInput(RawData &data)
{
    GameState result{data};
    int startEnd = 0;
    for (std::size_t y = 0; y < result.map.height; ++y) {
        for (std::size_t x = 0; x < result.map.width; ++x) {
            char c = result.map.get(x, y);
            if (c == 'S') {
                result.start = {x, y};
                startEnd |= 1;
            } else if (c == 'E') {
                result.end = {x, y};
                startEnd |= 2;
            }
        }
    }
    if (startEnd != 3) {
        throw std::runtime_error("No start or end point found");
    }
    return result;
}

auto findPath(const GameState &state)
{
    struct Reindeer {
        Point<> position;
        Point<int> direction;
        unsigned cost{};
        inline bool operator<(const Reindeer &other) const
        {
            return cost < other.cost;
        }
    };
    auto neighbors = [&](const Reindeer &r) {
        std::vector<Reindeer> result;
        for (const auto &dir : {Point<int>::UP, Point<int>::DOWN, Point<int>::LEFT, Point<int>::RIGHT}) {
            Point<> next = r.position + dir;
            if (state.map.contains(next) && state.map.get(next) != '#') {
                auto &neighbor = result.emplace_back(Reindeer{next, dir, 1});
                if (dir != r.direction) {
                    neighbor.cost += 1000;
                }
            }
        }
        return result;
    };

    std::set<Point<>> visited;
    std::multiset<Reindeer> reindeers;
    reindeers.insert({state.start, Point<int>::RIGHT});

    while (!reindeers.empty()) {
        Reindeer rudolph = *reindeers.begin();
        reindeers.erase(reindeers.begin());
        visited.insert(rudolph.position);

        if (rudolph.position == state.end) {
            return rudolph.cost;
        }

        for (auto neighbor : neighbors(rudolph)) {
            if (!visited.contains(neighbor.position)) {
                neighbor.cost += rudolph.cost;
                auto n = reindeers.insert(neighbor);
            }
        }
    }
    throw std::runtime_error("No path found");
}

int main(int argc, char *argv[])
{
    auto data = readFile(argv[1]);
    auto game = parseInput(data);
    fmt::print("Total cost: {}\n", findPath(game));
    return 0;
}
