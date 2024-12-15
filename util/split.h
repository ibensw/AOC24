#pragma once

#include <limits>
#include <string_view>
#include <vector>

std::vector<std::string_view> split(std::string_view str, std::string_view delimiter = "\n",
                                    std::size_t maxParts = std::numeric_limits<size_t>::max())
{
    std::vector<std::string_view> result;
    size_t start = 0;
    size_t end = 0;
    maxParts--;

    while (result.size() < maxParts && (end = str.find(delimiter, start)) != std::string_view::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
    }

    result.push_back(str.substr(start));
    return result;
}
