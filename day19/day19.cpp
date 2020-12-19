#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <deque>

using u32 = uint32_t;
using u64 = uint64_t;

// Match a character: "a", "b"
using SimpleMatch = char;

// Match other rules: 2 3
using RuleList = std::vector<u32>;

// Match one of the subrules: <rule> | <rule>
using Disjunction = std::pair<RuleList, RuleList>;

using Rule = std::variant<SimpleMatch, RuleList, Disjunction>;

struct RuleSet {
    std::vector<Rule> rules;

    bool evaluate(const SimpleMatch& match, const std::string_view& message, std::deque<size_t>& ruleIndices) const {
        if (message.empty()) {
            return false;
        }
        if (message[0] == match) {
            return matchesAll(message.substr(1), ruleIndices);
        }
        return false;
    }

    bool evaluate(const RuleList& ruleList, const std::string_view& message, std::deque<size_t>& ruleIndices) const {
        ruleIndices.insert(ruleIndices.begin(), ruleList.cbegin(), ruleList.cend());
        return matchesAll(message, ruleIndices);
    }

    bool evaluate(const Disjunction& disjunction, const std::string_view& message, std::deque<size_t>& ruleIndices) const {
        auto ruleIndices1 = ruleIndices;
        auto ruleIndices2 = ruleIndices;
        return evaluate(disjunction.first, message, ruleIndices1)
            || evaluate(disjunction.second, message, ruleIndices2);
    }

    bool matchesAll(const std::string_view& message, std::deque<size_t>& ruleIndices) const {
        if (message.empty() && ruleIndices.empty()) return true;
        if (message.empty() || ruleIndices.empty()) return false;

        auto ruleIndex = ruleIndices.front(); ruleIndices.pop_front();
        auto& rule = rules[ruleIndex];
        if (std::holds_alternative<SimpleMatch>(rule)) {
            return evaluate(std::get<SimpleMatch>(rule), message, ruleIndices);
        }
        else if (std::holds_alternative<RuleList>(rule)) {
            return evaluate(std::get<RuleList>(rule), message, ruleIndices);
        }
        else if (std::holds_alternative<Disjunction>(rule)) {
            return evaluate(std::get<Disjunction>(rule), message, ruleIndices);
        }
        else {
            std::abort();
        }
    }

    bool matches(const std::string& message) const {
        std::deque<size_t> ruleIndices;
        ruleIndices.push_back(0);
        return matchesAll(message, ruleIndices);
    }
};

struct Input {
    RuleSet ruleSet;
    std::vector<std::string> messages;

    u32 countValid() const {
        u32 validCount = 0;
        for (auto& msg : messages) {
            if (ruleSet.matches(msg)) validCount++;
        }
        return validCount;
    }
};

void part1(const Input& input) {
    std::cout << "part 1: " << input.countValid() << '\n';
}

void part2(const Input& input) {
    Input updatedRules = input;
    updatedRules.ruleSet.rules[8] = Disjunction{ {42}, {42, 8} };
    updatedRules.ruleSet.rules[11] = Disjunction{ {42, 31}, {42, 11, 31} };
    std::cout << "part 2: " << updatedRules.countValid() << '\n';
}

Input loadInput() {
    Input input;
    std::fstream f{ "input.txt" };

    // First part contains a ruleset
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) break;

        auto colonPos = line.find(':');
        u32 ruleNumber = std::stoul(line.substr(0, colonPos));
        auto ruleStr = line.substr(colonPos + 2);

        if (input.ruleSet.rules.size() <= ruleNumber) {
            input.ruleSet.rules.resize(ruleNumber + 1);
        }

        // Parse rule
        // - If it contains a double-quote, it's a SimpleMatch
        // - If it contains a pipe, it's a Disjunction
        // - Otherwise, it's a RuleList
        if (ruleStr.find('"') != ruleStr.npos) {
            // "<char>"
            input.ruleSet.rules[ruleNumber] = SimpleMatch{ ruleStr.substr(ruleStr.find('"') + 1, 1)[0] };
        }
        else if (ruleStr.find('|') != ruleStr.npos) {
            // <ruleNum> <ruleNum> | <ruleNum> <ruleNum>
            std::istringstream ss{ ruleStr };
            Disjunction rule;
            std::string value;
            bool first = true;
            while (ss >> value) {
                if (value == "|") {
                    first = false;
                }
                else if (first) {
                    rule.first.push_back(std::stoi(value));
                }
                else {
                    rule.second.push_back(std::stoi(value));
                }
            }
            input.ruleSet.rules[ruleNumber] = rule;
        }
        else {
            // <ruleNum> [<ruleNum> [...]]
            std::istringstream ss{ ruleStr };
            RuleList rule;
            std::string value;
            while (ss >> value) {
                rule.push_back(std::stoi(value));
            }
            input.ruleSet.rules[ruleNumber] = rule;
        }
    }

    // Second part contains the messages
    while (std::getline(f, line)) {
        input.messages.push_back(line);
    }

    return input;
}

int main() {
    auto input = loadInput();
    part1(input);
    part2(input);
    return 0;
}
