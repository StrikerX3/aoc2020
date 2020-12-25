#include <iostream>
#include <fstream>
#include <cstdint>

using u32 = uint32_t;
using u64 = uint64_t;

u32 iterate(u64 value, u32 subject) {
    return (value * subject) % 20201227;
}

struct Keys {
    u32 pkCard;
    u32 pkDoor;

    u32 cardLoops = 0;
    u32 doorLoops = 0;

    Keys(u32 card, u32 door)
        : pkCard(card)
        , pkDoor(door) {

        // Figure out the number of loops of each device based on their primary key
        // and the cryptographic handshake process
        u32 value = 1;
        u32 loops = 1;
        while (cardLoops == 0 || doorLoops == 0) {
            value = iterate(value, 7);
            if (cardLoops == 0 && value == pkCard) cardLoops = loops;
            if (doorLoops == 0 && value == pkDoor) doorLoops = loops;
            loops++;
        }
    }
};

void part1(const Keys& keys) {
    u32 value = 1;
    for (u32 loop = 0; loop < keys.cardLoops; loop++) {
        value = iterate(value, keys.pkDoor);
    }
    std::cout << "part 1: " << value << '\n';
}

auto loadInput() {
    return Keys{ 12320657, 9659666 };
    //return Keys{ 5764801, 17807724 };
}

int main() {
    auto keys = loadInput();
    part1(keys);
    // There is no part 2
    return 0;
}
