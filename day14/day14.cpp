#include <cstdint>
#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

using u32 = uint32_t;
using u64 = uint64_t;

struct Mask {
    std::bitset<36> ones;     // '1'
    std::bitset<36> zeros;    // '0'
    std::bitset<36> floating; // 'X'
};

struct MemoryWrite {
    u64 address;
    u64 value;
};

using Operation = std::variant<Mask, MemoryWrite>;

void part1(const std::vector<Operation>& operations) {
    std::unordered_map<u64, u64> memory;
    Mask mask;
    for (auto& op : operations) {
        if (std::holds_alternative<Mask>(op)) {
            mask = std::get<Mask>(op);
        }
        else {
            auto write = std::get<MemoryWrite>(op);
            write.value |= mask.ones.to_ullong();
            write.value &= ~mask.zeros.to_ullong();
            memory[write.address] = write.value;
        }
    }
    u64 sum = 0;
    for (auto& [addr, value] : memory) {
        sum += value;
    }
    std::cout << "part 1: " << sum << " (" << memory.size() << " memory addresses used)\n";
}

u64 expand(u64 bits, u64 mask) {
    // Adapted from Hacker's Delight (2nd edition)
    // 7-5 Expand, or Generalized Insert
    // Figure 7-12. Parallel suffix method for the expand operation.
    // pg. 157

    u64 m0 = mask;          // Save original mask.
    u64 mk = (~mask << 1);  // We will count 0's to right.
    u64 array[6];

    for (u64 i = 0; i < 6; i++) {
        u64 mp = mk ^ (mk << 1ull);  // Parallel suffix.
        mp ^= (mp << 2ull);
        mp ^= (mp << 4ull);
        mp ^= (mp << 8ull);
        mp ^= (mp << 16ull);
        mp ^= (mp << 32ull);
        u64 mv = mp & mask;  // Bits to move.
        array[i] = mv;
        mask = (mask ^ mv) | (mv >> (1 << i));  // Compress m.
        mk &= ~mp;
    }

    for (u64 i = 5; i <= 5; i--) {
        u64 mv = array[i];
        u64 t = bits << (1 << i);
        bits = (bits & ~mv) | (t & mv);
    }
    return bits & m0;  // Clear out extraneous bits.
}

void part2(const std::vector<Operation>& operations) {
    std::unordered_map<u64, u64> memory;
    Mask mask;
    for (auto& op : operations) {
        if (std::holds_alternative<Mask>(op)) {
            mask = std::get<Mask>(op);
        }
        else {
            auto write = std::get<MemoryWrite>(op);
            write.address |= mask.ones.to_ullong();
            u64 floatingCount = (1ull << mask.floating.count());
            for (u64 floatingBits = 0; floatingBits < floatingCount; floatingBits++) {
                write.address &= ~mask.floating.to_ullong();
                write.address |= expand(floatingBits, mask.floating.to_ullong());
                memory[write.address] = write.value;
            }
        }
    }
    u64 sum = 0;
    for (auto& [addr, value] : memory) {
        sum += value;
    }
    std::cout << "part 2: " << sum << " (" << memory.size() << " memory addresses used)\n";
}

std::vector<Operation> loadInput() {
    std::vector<Operation> operations;
    std::ifstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        Operation operation;
        if (line.starts_with("mask = ")) {
            Mask mask;
            for (size_t i = 0; i < 36; i++) {
                char bit = line.at(i + 7);
                switch (bit) {
                case '0': mask.zeros.set(35 - i); break;
                case '1': mask.ones.set(35 - i); break;
                case 'X': mask.floating.set(35 - i); break;
                }
            }
            operation = mask;
        }
        else {
            MemoryWrite write;
            write.address = std::stoul(line.substr(4, line.find(']') - 4));
            write.value = std::stoul(line.substr(line.find('=') + 2));
            operation = write;
        }
        operations.push_back(operation);
    }
    return operations;
}

int main() {
    auto data = loadInput();
    part1(data);
    part2(data);
    return 0;
}
