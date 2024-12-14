#include "readfile.h"
#include "regextools.h"
#include "table.h"
#include <algorithm>
#include <fmt/format.h>
#include <set>

struct Input {
    Point<> a;
    Point<> b;
    Point<> price;
};

std::vector<Input> parseInput(const RawData &data)
{
    std::vector<Input> result;
    std::string_view in = {data.data(), data.size()};
    forEachMatch(
        ctre::search<
            R"xxx(Button A: X\+(?<ax>\d+), Y\+(?<ay>\d+)\nButton B: X\+(?<bx>\d+), Y\+(?<by>\d+)\nPrize: X=(?<pricex>\d+), Y=(?<pricey>\d+))xxx">,
        in, [&result](auto &match) {
            auto &item = result.emplace_back();
            item.a.x = match.template get<"ax">().template to_number<std::size_t>();
            item.a.y = match.template get<"ay">().template to_number<std::size_t>();
            item.b.x = match.template get<"bx">().template to_number<std::size_t>();
            item.b.y = match.template get<"by">().template to_number<std::size_t>();
            item.price.x = match.template get<"pricex">().template to_number<std::size_t>();
            item.price.y = match.template get<"pricey">().template to_number<std::size_t>();
        });
    return result;
}

struct Path {
    int a;
    int b;
    inline auto cost() const
    {
        return a * 3 + b;
    }

    inline bool operator<(const Path &other) const
    {
        return cost() < other.cost();
    }
};

unsigned solveMachine(const Input &machine)
{
    std::vector<Path> possiblePaths;

    auto maxA = std::min(machine.price.x / machine.a.x, machine.price.y / machine.a.y);
    auto maxB = std::min(machine.price.x / machine.b.x, machine.price.y / machine.b.y);

    for (std::size_t a = 0; a <= maxA; ++a) {
        auto toGo = machine.price - a * machine.a;
        auto bSteps = toGo.x / machine.b.x;
        if (bSteps * machine.b == toGo) {
            fmt::print("Found path: A={}, B={}\n", a, bSteps);
            possiblePaths.emplace_back(a, bSteps);
        }
    }

    if (possiblePaths.empty()) {
        fmt::print("No possible path found\n");
        return 0;
    }

    return std::min_element(possiblePaths.begin(), possiblePaths.end())->cost();
}

int main(int argc, char *argv[])
{
    auto machines = parseInput(readFile(argv[1]));

    unsigned long long totalCost = 0;
    for (const auto &machine : machines) {
        totalCost += solveMachine(machine);
    }
    fmt::print("Total cost: {}\n", totalCost);

    return 0;
}
