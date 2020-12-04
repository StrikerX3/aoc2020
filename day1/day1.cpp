#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>

// Part 1 - Two sum
void part1(const std::vector<int>& nums) {
    std::unordered_set<int> complements;
    for (int num : nums) {
        int complement = 2020 - num;
        if (complements.contains(num)) {
            int result = num * complement;
            std::cout << "part 1: " << result << " (" << num << ", " << complement << ")\n";
            break;
        }
        complements.insert(complement);
    }
}

// Part 2 - Three sum
void part2(const std::vector<int>& nums) {
    for (size_t i = 0; i < nums.size(); i++) {
        int num1 = nums[i];

        int remainingSum = 2020 - num1;

        std::unordered_set<int> complements;
        for (size_t j = i + 1; j < nums.size(); j++) {
            int num2 = nums[j];
            int complement = remainingSum - num2;
            if (complements.contains(num2)) {
                int result = num1 * num2 * complement;
                std::cout << "part 2: " << result << " (" << num1 << ", " << num2 << ", " << complement << ")\n";
                break;
            }
            complements.insert(complement);
        }
    }
}

std::vector<int> loadInput() {
    std::vector<int> nums;
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
