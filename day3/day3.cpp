#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

void part1(const std::vector<std::string>& map) {
    size_t x = 0;
    size_t treeCount = 0;
    for (size_t y = 0; y < map.size(); y++) {
        auto& row = map[y];
        if (row[x] == '#') treeCount++;
        x = (x + 3) % row.size();
    }
    std::cout << "part 1: " << treeCount << "\n";
}

void part2(const std::vector<std::string>& map) {
    constexpr std::pair<size_t, size_t> slopes[] = {
        {1, 1},
        {3, 1},
        {5, 1},
        {7, 1},
        {1, 2},
    };

    size_t treeMult = 1;
    for (auto& slope : slopes) {
        size_t x = 0;
        size_t y = 0;
        size_t treeCount = 0;
        while (y < map.size()) {
            auto& row = map[y];
            if (row[x] == '#') treeCount++;
            x = (x + slope.first) % row.size();
            y += slope.second;
        }
        treeMult *= treeCount;
    }
    std::cout << "part 2: " << treeMult << "\n";
}

auto loadInput() {
    std::vector<std::string> map;
    std::ifstream f{ "input.txt" };
    char line[33];
    while (f.getline(line, std::size(line))) {
        map.push_back(line);
    }
    return map;
}

int main() {
    auto map = loadInput();
    part1(map);
    part2(map);
    return 0;
}
