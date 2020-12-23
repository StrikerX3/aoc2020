#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <array>

using u32 = uint32_t;
using u64 = uint64_t;

//       123456789
// digit(abcdefghi, 1) = a
// digit(abcdefghi, 5) = e
// digit(abcdefghi, 9) = i
u32 digit(u32 cups, u32 pos) {
    assert(pos >= 1 && pos <= 9);
    constexpr u64 divisors[] = {
        100000000ull, // 1
         10000000ull, // 2
          1000000ull, // 3
           100000ull, // 4
            10000ull, // 5
             1000ull, // 6
              100ull, // 7
               10ull, // 8
                1ull, // 9
    };
    return cups / divisors[pos - 1] % 10u;
}

// digitPos(123456789, 1) = 1
// digitPos(123456789, 5) = 5
// digitPos(123456789, 9) = 9
// digitPos(987654321, 1) = 9
// digitPos(987654321, 7) = 3
// digitPos(987654321, 9) = 1
u32 digitPos(u32 num, u32 digit) {
    u32 pos = 9;
    while (num != 0 && num % 10 != digit) {
        pos--;
        num /= 10;
    }
    return pos;
}


bool hasDigit(u32 num, u32 digit) {
    while (num != 0) {
        if (num % 10 == digit) return true;
        num /= 10;
    }
    return false;
}


//         123456789
// extract(abcdefghi, 1) = abc
// extract(abcdefghi, 2) = bcd
// extract(abcdefghi, 6) = fgh
// extract(abcdefghi, 7) = ghi
// extract(abcdefghi, 8) = hia
// extract(abcdefghi, 9) = iab
u32 extract(u64 cups, u32 pos) {
    assert(pos >= 1 && pos <= 9);
    cups *= 1000000001ull;   // abcdefghiabcdefghi
    constexpr u64 divisors[] = {
          1000000ull, // 1
           100000ull, // 2
            10000ull, // 3
             1000ull, // 4
              100ull, // 5
               10ull, // 6
                1ull, // 7
        100000000ull, // 8
         10000000ull, // 9
    };
    return (cups / divisors[pos - 1]) % 1000u;
}

//        123456789
// remove(abcdefghi, 1) = defghi
// remove(abcdefghi, 2) = aefghi
// remove(abcdefghi, 6) = abcdei
// remove(abcdefghi, 7) = abcdef
// remove(abcdefghi, 8) = bcdefg
// remove(abcdefghi, 9) = cdefgh
u32 remove(u32 cups, u32 pos) {
    switch (pos) {
    case 1: return cups % 1000000u;
    case 2: return cups % 100000u + (cups / 100000000u % 10u * 100000u);
    case 3: return cups % 10000u + (cups / 10000000u % 100u * 10000u);
    case 4: return cups % 1000u + (cups / 1000000u % 1000u * 1000u);
    case 5: return cups % 100u + (cups / 100000u % 10000u * 100u);
    case 6: return cups % 10u + (cups / 10000u % 100000u * 10u);
    case 7: return cups / 1000u;
    case 8: return cups / 100u % 1000000u;
    case 9: return cups / 10u % 1000000u;
    default: std::abort();
    }
}


//        123456
// insert(abcdef, ghi, 0) = ghiabcdef
// insert(abcdef, ghi, 1) = aghibcdef
// insert(abcdef, ghi, 2) = abghicdef
// insert(abcdef, ghi, 3) = abcghidef
// insert(abcdef, ghi, 4) = abcdghief
// insert(abcdef, ghi, 5) = abcdeghif
// insert(abcdef, ghi, 6) = abcdefghi
u32 insert(u32 cups, u32 add, u32 pos) {
    switch (pos) {
    case 0: return cups + (add * 1000000u);
    case 1: return cups % 100000u + (add * 100000u) + (cups - cups % 100000u) * 1000u;
    case 2: return cups % 10000u + (add * 10000u) + (cups - cups % 10000u) * 1000u;
    case 3: return cups % 1000u + (add * 1000u) + (cups - cups % 1000u) * 1000u;
    case 4: return cups % 100u + (add * 100u) + (cups - cups % 100u) * 1000u;
    case 5: return cups % 10u + (add * 10u) + (cups - cups % 10u) * 1000u;
    case 6: return cups * 1000u + add;
    default: std::abort();
    }
}

// rotateRight(abcdefghi, 0) = abcdefghi
// rotateRight(abcdefghi, 1) = iabcdefgh
// rotateRight(abcdefghi, 2) = hiabcdefg
// rotateRight(abcdefghi, 7) = cdefghiab
// rotateRight(abcdefghi, 8) = bcdefghia
// rotateRight(abcdefghi, 9) = abcdefghi
u32 rotateRight(u64 cups, u32 rot) {
    cups *= 1000000001ull;   // abcdefghiabcdefghi
    constexpr u64 divisors[] = {
                1ull, // 0
               10ull, // 1
              100ull, // 2
             1000ull, // 3
            10000ull, // 4
           100000ull, // 5
          1000000ull, // 6
         10000000ull, // 7
        100000000ull, // 8
    };
    return cups / divisors[rot % 9] % 1000000000ull;
}

u32 add(u32 x, u32 y, u32 limit) {
    u32 res = x + y;
    if (res > limit) res -= limit;
    return res;
}

u32 sub(u32 x, u32 y, u32 limit) {
    u32 res = x - y + limit;
    if (res > limit) res -= limit;
    if (res == 0) res = limit;
    return res;
}

