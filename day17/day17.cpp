#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cstdint>
#include <unordered_set>

using s32 = int32_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct Coord {
    s32 x, y, z, w;

    Coord operator+(const Coord& coord) const {
        return { x + coord.x, y + coord.y, z + coord.z, w + coord.w };
    }

    bool operator==(const Coord& coord) const {
        return x == coord.x && y == coord.y && z == coord.z && w == coord.w;
    }
};

namespace std {

template <>
struct hash<Coord> {
    std::size_t operator()(const Coord& s) const noexcept {
        size_t h = 17;
        h = 31 * h + std::hash<u32>{}(s.x);
        h = 31 * h + std::hash<u32>{}(s.y);
        h = 31 * h + std::hash<u32>{}(s.z);
        return h;
    }
};

} // namespace std

struct PocketDimension {
    PocketDimension(const std::vector<std::string>& initialState) {
        for (s32 y = 0; y < initialState.size(); y++) {
            auto& row = initialState[y];
            for (s32 x = 0; x < row.size(); x++) {
                if (row[x] == '#') {
                    activeCells.insert({ x, y, 0, 0 });
                    maxCoord.x = std::max(maxCoord.x, x);
                    maxCoord.y = std::max(maxCoord.y, y);
                }
            }
        }
    }

    void simulate3D() {
        constexpr auto neighbors = []() -> auto {
            std::array<Coord, 3 * 3 * 3 - 1> coords;
            size_t i = 0;
            for (s32 z = -1; z <= +1; z++) {
                for (s32 y = -1; y <= +1; y++) {
                    for (s32 x = -1; x <= +1; x++) {
                        if (x == 0 && y == 0 && z == 0) continue;
                        coords[i++] = { x, y, z, 0 };
                    }
                }
            }
            return coords;
        }();

        Coord newMinCoord{ 0, 0, 0, 0 };
        Coord newMaxCoord{ 0, 0, 0, 0 };

        std::unordered_set<Coord> newState;

        for (s32 z = minCoord.z - 1; z <= maxCoord.z + 1; z++) {
            for (s32 y = minCoord.y - 1; y <= maxCoord.y + 1; y++) {
                for (s32 x = minCoord.x - 1; x <= maxCoord.x + 1; x++) {
                    Coord coord{ x, y, z, 0 };
                    u32 neighborCount = 0;
                    for (auto& neighbor : neighbors) {
                        if (activeCells.contains(coord + neighbor)) {
                            neighborCount++;
                        }
                    }
                    bool currActive = activeCells.contains(coord);
                    if ((currActive && (neighborCount == 2 || neighborCount == 3))
                        || (!currActive && neighborCount == 3)) {

                        newState.insert(coord);

                        newMinCoord.x = std::min(x, newMinCoord.x);
                        newMinCoord.y = std::min(y, newMinCoord.y);
                        newMinCoord.z = std::min(z, newMinCoord.z);

                        newMaxCoord.x = std::max(x, newMaxCoord.x);
                        newMaxCoord.y = std::max(y, newMaxCoord.y);
                        newMaxCoord.z = std::max(z, newMaxCoord.z);
                    }
                }
            }
        }
        activeCells = newState;
        minCoord = newMinCoord;
        maxCoord = newMaxCoord;
    }

    void simulate4D() {
        constexpr auto neighbors = []() -> auto {
            std::array<Coord, 3 * 3 * 3 * 3 - 1> coords;
            size_t i = 0;
            for (s32 w = -1; w <= +1; w++) {
                for (s32 z = -1; z <= +1; z++) {
                    for (s32 y = -1; y <= +1; y++) {
                        for (s32 x = -1; x <= +1; x++) {
                            if (x == 0 && y == 0 && z == 0 && w == 0) continue;
                            coords[i++] = { x, y, z, w };
                        }
                    }
                }
            }
            return coords;
        }();

        Coord newMinCoord{ 0, 0, 0, 0 };
        Coord newMaxCoord{ 0, 0, 0, 0 };

        std::unordered_set<Coord> newState;

        for (s32 w = minCoord.w - 1; w <= maxCoord.w + 1; w++) {
            for (s32 z = minCoord.z - 1; z <= maxCoord.z + 1; z++) {
                for (s32 y = minCoord.y - 1; y <= maxCoord.y + 1; y++) {
                    for (s32 x = minCoord.x - 1; x <= maxCoord.x + 1; x++) {
                        Coord coord{ x, y, z, w };
                        u32 neighborCount = 0;
                        for (auto& neighbor : neighbors) {
                            if (activeCells.contains(coord + neighbor)) {
                                neighborCount++;
                            }
                        }
                        bool currActive = activeCells.contains(coord);
                        if ((currActive && (neighborCount == 2 || neighborCount == 3))
                            || (!currActive && neighborCount == 3)) {

                            newState.insert(coord);

                            newMinCoord.x = std::min(x, newMinCoord.x);
                            newMinCoord.y = std::min(y, newMinCoord.y);
                            newMinCoord.z = std::min(z, newMinCoord.z);
                            newMinCoord.w = std::min(w, newMinCoord.w);

                            newMaxCoord.x = std::max(x, newMaxCoord.x);
                            newMaxCoord.y = std::max(y, newMaxCoord.y);
                            newMaxCoord.z = std::max(z, newMaxCoord.z);
                            newMaxCoord.w = std::max(z, newMaxCoord.w);
                        }
                    }
                }
            }
        }
        activeCells = newState;
        minCoord = newMinCoord;
        maxCoord = newMaxCoord;
    }

    void simulate3D(u32 iterations) {
        for (u32 i = 0; i < iterations; i++) {
            simulate3D();
        }
    }

    void simulate4D(u32 iterations) {
        for (u32 i = 0; i < iterations; i++) {
            simulate4D();
        }
    }

    size_t cellCount() {
        return activeCells.size();
    }

    std::unordered_set<Coord> activeCells;
    Coord minCoord{ 0, 0, 0 };
    Coord maxCoord{ 0, 0, 0 };
};

void part1(const std::vector<std::string>& initialState) {
    PocketDimension dimension{ initialState };
    dimension.simulate3D(6);
    std::cout << "part 1: " << dimension.cellCount() << '\n';
}

void part2(const std::vector<std::string>& initialState) {
    PocketDimension dimension{ initialState };
    dimension.simulate4D(6);
    std::cout << "part 2: " << dimension.cellCount() << '\n';
}

std::vector<std::string> loadInput() {
    std::vector<std::string> initialState;
    std::fstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        initialState.push_back(line);
    }
    return initialState;
}

int main() {
    auto initialState = loadInput();
    part1(initialState);
    part2(initialState);
    return 0;
}
