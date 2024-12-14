#include "point.h"
#include "readfile.h"
#include "regextools.h"
#include <algorithm>
#include <execution>
#include <fmt/format.h>
#include <map>

struct Robot {
    Point<long> position;
    Point<long> direction;
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

    std::array<std::size_t, 4> countQuadrants() const
    {
        std::array<std::size_t, 4> quadrants;
        for (const Robot &robot : robots) {
            auto midWidth = width / 2;
            auto midHeight = height / 2;
            if (robot.position.x < midWidth && robot.position.y < midHeight) {
                ++quadrants[0];
            } else if (robot.position.x > midWidth && robot.position.y < midHeight) {
                ++quadrants[1];
            } else if (robot.position.x < midWidth && robot.position.y > midHeight) {
                ++quadrants[2];
            } else if (robot.position.x > midWidth && robot.position.y > midHeight) {
                ++quadrants[3];
            }
        }
        return quadrants;
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
    // input.print();
    input.moveRobots(100);
    fmt::print("\nAfter 100 steps:\n");
    // input.print();
    auto [tl, tr, bl, br] = input.countQuadrants();
    fmt::print("{:>4} | {:>4}\n-----------\n{:>4} | {:>4}\n", tl, tr, bl, br);
    fmt::print("Safety = {}\n", tl * tr * bl * br);
}
