#include "point.h"
#include "readfile.h"
#include "regextools.h"
#include <algorithm>
#include <execution>
#include <fmt/format.h>
#include <iostream>
#include <map>
#include <set>

struct Robot {
    Point<long> position;
    Point<long> direction;

    bool operator==(const Robot &other) const
    {
        return position == other.position && direction == other.direction;
    }
};

struct Field {
    long width{11};
    long height{7};
    std::vector<Robot> robots;

    void moveRobots(long times)
    {
        std::for_each(std::execution::par_unseq, robots.begin(), robots.end(), [this, times](Robot &robot) {
            robot.position += robot.direction * times;
            robot.position.x += robot.position.x < 0 ? times * width : 0;
            robot.position.y += robot.position.y < 0 ? times * height : 0;
            robot.position.x %= width;
            robot.position.y %= height;
        });
    }

    void print() const
    {
        std::map<Point<long>, std::size_t> count;
        std::for_each(robots.begin(), robots.end(), [&count](const Robot &robot) { count[robot.position]++; });
        for (long y = 0; y < height; ++y) {
            for (long x = 0; x < width; ++x) {
                if (count.contains({x, y})) {
                    fmt::print("{}", count[{x, y}]);
                } else {
                    fmt::print(".");
                }
            }
            fmt::print("\n");
        }
    }

    auto findTree()
    {
        auto treeScore = [this](const std::vector<Robot> &r) {
            std::set<Point<long>> positions;
            for (const auto &robot : r) {
                positions.insert(robot.position);
            }
            return positions.size();
        };

        auto r = robots;
        auto best = r;
        auto bestMoves = 0;
        auto bestScore = 0;
        auto moves = 0;

        do {
            auto score = treeScore(robots);
            if (score > bestScore) {
                print();
                fmt::print("Score: {}, moves: {}\n", score, moves);
                best = r;
                bestMoves = moves;
                bestScore = score;
            }
            moveRobots(1);
            moves++;
        } while (r != robots);
        fmt::print("Tested {} moves, best score: {}\n", moves, bestScore);
        return bestMoves;
    }
};

Field parseField(const RawData &data, long width, long height)
{
    Field result{width, height};
    std::string_view in = {data.data(), data.size()};
    forEachMatch(ctre::search<R"xxx(p=(?<px>-?\d+),(?<py>-?\d+) v=(?<vx>-?\d+),(?<vy>-?\d+)\n)xxx">, in,
                 [&result](auto &match) {
                     auto &robot = result.robots.emplace_back();
                     robot.position.x = match.template get<"px">().template to_number<long>() % result.width;
                     robot.position.y = match.template get<"py">().template to_number<long>() % result.height;
                     robot.direction.x = match.template get<"vx">().template to_number<long>() % result.width;
                     robot.direction.y = match.template get<"vy">().template to_number<long>() % result.height;
                 });
    return result;
}

int main(int argc, char *argv[])
{
    auto input = parseField(readFile(argv[1]), std::atoi(argv[2]), std::atoi(argv[3]));
    input.print();

    auto moves = input.findTree();
    input.print();
    fmt::print("After {} moves\n", moves);
}
