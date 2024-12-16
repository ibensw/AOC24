#pragma once

#include <cstddef>

template <typename T = std::size_t> struct Point {
    T x;
    T y;

    Point operator+(Point other) const noexcept
    {
        return {x + other.x, y + other.y};
    }
    Point operator-(Point other) const noexcept
    {
        return {x - other.x, y - other.y};
    }
    template <typename Factor> Point operator*(Factor f) const noexcept
    {
        return {x * f, y * f};
    }
    template <typename Factor> Point operator/(Factor f) const noexcept
    {
        return {x / f, y / f};
    }
    bool operator<(Point other) const noexcept
    {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
    bool operator==(Point other) const noexcept
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(Point other) const noexcept
    {
        return !(*this == other);
    }
    Point &operator+=(Point other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point &operator-=(Point other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template <typename Factor> Point &operator*=(Factor f) noexcept
    {
        x *= f;
        y *= f;
        return *this;
    }
    template <typename Factor> Point &operator/=(Factor f) noexcept
    {
        x /= f;
        y /= f;
        return *this;
    }

    template <typename U> operator Point<U>() const
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    static constexpr Point UP = {0, static_cast<T>(-1)};
    static constexpr Point DOWN = {0, 1};
    static constexpr Point LEFT = {static_cast<T>(-1), 0};
    static constexpr Point RIGHT = {1, 0};
};

template <typename F, typename T> Point<T> operator*(F f, Point<T> p)
{
    return p * f;
}
template <typename F, typename T> Point<T> operator/(F f, Point<T> p)
{
    return p / f;
}