//       123456789
// shift(abcdefghi, 2, 5) = aebcdfghi
// shift(abcdefghi, 4, 1) = adefbcghi -> hiadefbcg   (rotated so that the next cup remains at the same position)
u32 shift(u32 cups, u32 src, u32 dst) {
    assert(src >= 1 && src <= 9);
    assert(dst >= 1 && dst <= 9);
    if (src == dst) return cups;

    u32 insertOffset;
    switch (src) {
    case 8: insertOffset = 1; break;
    case 9: insertOffset = 2; break;
    default: insertOffset = (src < dst) ? 3 : 0; break;
    }
    u32 nums = extract(cups, src);
    cups = remove(cups, src);
    cups = insert(cups, nums, sub(dst, insertOffset, 9));
    if (src > dst) {
        // rotate so that the current cup remains at the same position
        switch (src) {
        case 8: cups = rotateRight(cups, 7); break;
        case 9: cups = rotateRight(cups, 8); break;
        default: cups = rotateRight(cups, 6); break;
        }
    }
    return cups;
}

void part1(u32 cups) {
    auto addMod = [](u32 x, u32 y) { return add(x, y, 9); };
    auto subMod = [](u32 x, u32 y) { return sub(x, y, 9); };

    u32 pos = 1u;
    for (u32 turn = 1u; turn <= 100u; turn++) {
        u32 current = digit(cups, pos);
        u32 nextTrio = extract(cups, addMod(pos, 1));
        u32 dest = subMod(current, 1);
        while (hasDigit(nextTrio, dest)) dest = subMod(dest, 1);
        u32 destPos = digitPos(cups, dest);
        cups = shift(cups, addMod(pos, 1), destPos);
        pos = addMod(pos, 1);
    }
    u32 onePos = digitPos(cups, 1);
    cups = rotateRight(cups, subMod(2, onePos) - 1) % 100000000u;

    std::cout << "part 1: " << cups << '\n';
}

struct Cup {
    u32 number;
    Cup* prev = nullptr;
    Cup* next = nullptr;
};

void part1Pointers(u32 startingCups) {
    std::array<Cup, 10> cups;

    // Initialize array
    Cup* first = &cups[startingCups % 10u];
    Cup* next = first;
    cups[startingCups % 10u].number = startingCups % 10u;
    startingCups /= 10u;
    for (size_t i = 0; i < 8; i++) {
        u32 pos = startingCups % 10u;
        cups[pos].next = next;
        cups[pos].number = pos;
        next->prev = &cups[pos];
        next = &cups[pos];
        startingCups /= 10u;
    }
    first->next = next;
    next->prev = first;
    Cup* last = next;
    
    auto subMod = [](u32 x, u32 y) { return sub(x, y, 9); };

    Cup* current = last;
    for (u32 turn = 1u; turn <= 100u; turn++) {
        Cup& trio1 = *current->next;
        Cup& trio2 = *trio1.next;
        Cup& trio3 = *trio2.next;
        u32 destPos = subMod(current->number, 1);
        size_t nextTrioCount = 0;
        while (nextTrioCount < 3 && (destPos == trio1.number || destPos == trio2.number || destPos == trio3.number)) {
            destPos = subMod(destPos, 1);
        }
        Cup& dest = cups[destPos];

        current->next = trio3.next;
        trio3.next->prev = current;

        trio1.prev = &dest;
        trio3.next = dest.next;

        dest.next = &trio1;
        trio3.next->prev = &trio3;

        current = current->next;
    }

    current = &cups[1];
    std::cout << "part 1: ";
    for (size_t i = 0; i < 8; i++) {
        current = current->next;
        std::cout << current->number;
    }
    std::cout << '\n';
}
void part2(u32 startingCups) {
    std::unique_ptr<std::array<Cup, 1000001>> cupsPtr = std::make_unique<std::array<Cup, 1000001>>();
    auto& cups = *cupsPtr;

    // Initialize array
    Cup* first = nullptr;
    Cup* prev = &cups[1000000];
    for (size_t i = 0; i < 9; i++) {
        u32 pos = startingCups % 10u;
        cups[pos].next = prev;
        cups[pos].number = pos;
        prev->prev = &cups[pos];
        if (first == nullptr) first = &cups[pos];
        prev = &cups[pos];
        startingCups /= 10u;
    }
    Cup* last = prev;
    prev = first;
    for (u32 i = 10u; i <= 1000000u; i++) {
        prev->next = &cups[i];
        cups[i].prev = prev;
        prev = &cups[i];
        cups[i].number = i;
    }
    prev->next = last;
    last->prev = prev;

    auto subMod = [](u32 x, u32 y) { return sub(x, y, 1000000); };

    Cup* current = last;
    for (u32 turn = 1u; turn <= 10'000'000u; turn++) {
        Cup& trio1 = *current->next;
        Cup& trio2 = *trio1.next;
        Cup& trio3 = *trio2.next;
        u32 destPos = subMod(current->number, 1);
        size_t nextTrioCount = 0;
        while (nextTrioCount < 3 && (destPos == trio1.number || destPos == trio2.number || destPos == trio3.number)) {
            destPos = subMod(destPos, 1);
        }
        Cup& dest = cups[destPos];

        current->next = trio3.next;
        trio3.next->prev = current;

        trio1.prev = &dest;
        trio3.next = dest.next;

        dest.next = &trio1;
        trio3.next->prev = &trio3;

        current = current->next;
    }

    u64 result = (u64)cups[1].next->number * cups[1].next->next->number;
    std::cout << "part 2: " << result << '\n';
}

u32 loadInput() {
    return 368195742;
    //return 389125467;
}

int main() {
    auto cups = loadInput();
    part1(cups);
    part1Pointers(cups);
    part2(cups);
    return 0;
}
