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
    auto makeYearRangeValidator = [](const std::string key, int min, int max) -> auto {
        return [=](const Passport& passport) -> bool {
            std::regex rgx{ "^(\\d{4})$" };
            std::smatch match;
            if (std::regex_match(passport.at(key), match, rgx)) {
                auto yr = std::stoi(match.str());
                return (yr >= min) && (yr <= max);
            }
            return false;
        };
    };
    auto makeHeightValidator = [](const std::string key, int minCM, int maxCM, int minIN, int maxIN) -> auto {
        return [=](const Passport& passport) -> bool {
            std::regex rgx{ "^(\\d{3})cm|(\\d{2})in$" };
            std::smatch match;
            if (std::regex_match(passport.at(key), match, rgx)) {
                auto hgtCMStr = match[1].str();
                auto hgtINStr = match[2].str();
                if (!hgtCMStr.empty()) {
                    auto hgt = std::stoi(hgtCMStr);
                    return (hgt >= minCM) && (hgt <= maxCM);
                }
                else if (!hgtINStr.empty()) {
                    auto hgt = std::stoi(hgtINStr);
                    return (hgt >= minIN) && (hgt <= maxIN);
                }
            }
            return false;
        };
    };
    auto makeRegexValidator = [](const std::string key, const std::string pattern) -> auto {
        return [=](const Passport& passport) -> bool {
            std::regex rgx{ pattern };
            std::smatch match;
            return std::regex_match(passport.at(key), match, rgx);
        };
    };

    auto byrValidator = makeYearRangeValidator("byr", 1920, 2002);
    auto iyrValidator = makeYearRangeValidator("iyr", 2010, 2020);
    auto eyrValidator = makeYearRangeValidator("eyr", 2020, 2030);
    auto hgtValidator = makeHeightValidator("hgt", 150, 193, 59, 76);
    auto hclValidator = makeRegexValidator("hcl", "^#[0-9a-f]{6}$");
    auto eclValidator = makeRegexValidator("ecl", "^amb|blu|brn|gry|grn|hzl|oth$");
    auto pidValidator = makeRegexValidator("pid", "^\\d{9}$");

    std::cout << "part 2: " << std::count_if(passports.begin(), passports.end(), [&](const Passport& passport) -> bool {
        return isValid(passport)
            && byrValidator(passport)
            && iyrValidator(passport)
            && eyrValidator(passport)
            && hgtValidator(passport)
            && hclValidator(passport)
            && eclValidator(passport)
            && pidValidator(passport);
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
