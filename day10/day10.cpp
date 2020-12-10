#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using u32 = uint32_t;
using u64 = uint64_t;

void part1(const std::vector<u32>& adapters) {
    u32 diff1 = 0;
    u32 diff3 = 1;  // device adapter is always +3 higher than the highest adapter
    for (size_t i = 1; i < adapters.size(); i++) {
        auto diff = adapters[i] - adapters[i - 1];
        if (diff == 1) diff1++;
        else if (diff == 3) diff3++;
    }
    std::cout << "part 1: " << (diff1 * diff3) << " (" << diff1 << ", " << diff3 << ")\n";
}

u64 countCombinations(const std::vector<u32>& adapters, size_t index) {
    static std::unordered_map<size_t, u64> memory;
    if (memory.contains(index)) {
        return memory.at(index);
    }
    u64 count = 0;
    u32 currAdapter = adapters[index];
    for (size_t i = index + 1; i < adapters.size(); i++) {
        if (adapters[i] - currAdapter <= 3) {
            count += countCombinations(adapters, i);
            if (i == adapters.size() - 1) {
                count++;
            }
        }
    }
    memory[index] = count;
    return count;
}

void part2(const std::vector<u32>& adapters) {
    std::cout << "part 2: " << countCombinations(adapters, 0) << "\n";
}

std::vector<u32> loadInput() {
    std::vector<u32> adapters;
    std::ifstream f{ "input.txt" };
    int adapter;
    adapters.push_back(0); // force the seat adapter into the list
    while (f >> adapter) {
        adapters.push_back(adapter);
    }
    std::sort(adapters.begin(), adapters.end()); // sort to make it easier to work with
    return adapters;
}

int main() {
    auto adapters = loadInput();
    part1(adapters);
    part2(adapters);
    return 0;
}
