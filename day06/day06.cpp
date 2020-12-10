#include <bitset>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

using u32 = uint32_t;

struct Answers {
    std::bitset<26> any{ 0 };
    std::bitset<26> all{ ~0ull };
    
    void Reset() {
        any = 0;
        all = ~0ull;
    }
};

void part1(const std::vector<Answers>& answers) {
    u32 total = 0;
    for (auto &answer : answers) {
        total += answer.any.count();
    }
    std::cout << "part 1: " << total << "\n";
}

void part2(const std::vector<Answers>& answers) {
    u32 total = 0;
    for (auto& answer : answers) {
        total += answer.all.count();
    }
    std::cout << "part 2: " << total << "\n";
}

auto loadInput() {
    std::vector<Answers> answers;
    std::ifstream f{ "input.txt" };
    std::string line;
    Answers groupAnswers;
    while (std::getline(f, line)) {
        if (line.empty()) {
            answers.push_back(groupAnswers);
            groupAnswers.Reset();
            continue;
        }
        std::bitset<26> personAnswers;
        for (char c : line) {
            personAnswers.set(c - 'a');
        }
        groupAnswers.any |= personAnswers;
        groupAnswers.all &= personAnswers;
    }
    if (groupAnswers.any.any()) {
        answers.push_back(groupAnswers);
    }
    return answers;
}

int main() {
    auto answers = loadInput();
    part1(answers);
    part2(answers);
    return 0;
}
