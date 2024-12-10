#include <algorithm>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

std::vector<char> readFile(const std::filesystem::path &filePath)
{
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("File does not exist: " + filePath.string());
    }

    std::ifstream file(filePath, std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory and read the file
    std::vector<char> data(fileSize);
    if (!file.read(data.data(), fileSize)) {
        throw std::runtime_error("Failed to read file: " + filePath.string());
    }

    return data;
}

// don't have access to cpp23 compiler for mdspan
struct Table {
    Table(std::vector<char> d) : data(std::move(d))
    {
        auto newline = std::ranges::find(data, '\n');
        width = std::distance(data.begin(), newline) + 1;
        height = data.size() / width;
        fmt::print("{}x{}\n", width, height);
    }

    std::vector<char> data;
    std::size_t width;
    std::size_t height;
    inline const char &get(std::size_t x, std::size_t y) const noexcept
    {
        return data[y * width + x];
    }

    inline auto getTile(std::size_t x, std::size_t y)
    {
        std::array<std::array<char, 3>, 3> tile;

        for (std::size_t i = 0; i < 3; ++i) {
            tile[i] = {get(x + i, y), get(x + i, y + 1), get(x + i, y + 2)};
        }

        return tile;
    }
};

bool isXmas(const std::array<std::array<char, 3>, 3> &tile)
{
    if (tile[1][1] != 'A') {
        return false;
    }
    for (auto x : {0, 2}) {
        for (auto y : {0, 2}) {
            if (tile[x][y] != 'M' && tile[x][y] != 'S') {
                return false;
            }
        }
    }
    return (tile[0][0] != tile[2][2]) && (tile[0][2] != tile[2][0]);
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table(input);

    auto sum = 0;
    for (std::size_t y = 0; y < table.height - 2; ++y) {
        for (std::size_t x = 0; x < table.width; ++x) {
            auto tile = table.getTile(x, y);
            sum += isXmas(tile) ? 1 : 0;
        }
    }

    fmt::print("Sum: {}\n", sum);
}
