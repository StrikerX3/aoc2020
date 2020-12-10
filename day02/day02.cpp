#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>

struct Password {
    size_t num1;
    size_t num2;
    char ch;
    std::string password;


    bool valid1() const {
        size_t count = std::ranges::count(password, ch);
        return (count >= num1) && (count <= num2);
    }

    bool valid2() const {
        bool pos1 = (password[num1 - 1] == ch);
        bool pos2 = (password[num2 - 1] == ch);
        return pos1 != pos2;
    }

    friend std::istream& operator>>(std::istream& is, Password& password) {
        // line format:
        // <num1>-<num2> <ch>: <password>
        char discard;
        return is >> password.num1 >> discard >> password.num2 >> password.ch >> discard >> password.password;
    }
};

// Part 1 - Count valid passwords where num1 and num2 specify the minimum and maximum number of times (respectively)
// ch must appear in the password
void part1(const std::vector<Password>& passwords) {
    size_t validCount = 0;
    for (auto& password : passwords) {
        if (password.valid1()) validCount++;
    }
    std::cout << "part 1: " << validCount << "\n";
}

// Part 2 - Count valid passwords where ch must appear exactly once in positions num1 and num2 (1-based)
void part2(const std::vector<Password>& passwords) {
    size_t validCount = 0;
    for (auto& password : passwords) {
        if (password.valid2()) validCount++;
    }
    std::cout << "part 2: " << validCount << "\n";
}

auto loadInput() {
    std::vector<Password> passwords;
    std::ifstream f{ "input.txt" };
    Password password;
    while (f >> password) {
        passwords.push_back(password);
    }
    return passwords;
}

int main() {
    auto passwords = loadInput();
    part1(passwords);
    part2(passwords);
    return 0;
}
