#include "readfile.h"
#include "regextools.h"
#include "table.h"
#include <algorithm>
#include <fmt/format.h>
#include <numeric>
#include <set>
#include <utility>

struct Input {
    Point a;
    Point b;
    Point price;
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
            item.price.x = match.template get<"pricex">().template to_number<std::size_t>() + 10000000000000;
            item.price.y = match.template get<"pricey">().template to_number<std::size_t>() + 10000000000000;
        });
    return result;
}

struct Path {
    std::size_t a;
    std::size_t b;
    inline auto cost() const
    {
        return a * 3 + b;
    }

    inline bool operator<(const Path &other) const
    {
        return cost() < other.cost();
    }
};

bool simplify(std::size_t &a, std::size_t &b, std::size_t &end)
{
    auto g = std::gcd(a, b);
    if (end % g != 0) {
        return false;
    }
    a /= g;
    b /= g;
    end /= g;
    return true;
}

bool simplify(Input &i)
{
    return simplify(i.a.x, i.b.x, i.price.x) && simplify(i.a.y, i.b.y, i.price.y);
}

Point reduceLine(Point diff, Point b)
{
    auto moveTimes = std::min(b.x / diff.x, b.y / diff.y);
    return b - moveTimes * diff;
}

unsigned long solveMachinePart(Point a2, Point b1, Point b2)
{
    long determinant = a2.x * (b2.y - b1.y);
    determinant -= a2.y * (b2.x - b1.x);
    if (determinant <= 0) {
        // todo: fix if lines are subsets
        return 0;
    }
    auto t = (b1.x * (b2.y - b1.y) - b1.y * (b2.x - b1.x)) / determinant;
    return t;
}

unsigned long solveMachine(const Input &machine)
{
    auto r1 = reduceLine(machine.b, machine.price);
    auto r2 = reduceLine(machine.a, machine.price);
    auto t1 = solveMachinePart(machine.a, r1, r1 + machine.b);
    auto t2 = solveMachinePart(machine.b, r2, r2 + machine.a);

    auto i1 = machine.a * t1;
    auto u1 = (machine.price - i1).x / machine.b.x;
    auto v1 = i1 + u1 * machine.b;
    auto price1 = t1 * 3 + u1;
    if (v1 != machine.price) {
        price1 = 0;
    }

    auto i2 = machine.b * t2;
    auto u2 = (machine.price - i2).x / machine.a.x;
    auto v2 = i2 + u2 * machine.a;
    auto price2 = t2 + u2 * 3;
    if (v2 != machine.price) {
        price2 = 0;
    }

    if (t1 && t2) {
        return std::min(price1, price2);
    } else if (t1) {
        return price1;
    } else {
        return price2;
    }
}

int main(int argc, char *argv[])
{
    auto machines = parseInput(readFile(argv[1]));

    unsigned long totalCost = 0;
    for (auto &machine : machines) {
        if (true) {
            auto cost = solveMachine(machine);
            fmt::print("Cost for machine {}\n", cost);
            totalCost += cost;
        } else {
            fmt::print("Invalid machine\n");
        }
    }
    fmt::print("Total cost: {}\n", totalCost);
    return 0;
}
