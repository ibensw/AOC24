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

    inline std::vector<char> getRow(std::size_t y) const noexcept
    {
        return std::vector<char>(data.begin() + y * width, data.begin() + (y + 1) * width);
    }
    inline std::vector<char> getColumn(std::size_t x) const noexcept
    {
        std::vector<char> result;
        for (std::size_t y = 0; y < height; ++y) {
            result.push_back(get(x, y));
        }
        return result;
    }

    inline std::vector<char> getDiagonalSlash(int i) const noexcept
    {
        std::vector<char> result;
        i -= height;
        for (int x = 0; x < height; ++x) {
            if (x + i >= 0 && x + i < width) {
                result.push_back(get(x + i, x));
            }
        }
        return result;
    }
    inline std::vector<char> getDiagonalBackslash(int i) const noexcept
    {
        std::vector<char> result;
        for (int x = 0; x < height; ++x) {
            if (i - x >= 0 && i - x < width) {
                result.push_back(get(i - x, x));
            }
        }
        return result;
    }
};

int count(const std::vector<char> &row, std::string_view pattern)
{
    int count = 0;
    auto start = row.cbegin();

    while (true) {
        start = std::search(start, row.cend(), pattern.begin(), pattern.end());
        if (start != row.cend()) {
            ++count;
            start += pattern.size();
        } else {
            break;
        }
    };

    fmt::print("{} - {}: {}\n", row, pattern, count);
    return count;
}

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    Table table(input);

    // quick and dirty implementation
    // horizontal
    auto sum = 0;
    for (std::size_t y = 0; y < table.height; ++y) {
        sum += count(table.getRow(y), "XMAS");
        sum += count(table.getRow(y), "SAMX");
    }
    for (std::size_t x = 0; x < table.width; ++x) {
        sum += count(table.getColumn(x), "XMAS");
        sum += count(table.getColumn(x), "SAMX");
    }
    for (std::size_t x = 0; x < table.width + table.height - 1; ++x) {
        sum += count(table.getDiagonalSlash(x), "XMAS");
        sum += count(table.getDiagonalSlash(x), "SAMX");
    }
    for (std::size_t x = 0; x < table.width + table.height - 1; ++x) {
        sum += count(table.getDiagonalBackslash(x), "XMAS");
        sum += count(table.getDiagonalBackslash(x), "SAMX");
    }

    fmt::print("Sum: {}\n", sum);
}
