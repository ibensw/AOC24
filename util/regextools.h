#pragma once

#include <ctre.hpp>
#include <string_view>

template <typename Regex, typename Func> std::string_view forEachMatch(Regex regex, std::string_view input, Func func)
{
    for (; auto match = regex.search(input); input = input.substr(match.end() - input.begin())) {
        func(match);
    }

    return input;
}
