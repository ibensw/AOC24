#include "readfile.h"
#include "table.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <set>

template <> class fmt::formatter<Point>
{
  public:
    constexpr auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }
    template <typename Context> constexpr auto format(const Point &foo, Context &ctx) const
    {
        return format_to(ctx.out(), "({},{})", foo.x, foo.y);
    }
};

class BoolTable
{
  public:
    BoolTable(std::size_t w, std::size_t h) : width(w), height(h), data(width * height, false)
    {
    }

    bool get(std::size_t x, std::size_t y)
    {
        return data[y * width + x];
    }
    bool set(std::size_t x, std::size_t y, bool value = true)
    {
        return data[y * width + x] = value;
    }

    std::size_t width;
    std::size_t height;

    std::vector<bool> data;
};

struct Region {
    char id;
    std::size_t area{0};
    std::size_t perimeter;
};

Region sweepRegion(const Table &table, BoolTable &visited, Point start)
{
    std::set<Point> hFences;
    std::set<Point> vFences;
    auto value = table.get(start);
    Region region{value};
    std::set<Point> toExplore{start};
    fmt::print("Region {} found at {}, {}\n", value, start.x, start.y);
    while (!toExplore.empty()) {
        auto current = toExplore.begin();
        auto location = *current;
        toExplore.erase(toExplore.begin());

        visited.set(location.x, location.y, true);
        region.area++;

        for (const auto &direction : {UP, DOWN, LEFT, RIGHT}) {
            Point neighbor = location + direction;
            if (table.contains(neighbor) && table.get(neighbor) == value) {
                if (!visited.get(neighbor.x, neighbor.y)) {
                    toExplore.insert(neighbor);
                }
            } else {
                if (direction.x == 0) {
                    Point fence = {location.x, location.y + (direction.y == 1 ? 1 : 0)};

                    if (!vFences.contains({fence.x + 1, fence.y}) && !vFences.contains({fence.x - 1, fence.y})) {
                        region.perimeter++;
                    }

                    vFences.insert(fence);
                } else {
                    Point fence = {location.x + (direction.x == 1 ? 1 : 0), location.y};

                    if (!hFences.contains({fence.x, fence.y + 1}) && !hFences.contains({fence.x, fence.y - 1})) {
                        region.perimeter++;
                    }

                    hFences.insert(fence);
                }
            }
        }
    }
    return region;
}

std::vector<Region> findRegions(const Table &table)
{
    std::vector<Region> regions;
    BoolTable visited(table.width, table.height);
    for (std::size_t y = 0; y < table.height; ++y) {
        for (std::size_t x = 0; x < table.width; ++x) {
            if (!visited.get(x, y)) {
                regions.push_back(sweepRegion(table, visited, {x, y}));
            }
        }
    }
    return regions;
}

// unsigned long countFences(const std::set<Point> &segments)
// {
//     for (auto start = segments.begin(); start != segments.end(); ++start) {
//         auto test = start;
//         for (++test; test != segments.end(); ++test) {
//             bool xByOne = (start->x - test->x == 1) || (test->x - start->x == 1);
//             bool yByOne = (start->y - test->y == 1) || (test->y - start->y == 1);
//             if (xByOne ^ yByOne) {
//             }
//         }
//     }
// }

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table(input);

    auto regions = findRegions(table);

    unsigned long long totalCost = 0;

    for (const auto &region : regions) {
        fmt::print("Region {}: Area={}, Perimeter={}\n", region.id, region.area, region.perimeter);
        totalCost += region.area * region.perimeter;
    }

    fmt::print("Total cost: {}\n", totalCost);

    return 0;
}
