#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

using u32 = uint32_t;
using u64 = uint64_t;

struct Data {
    u32 earliestDeparture;
    std::vector<u32> busIDs;
};

void part1(const Data& data) {
    u32 min = std::numeric_limits<u32>::max();
    u32 busID;
    for (auto id : data.busIDs) {
        if (id == 0) continue;
        u32 nextDeparture = (data.earliestDeparture + id - 1) / id * id;
        if (nextDeparture < min) {
            min = nextDeparture;
            busID = id;
        }
    }
    std::cout << "part 1: " << (busID * (min - data.earliestDeparture))
        << " (" << busID << ", " << min << " - " << data.earliestDeparture << " = " << (min - data.earliestDeparture) << ")\n";
}

void part2(const Data& data) {
    u64 firstDeparture = 0;
    u64 lcm = data.busIDs[0];
    for (size_t i = 1; i < data.busIDs.size(); i++) {
        auto id = data.busIDs[i];
        if (id == 0) continue;
        while ((firstDeparture + i) % id > 0) {
            firstDeparture += lcm;
        }
        lcm = std::lcm(lcm, id);
    }
    std::cout << "part 2: " << firstDeparture << '\n';
}

Data loadInput() {
    Data data;
    std::ifstream f{ "input.txt" };
    std::string busID;
    f >> data.earliestDeparture;
    while (std::getline(f, busID, ',')) {
        if (!busID.empty()) {
            if (busID == "x") {
                data.busIDs.push_back(0);
            }
            else {
                data.busIDs.push_back(std::stoi(busID));
            }
        }
    }
    return data;
}

int main() {
    auto data = loadInput();
    part1(data);
    part2(data);
    return 0;
}
