#include <cstdint>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using u64 = uint64_t;

template <typename Func>
void process(const std::vector<u64>& nums, Func&& func) {
    // process preamble
    std::unordered_map<u64, u64> counts;
    for (size_t i = 0; i < 24; i++) {
        for (size_t j = i + 1; j < 25; j++) {
            if (nums[i] != nums[j]) {
                counts[nums[i] + nums[j]]++;
            }
        }
    }

    // process remainder
    for (size_t i = 25; i < nums.size(); i++) {
        if (counts[nums[i]] == 0) {
            func(nums[i]);
            break;
        }
        for (size_t j = i - 24; j < i; j++) {
            if (nums[i] != nums[j]) {
                counts[nums[i] + nums[j]]++;
            }
            counts[nums[i - 25] + nums[j]]--;
        }
    }
}

void findContiguous(const std::vector<u64>& num, u64 target) {
    auto start = num.begin();
    auto end = num.begin();
    auto sum = *start;
    while (end != num.end()) {
        if (sum == target) {
            u64 min = ~0ull;
            u64 max = 0;
            for (auto it = start; it != end; it++) {
                min = std::min(min, *it);
                max = std::max(max, *it);
            }
            std::cout << "part 2: " << (min + max) << "\n";
            break;
        }
        if (sum > target) {
            sum -= *start;
            start++;
        }
        else { // sum < target
            end++;
            sum += *end;
        }
    }
}

void part1(const std::vector<u64>& nums) {
    process(nums, [](u64 num) {
        std::cout << "part 1: " << num << "\n";
    });
}

void part2(const std::vector<u64>& nums) {
    process(nums, [&](u64 num) {
        findContiguous(nums, num);
    });
}

std::vector<u64> loadInput() {
    std::vector<u64> nums;
    std::ifstream f{ "input.txt" };
    int num;
    while (f >> num) {
        nums.push_back(num);
    }
    return nums;
}

int main() {
    auto nums = loadInput();
    part1(nums);
    part2(nums);
    return 0;
}
