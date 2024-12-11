#include "readfile.h"
#include <algorithm>
#include <fmt/format.h>
#include <set>

struct Point {
    int x;
    int y;
};
Point operator+(Point a, Point b)
{
    return {a.x + b.x, a.y + b.y};
}
Point operator-(Point a, Point b)
{
    return {a.x - b.x, a.y - b.y};
}
Point operator*(Point a, int b)
{
    return {a.x * b, a.y * b};
}
Point operator*(int a, Point b)
{
    return b * a;
}
bool operator<(const Point &a, const Point &b) noexcept
{
    if (a.x == b.x) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

static constexpr Point UP = {-1, 0};
static constexpr Point DOWN = {1, 0};
static constexpr Point LEFT = {0, -1};
static constexpr Point RIGHT = {0, 1};

struct Table {
    Table(std::vector<char> &d) : data(std::move(d))
    {
        auto newline = std::ranges::find(data, '\n');
        width = std::distance(data.begin(), newline);
        height = data.size() / (width + 1);
        fmt::print("{}x{}\n", width, height);
        std::for_each(data.begin(), data.end(), [](char &c) { c -= '0'; });
    }

    std::vector<char> data;
    std::size_t width;
    std::size_t height;
    inline int get(std::size_t x, std::size_t y) const noexcept
    {
        return data[y * (width + 1) + x];
    }

    void print() const
    {
        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                fmt::print("{}", get(x, y));
            }
            fmt::print("\n");
        }
    }
};

int findPaths(const Table &table, Point start)
{
    std::set<Point> visited;
    std::set<Point> toVisit = {start};

    while (!toVisit.empty()) {
        auto current = *toVisit.begin();
        auto currentHeight = table.get(current.x, current.y);
        toVisit.erase(toVisit.begin());
        visited.insert(current);
        if (currentHeight < 9) {
            for (const auto &direction : {UP, DOWN, LEFT, RIGHT}) {
                Point next = current + direction;
                if (next.x >= 0 && next.x < table.width && next.y >= 0 && next.y < table.height &&
                    table.get(next.x, next.y) == currentHeight + 1 && !visited.contains(next)) {
                    toVisit.insert(next);
                }
            }
        }
    }

    return std::count_if(visited.begin(), visited.end(), [&](const Point &p) { return table.get(p.x, p.y) == 9; });
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table(input);
    // table.print();
    unsigned long long totalPaths = 0;
    for (int y = 0; y < table.height; ++y) {
        for (int x = 0; x < table.width; ++x) {
            if (table.get(x, y) == 0) {
                auto paths = findPaths(table, {x, y});
                totalPaths += paths;
            }
        }
    }
    fmt::print("Total paths to the top: {}\n", totalPaths);
    return 0;
}
