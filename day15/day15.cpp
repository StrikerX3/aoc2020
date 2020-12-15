#include <iostream>
#include <vector>
#include <unordered_map>

int calcTurns(const std::vector<int>& nums, int numTurns) {
    struct Memory {
        int lastTurn = 0;
        int secondToLastTurn = 0;
    };
    std::unordered_map<int, Memory> memory;
    int turn = 1;
    int num = *nums.rbegin();
    for (auto startingNum : nums) {
        memory[startingNum].lastTurn = turn;
        turn++;
    }
    while (turn <= numTurns) {
        if (memory[num].secondToLastTurn == 0) {
            num = 0;
        }
        else {
            num = memory[num].lastTurn - memory[num].secondToLastTurn;
        }
        memory[num].secondToLastTurn = memory[num].lastTurn;
        memory[num].lastTurn = turn;
        turn++;
    }
    return num;
}

void part1(const std::vector<int>& nums) {
    std::cout << "part 1: " << calcTurns(nums, 2020) << '\n';
}

void part2(const std::vector<int>& nums) {
    std::cout << "part 2: " << calcTurns(nums, 30000000) << '\n';
}

int main() {
    std::vector<int> nums = { 2, 1, 10, 11, 0, 6 };
    part1(nums);
    part2(nums);
    return 0;
}
