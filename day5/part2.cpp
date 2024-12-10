#include "readfile.h"
#include "regextools.h"
#include <algorithm>
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

// void makeValid(const std::vector<std::pair<int, int>> &rules, std::vector<int> &update)
// {
//     do { // do-while since we know it's not valid from the start
//         std::next_permutation(update.begin(), update.end());
//     } while (!isValid(rules, update));
// }

std::vector<std::pair<int, int>> makeRules(std::vector<std::pair<int, int>> &rules,
                                           std::vector<int>::iterator updateBegin, std::vector<int>::iterator updateEnd)
{
    std::vector<std::pair<int, int>> useFullRules;
    std::copy_if(rules.begin(), rules.end(), std::back_inserter(useFullRules), [=](const auto &rule) {
        return std::find(updateBegin, updateEnd, rule.first) != updateEnd &&
               std::find(updateBegin, updateEnd, rule.second) != updateEnd;
    });
    return useFullRules;
}

void makeValid(std::vector<std::pair<int, int>> rules, std::vector<int>::iterator first,
               std::vector<int>::iterator last)
{
    if (std::distance(first, last) < 2) {
        return;
    }

    auto newFirst = first;
    for (auto page = first; page != last; ++page) {
        bool isFirst = true;
        for (auto &rule : rules) {
            if (rule.second == *page) {
                isFirst = false;
            }
        }
        if (isFirst) {
            newFirst = page;
        }
    }

    std::swap(*first, *newFirst);
    makeValid(makeRules(rules, first + 1, last), first + 1, last);
}

int main(int argc, char *argv[])
{
    auto input = parseInput(readFile(argv[1]));
    auto sumValid = 0;
    for (auto &update : input.updates) {
        auto rules = makeRules(input.rules, update.begin(), update.end());
        bool valid = isValid(rules, update);
        fmt::print("{}: {}\n", update, valid ? "valid" : "invalid");
        if (!valid) {
            makeValid(rules, update.begin(), update.end());
            fmt::print("-> {}\n", update);
            if (!isValid(rules, update)) {
                isValid(rules, update);
                throw std::runtime_error("This one is wrong!");
            }
            sumValid += update[update.size() / 2];
        }
    }
    fmt::print("Sum of valid updates: {}\n", sumValid);
    return 0;
}
