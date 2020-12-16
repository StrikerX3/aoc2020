#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

using u32 = uint32_t;
using u64 = uint64_t;

struct Range {
    u32 min, max;

    bool matches(u32 value) const {
        return value >= min && value <= max;
    }
};

using Ticket = std::vector<u32>;

struct DataSet {
    std::unordered_map<std::string, std::pair<Range, Range>> rules;

    Ticket myTicket;
    std::vector<Ticket> nearbyTickets;
};

void part1(const DataSet& dataSet) {
    u32 ticketScanningErrorRate = 0;
    for (auto& ticket : dataSet.nearbyTickets) {
        for (auto num : ticket) {
            bool anyRuleValid = false;
            for (auto& [_, rule] : dataSet.rules) {
                if (rule.first.matches(num) || rule.second.matches(num)) {
                    anyRuleValid = true;
                    break;
                }
            }
            if (!anyRuleValid) {
                ticketScanningErrorRate += num;
            }
        }
    }

    std::cout << "part 1: " << ticketScanningErrorRate << '\n';
}

void part2(const DataSet& dataSet) {
    // Eliminate bad tickets
    std::vector<Ticket> validTickets;
    validTickets.push_back(dataSet.myTicket);
    for (auto& ticket : dataSet.nearbyTickets) {
        bool allFieldsValid = true;
        for (auto num : ticket) {
            bool anyRuleValid = false;
            for (auto& [_, rule] : dataSet.rules) {
                if (rule.first.matches(num) || rule.second.matches(num)) {
                    anyRuleValid = true;
                    break;
                }
            }
            if (!anyRuleValid) {
                allFieldsValid = false;
                break;
            }
        }
        if (allFieldsValid) {
            validTickets.push_back(ticket);
        }
    }

    // rule name -> sieve over the fields, where true means the rule is valid for that particular field
    std::unordered_map<std::string, std::vector<bool>> ruleSieve;
    for (auto& [name, _] : dataSet.rules) {
        ruleSieve[name].resize(dataSet.myTicket.size());
        std::fill(ruleSieve[name].begin(), ruleSieve[name].end(), true);
    }
    for (auto& [name, rule] : dataSet.rules) {
        for (auto& ticket : validTickets) {
            for (size_t i = 0; i < ticket.size(); i++) {
                auto num = ticket[i];
                ruleSieve[name][i] = ruleSieve[name][i] && (rule.first.matches(num) || rule.second.matches(num));
            }
        }
    }

    // The sieves will have different numbers of "true" values, from 1 to N.
    // Sort them by count here (index 0 = 1, index N-1 = N)
    std::vector<std::string> rulesBySieveCount;
    rulesBySieveCount.resize(dataSet.myTicket.size());
    for (auto& [name, sieve] : ruleSieve) {
        auto count = std::count_if(sieve.begin(), sieve.end(), [](bool entry) { return entry; });
        rulesBySieveCount[count - 1] = name;
    }

    // Now assign rules to fields
    std::unordered_map<std::string, size_t> rulesToFields;
    for (size_t count = 1; count <= dataSet.myTicket.size(); count++) {
        auto& name = rulesBySieveCount[count - 1];
        auto& sieve = ruleSieve[name];
        
        // Find the position of the only true value in the sieve
        size_t pos = 0;
        while (!sieve[pos]) {
            pos++;
        }
        
        // Assign rule to that field
        rulesToFields[name] = pos;
        
        // Clear the flag from every sieve
        for (auto& [_, sieve] : ruleSieve) {
            sieve[pos] = false;
        }
    }

    // Finally get the answer to the problem
    u64 product = 1;
    for (auto& [name, pos] : rulesToFields) {
        if (name.find("departure") != name.npos) {
            product *= dataSet.myTicket[pos];
        }
    }

    std::cout << "part 2: " << product << '\n';
}

DataSet loadInput() {
    DataSet dataSet;
    std::fstream f{ "input.txt" };
    std::regex rgxRule{ "(.+): (\\d+)-(\\d+) or (\\d+)-(\\d+)" };

    std::string line;
    // Each line has a rule in the format:
    // <name>: <range> or <range>
    // The list ends with a blank line.
    while (std::getline(f, line)) {
        if (line.empty()) break;

        std::smatch match;
        if (std::regex_match(line, match, rgxRule)) {
            auto name = match[1].str();
            Range range1{ std::stoul(match[2].str()), std::stoul(match[3].str()) };
            Range range2{ std::stoul(match[4].str()), std::stoul(match[5].str()) };
            dataSet.rules.emplace(std::make_pair(name, std::make_pair(range1, range2)));
        }
    }

    auto parseTicket = [](const std::string& line) -> Ticket {
        Ticket ticket;
        std::istringstream ss{ line };
        std::string number;
        while (std::getline(ss, number, ',')) {
            ticket.push_back(std::stoul(number));
        }
        return ticket;
    };

    // Next line should be "your ticket:", followed by a list of numbers that represent a ticket,
    // followed by a blank line
    std::getline(f, line);
    if (line != "your ticket:") {
        std::abort();
    }
    std::getline(f, line);
    dataSet.myTicket = parseTicket(line);
    std::getline(f, line);

    // Next line should be "nearby tickets:", followed by multiple lines representing nearby tickets.
    // The list ends with a blank line.
    std::getline(f, line);
    if (line != "nearby tickets:") {
        std::abort();
    }
    while (std::getline(f, line)) {
        if (line.empty()) break;
        dataSet.nearbyTickets.push_back(parseTicket(line));
    }

    return dataSet;
}

int main() {
    auto dataSet = loadInput();
    part1(dataSet);
    part2(dataSet);
    return 0;
}
