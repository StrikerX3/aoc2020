#include <bitset>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using u32 = uint32_t;

u32 toID(std::string seat) {
    u32 id = 0;
    for (auto ch : seat) {
        id = (id << 1) | (ch == 'B' || ch == 'R');
    }
    return id;
}

void part1(const std::vector<std::string>& seats) {
    u32 maxID = 0;
    for (auto &seat : seats) {
        maxID = std::max(maxID, toID(seat));
    }
    std::cout << "part 1: " << maxID << "\n";
}

void part2(const std::vector<std::string>& seats) {
    u32 minID = 1024;
    std::bitset<1024> takenSeats;
    for (auto& seat : seats) {
        u32 id = toID(seat);
        takenSeats.set(id);
        minID = std::min(minID, id);
    }
    for (u32 id = minID; id < 1024; id++) {
        if (!takenSeats.test(id)) {
            std::cout << "part 2: " << id << "\n";
            break;
        }
    }
}

auto loadInput() {
    std::vector<std::string> seats;
    std::ifstream f{ "input.txt" };
    std::string seat;
    while (f >> seat) {
        seats.push_back(seat);
    }
    return seats;
}

int main() {
    auto seats = loadInput();
    part1(seats);
    part2(seats);
    return 0;
}
