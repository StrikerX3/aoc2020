#include <cstdint>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using u32 = uint32_t;

struct Rule {
    u32 count;
    std::string color;
};

struct Rules {
    // What bags can a bag of a given color contain
    std::unordered_multimap<std::string, Rule> forward;

    // What bags can contain a bag of a given color
    std::unordered_multimap<std::string, std::string> backward;
};

void part1(const Rules& rules) {
    std::deque<std::string> bagsToCheck;
    std::unordered_set<std::string> bagsThatContainIt;
    auto range = rules.backward.equal_range("shiny gold");
    for (auto it = range.first; it != range.second; it++) {
        bagsToCheck.push_back(it->second);
    }
    while (!bagsToCheck.empty()) {
        auto bag = bagsToCheck.front();
        bagsToCheck.pop_front();
        bagsThatContainIt.insert(bag);
        auto range = rules.backward.equal_range(bag);
        for (auto it = range.first; it != range.second; it++) {
            bagsToCheck.push_back(it->second);
        }
    }
    std::cout << "part 1: " << bagsThatContainIt.size() << "\n";
}

size_t countContainedBags(const Rules& rules, std::string bag) {
    size_t total = 1;
    auto range = rules.forward.equal_range(bag);
    for (auto it = range.first; it != range.second; it++) {
        total += it->second.count * countContainedBags(rules, it->second.color);
    }
    return total;
}

void part2(const Rules& rules) {
    std::cout << "part 2: " << (countContainedBags(rules, "shiny gold") - 1) << "\n";
}

auto loadInput() {
    std::ifstream f{ "input.txt" };
    std::string line;
    std::regex rgxEntry{ "(\\d+) (.*?) bags?" };
    std::smatch match;
    Rule rule;
    Rules rules;
    while (std::getline(f, line)) {
        auto containPos = line.find(" bags contain ");
        auto container = line.substr(0, containPos);
        auto contained = line.substr(containPos + 14);
        auto pos = contained.cbegin();
        while (std::regex_search(pos, contained.cend(), match, rgxEntry)) {
            rule = { std::stoul(match[1]), match[2] };
            rules.forward.emplace(std::make_pair(container, rule));
            rules.backward.emplace(std::make_pair(rule.color, container));
            pos = match.suffix().first;
        }
    }
    return rules;
}

int main() {
    auto rules = loadInput();
    part1(rules);
    part2(rules);
    return 0;
}
