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

void print(const Warehouse &warehouse)
{
    std::string_view tableView = {warehouse.map.data.data(), warehouse.map.data.size()};
    fmt::print("{}\n", tableView);
    fmt::print("Robot position: ({}, {})\n", warehouse.robot.x, warehouse.robot.y);
}

using Movement = std::vector<Point<int>>;

std::tuple<Warehouse, Movement> parseInput(const RawData &data)
{
    auto [map, movements] = [&] {
        std::string_view in = {data.data(), data.size()};
        auto splitted = split(in, "\n\n");
        return std::make_tuple(splitted[0], splitted[1]);
    }();
    std::vector<char> mapData;
    mapData.reserve(map.size() * 2);
    for (auto c : map) {
        switch (c) {
            case '#':
                mapData.push_back('#');
                mapData.push_back('#');
                break;
            case '.':
                mapData.push_back('.');
                mapData.push_back('.');
                break;
            case 'O':
                mapData.push_back('[');
                mapData.push_back(']');
                break;
            case '@':
                mapData.push_back('@');
                mapData.push_back('.');
                break;
            default:
                mapData.push_back(c);
        }
    }

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
    // print(warehouse);
    auto value = warehouse.map.get(newPos);
    switch (value) {
        case '#':
            return false;
        case '.':
            std::swap(warehouse.map.get(newPos), warehouse.map.get(p));
            return true;
        case '[':
        case ']': {
            if ((value == ']' && move == Point<int>::LEFT) || (value == '[' && move == Point<int>::RIGHT)) {
                if (!movePos(warehouse, move, newPos + move) || !movePos(warehouse, move, newPos)) {
                    return false;
                }
                std::swap(warehouse.map.get(newPos), warehouse.map.get(p));
                return true;
            } else if (move == Point<int>::UP || move == Point<int>::DOWN) {
                auto otherPos = newPos + (value == '[' ? Point<int>::RIGHT : Point<int>::LEFT);
                auto result = movePos(warehouse, move, newPos) && movePos(warehouse, move, otherPos);
                std::swap(warehouse.map.get(newPos), warehouse.map.get(p));
                return result;
            }
            return false;
        }
        default:
            fmt::print("Invalid cell type at position {}, {}: {}\n", newPos.x, newPos.y, warehouse.map.get(newPos));
            throw std::runtime_error("Invalid cell type");
    }
}

void moveStep(Warehouse &warehouse, Movement::value_type move)
{
    auto copy = warehouse;
    if (movePos(copy, move, warehouse.robot)) {
        warehouse = copy;
        warehouse.robot = warehouse.robot + move;
    }
}

unsigned long countGps(const Warehouse &warehouse)
{
    unsigned long count = 0;
    for (std::size_t y = 0; y < warehouse.map.height; ++y) {
        for (std::size_t x = 0; x < warehouse.map.width; ++x) {
            if (warehouse.map.get(x, y) == '[') {
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
