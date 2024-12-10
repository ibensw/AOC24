#include "readfile.h"
#include "regextools.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ranges>
#include <set>

struct Input {
    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
};

Input parseInput(const RawData &data)
{
    Input result;
    std::string_view in = {data.data(), data.size()};

    in = forEachMatch(ctre::search<"(?<before>\\d+)\\|(?<after>\\d+)">, in, [&result](auto &match) {
        auto before = match.template get<"before">().template to_number<int>();
        auto after = match.template get<"after">().template to_number<int>();
        result.rules.emplace_back(before, after);
    });

    forEachMatch(ctre::search<"\\d+(,\\d+)+\\n">, in, [&result](auto &match) {
        auto &update = result.updates.emplace_back();
        std::istringstream iss(match.str());
        int num;
        while (iss >> num) {
            update.push_back(num);
            char c;
            iss >> c; // consume comma
        }
    });

    return result;
}

bool isValid(const std::vector<std::pair<int, int>> &rules, const std::vector<int> &update)
{
    std::set<int> seen;
    seen.insert(update[0]);
    for (auto page : update | std::views::drop(1)) {
        for (auto [before, after] : rules) {
            if (before == page && seen.find(after) != seen.end()) {
                return false;
            }
        }
        seen.insert(page);
    }
    return true;
}

int main(int argc, char *argv[])
{
    auto input = parseInput(readFile(argv[1]));
    auto sumValid = 0;
    for (const auto &update : input.updates) {
        bool valid = isValid(input.rules, update);
        fmt::print("{}: {}\n", update, valid ? "valid" : "invalid");
        if (valid) {
            sumValid += update[update.size() / 2];
        }
    }
    fmt::print("Sum of valid updates: {}\n", sumValid);
    return 0;
}
