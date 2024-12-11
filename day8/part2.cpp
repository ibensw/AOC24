#include "readfile.h"
#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <map>
#include <set>
#include <utility>

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
Point &operator+=(Point &a, const Point &b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}
Point &operator-=(Point &a, const Point &b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

struct Table {
    Table(std::vector<char> &d) : data(std::move(d))
    {
        auto newline = std::ranges::find(data, '\n');
        width = std::distance(data.begin(), newline);
        height = data.size() / (width + 1);
        fmt::print("{}x{}\n", width, height);
    }

    std::vector<char> data;
    std::size_t width;
    std::size_t height;
    inline char get(std::size_t x, std::size_t y) const noexcept
    {
        return data[y * (width + 1) + x];
    }

    inline bool contains(std::size_t x, std::size_t y) const noexcept
    {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

std::set<Point> findAntiNodes(const Table &table)
{
    std::set<Point> points;
    std::map<char, std::vector<Point>> antennas;
    for (std::size_t y = 0; y < table.height; ++y) {
        for (std::size_t x = 0; x < table.width; ++x) {
            char c = table.get(x, y);
            if (c >= '0' && c <= 'z') {
                antennas[c].emplace_back(x, y);
            }
        }
    }

    for (const auto &[letter, locations] : antennas) {
        for (auto start = locations.begin(); start != locations.end(); ++start) {
            points.insert(*start);
            for (auto test = start + 1; test != locations.end(); ++test) {
                auto diff = *test - *start;
                for (auto sp = *start - diff; table.contains(sp.x, sp.y); sp -= diff) {
                    points.insert(sp);
                }
                for (auto sp = *test + diff; table.contains(sp.x, sp.y); sp += diff) {
                    points.insert(sp);
                }
            }
        }
    }
    return points;
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table(input);

    auto result = findAntiNodes(table);
    fmt::print("Total anti-nodes: {}\n", result.size());
}
