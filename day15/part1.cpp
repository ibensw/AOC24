#include "point.h"
#include "readfile.h"
#include "regextools.h"
#include "split.h"
#include "table.h"

#include <fmt/format.h>

struct Warehouse {
    Warehouse(RawData &data) : map(data)
    {
    }
    Table map;
    Point<> robot;
    std::vector<Point<>> boxes;
};

using Movement = std::vector<Point<int>>;

std::tuple<Warehouse, Movement> parseInput(const RawData &data)
{
    auto [map, movements] = [&] {
        std::string_view in = {data.data(), data.size()};
        auto splitted = split(in, "\n\n");
        return std::make_tuple(splitted[0], splitted[1]);
    }();
    std::vector<char> mapData(map.begin(), map.end());
    Warehouse warehouse(mapData);
    for (std::size_t y = 0; y < warehouse.map.height; ++y) {
        for (std::size_t x = 0; x < warehouse.map.width; ++x) {
            if (warehouse.map.get(x, y) == '@') {
                warehouse.robot = {x, y};
                break;
            }
        }
    }
    Movement moves;
    moves.reserve(movements.size());
    for (auto move : movements) {
        switch (move) {
            case '^':
                moves.push_back(Point<int>::UP);
                break;
            case 'v':
                moves.push_back(Point<int>::DOWN);
                break;
            case '>':
                moves.push_back(Point<int>::RIGHT);
                break;
            case '<':
                moves.push_back(Point<int>::LEFT);
                break;
            case '\n':
                break;
            default:
                fmt::print("Invalid movement: '{}'\n", move);
        }
    }
    return std::make_tuple(warehouse, moves);
}

template <typename T, typename U> inline Point<decltype(T{} + U{})> operator+(const Point<T> &a, const Point<U> &b)
{
    return {a.x + b.x, a.y + b.y};
}

bool movePos(Warehouse &warehouse, Movement::value_type move, Point<> p)
{
    Point<> newPos = p + move;
    switch (warehouse.map.get(newPos)) {
        case '#':
            return false;
        case '.':
            std::swap(warehouse.map.get(newPos), warehouse.map.get(p));
            return true;
        case 'O':
            if (movePos(warehouse, move, newPos)) {
                std::swap(warehouse.map.get(newPos), warehouse.map.get(p));
                return true;
            }
            return false;
        default:
            fmt::print("Invalid cell type at position {}, {}: {}\n", newPos.x, newPos.y, warehouse.map.get(newPos));
            throw std::runtime_error("Invalid cell type");
    }
}

void moveStep(Warehouse &warehouse, Movement::value_type move)
{
    if (movePos(warehouse, move, warehouse.robot)) {
        warehouse.robot = warehouse.robot + move;
    }
}

void print(const Warehouse &warehouse)
{
    std::string_view tableView = {warehouse.map.data.data(), warehouse.map.data.size()};
    fmt::print("{}\n", tableView);
    fmt::print("Robot position: ({}, {})\n", warehouse.robot.x, warehouse.robot.y);
}

unsigned long countGps(const Warehouse &warehouse)
{
    unsigned long count = 0;
    for (std::size_t y = 0; y < warehouse.map.height; ++y) {
        for (std::size_t x = 0; x < warehouse.map.width; ++x) {
            if (warehouse.map.get(x, y) == 'O') {
                count += x + y * 100;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    auto data = readFile(argv[1]);
    auto [warehouse, moves] = parseInput(data);

    print(warehouse);

    for (auto move : moves) {
        moveStep(warehouse, move);
    }

    print(warehouse);
    fmt::print("Total GPS distance: {}\n", countGps(warehouse));

    // fmt::print("Robot position: ({}, {})\n", warehouse.robot.x, warehouse.robot.y);
    // return 0;
}
