#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <regex>

using u32 = uint32_t;
using Passport = std::unordered_map<std::string, std::string>;

bool isValid(const Passport& passport) {
    return passport.count("byr") > 0
        && passport.count("iyr") > 0
        && passport.count("eyr") > 0
        && passport.count("hgt") > 0
        && passport.count("hcl") > 0
        && passport.count("ecl") > 0
        && passport.count("pid") > 0;
}

void part1(const std::vector<Passport>& passports) {
    std::cout << "part 1: " << std::count_if(passports.begin(), passports.end(), isValid) << "\n";
}

void part2(const std::vector<Passport>& passports) {
    std::regex rgx_yr{ "^(\\d{4})$" };
    std::regex rgx_hgt{ "^(\\d{3})cm|(\\d{2})in$" };
    std::regex rgx_hcl{ "^#[0-9a-f]{6}$" };
    std::regex rgx_ecl{ "^amb|blu|brn|gry|grn|hzl|oth$" };
    std::regex rgx_pid{ "^\\d{9}$" };

    std::cout << "part 2: " << std::count_if(passports.begin(), passports.end(), [&](const Passport& passport) -> bool {
        if (!isValid(passport)) {
            return false;
        }

        std::smatch match;

        if (std::regex_match(passport.at("byr"), match, rgx_yr)) {
            auto yr = std::stoi(match.str());
            if (yr < 1920 || yr > 2002) return false;
        }
        else {
            return false;
        }

        if (std::regex_match(passport.at("iyr"), match, rgx_yr)) {
            auto yr = std::stoi(match.str());
            if (yr < 2010 || yr > 2020) return false;
        }
        else {
            return false;
        }

        if (std::regex_match(passport.at("eyr"), match, rgx_yr)) {
            auto yr = std::stoi(match.str());
            if (yr < 2020 || yr > 2030) return false;
        }
        else {
            return false;
        }

        if (std::regex_match(passport.at("hgt"), match, rgx_hgt)) {
            auto hgtCMStr = match[1].str();
            auto hgtINStr = match[2].str();
            if (!hgtCMStr.empty()) {
                auto hgt = std::stoi(hgtCMStr);
                if (hgt < 150 || hgt > 193) return false;
            }
            else if (!hgtINStr.empty()) {
                auto hgt = std::stoi(hgtINStr);
                if (hgt < 59 || hgt > 76) return false;
            }
        }
        else {
            return false;
        }

        if (!std::regex_match(passport.at("hcl"), match, rgx_hcl)) {
            return false;
        }

        if (!std::regex_match(passport.at("ecl"), match, rgx_ecl)) {
            return false;
        }

        if (!std::regex_match(passport.at("pid"), match, rgx_pid)) {
            return false;
        }

        return true;
    }) << "\n";
}

auto loadInput() {
    std::vector<Passport> passports;
    std::ifstream f{ "input.txt" };
    char line[81];
    std::unordered_map<std::string, std::string> map;
    while (f.getline(line, std::size(line))) {
        if (strlen(line) == 0) {
            passports.push_back(map);
            map.clear();
        }
        else {
            std::istringstream iss{ line };
            std::string entry;
            while (iss >> entry) {
                auto colonPos = entry.find(':');
                auto key = entry.substr(0, colonPos);
                auto value = entry.substr(colonPos + 1);
                map[key] = value;
            }
        }
    }
    if (!map.empty()) {
        passports.push_back(map);
    }

    return passports;
}

int main() {
    auto passports = loadInput();
    part1(passports);
    part2(passports);
    return 0;
}
