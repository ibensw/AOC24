#pragma once

#include "point.h"
#include <vector>

class Table
{
  public:
    Table(std::vector<char> &d) : data(std::move(d))
    {
        auto newline = std::ranges::find(data, '\n');
        width = std::distance(data.begin(), newline);
        height = data.size() / (width + 1);
    }

    template <typename Tranform> Table(std::vector<char> &d, Tranform transform) : Table(d)
    {
        for (auto &c : data) {
            c = transform(c);
        }
    }

    inline bool contains(std::size_t x, std::size_t y) const noexcept
    {
        return x < width && y < height;
    }
    inline bool contains(Point<> p) const noexcept
    {
        return contains(p.x, p.y);
    }

    inline char &get(std::size_t x, std::size_t y) noexcept
    {
        return data[y * (width + 1) + x];
    }
    inline char get(std::size_t x, std::size_t y) const noexcept
    {
        return data[y * (width + 1) + x];
    }

    inline char &get(Point<> p) noexcept
    {
        return get(p.x, p.y);
    }
    inline char get(Point<> p) const noexcept
    {
        return get(p.x, p.y);
    }

    std::size_t width;
    std::size_t height;
    std::vector<char> data;
};
