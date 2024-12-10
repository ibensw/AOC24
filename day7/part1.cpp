#include "readfile.h"
#include "regextools.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ranges>
#include <set>

using IntType = unsigned long long;

struct InputLine {
    IntType result;
    std::vector<IntType> operands;
};

std::vector<InputLine> parseInput(const RawData &data)
{
    std::vector<InputLine> result;
    std::string_view in = {data.data(), data.size()};

    forEachMatch(ctre::search<"(?<result>\\d+):(?<operands>( \\d+)+)">, in, [&result](auto &match) {
        auto &line = result.emplace_back();
        line.result = match.template get<"result">().template to_number<IntType>();
        std::istringstream iss(match.template get<"operands">().str());
        IntType num;
        while (iss >> num) {
            line.operands.push_back(num);
        }
    });

    return result;
}

bool solve(IntType total, IntType current, std::vector<IntType>::const_iterator next,
           std::vector<IntType>::const_iterator end)
{
    if (next == end) {
        return total == current;
    }
    if (current > total) {
        return false;
    }
    return solve(total, current * *next, next + 1, end) || solve(total, current + *next, next + 1, end);
};

bool findSolution(const InputLine &line)
{
    return solve(line.result, line.operands[0], line.operands.begin() + 1, line.operands.end());
}

int main(int argc, char *argv[])
{
    auto input = parseInput(readFile(argv[1]));
    IntType totalGood = 0;
    for (const auto &line : input) {
        if (findSolution(line)) {
            totalGood += line.result;
        }
    }
    fmt::print("Found solution sum: {}\n", totalGood);
}
