#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <algorithm>
#include <bit>

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct Tile {
    u32 id;
    std::vector<std::string> map;
    u16 edgeBits[4];

    void rotateCW() {
        // Rotate in place with a 4-way swap
        auto sz = map.size();
        for (size_t y = 0; y < sz / 2; y++) {
            auto fy = y;
            auto ry = sz - 1 - y;
            for (size_t x = y; x < sz - 1 - y; x++) {
                auto fx = x;
                auto rx = sz - 1 - x;
                auto tmp = map[fy][fx];
                map[fy][fx] = map[rx][fy];
                map[rx][fy] = map[ry][rx];
                map[ry][rx] = map[fx][ry];
                map[fx][ry] = tmp;
            }
        }
    }

    void horzFlip() {
        for (auto& row : map) {
            std::reverse(row.begin(), row.end());
        }
    }

    void vertFlip() {
        std::reverse(map.begin(), map.end());
    }

    void matchTop(u16 bits) {
        for (size_t v = 0; v < 2; v++) {
            for (size_t h = 0; h < 2; h++) {
                for (size_t r = 0; r < 4; r++) {
                    if (topBits() == bits) return;
                    rotateCW();
                }
                horzFlip();
            }
            horzFlip();
            vertFlip();
        }
    }

    void matchLeft(u16 bits) {
        for (size_t v = 0; v < 2; v++) {
            for (size_t h = 0; h < 2; h++) {
                for (size_t r = 0; r < 4; r++) {
                    if (lftBits() == bits) return;
                    rotateCW();
                }
                horzFlip();
            }
            horzFlip();
            vertFlip();
        }
    }

    u16 topBits() const {
        u16 bits = 0;
        for (size_t i = 0; i < map.size(); i++) {
            if (map[0][i] == '#') bits |= (1 << i);
        }
        return bits;
    }

    u16 lftBits() const {
        u16 bits = 0;
        for (size_t i = 0; i < map.size(); i++) {
            if (map[i][0] == '#') bits |= (1 << i);
        }
        return bits;
    }

    u16 btmBits() const {
        u16 bits = 0;
        for (size_t i = 0; i < map.size(); i++) {
            if (map[map.size() - 1][i] == '#') bits |= (1 << i);
        }
        return bits;
    }

    u16 rgtBits() const {
        u16 bits = 0;
        for (size_t i = 0; i < map.size(); i++) {
            if (map[i][map.size() - 1] == '#') bits |= (1 << i);
        }
        return bits;
    }

    void calcEdgeBits() {
        edgeBits[0] = topBits();
        edgeBits[1] = lftBits();
        edgeBits[2] = btmBits();
        edgeBits[3] = rgtBits();
    }
};

u16 bitReverse10(u16 bits) {
    u16 result = ((bits & 0b11111'00000) >> 5) | ((bits & 0b00000'11111) << 5);
    return ((result & 0b10000'10000) >> 4) | ((result & 0b01000'01000) >> 2)
        | ((result & 0b00100'00100))
        | ((result & 0b00010'00010) << 2) | ((result & 0b00001'00001) << 4);
}

void part1(std::vector<Tile>& tiles) {
    // Build lookup tables for edge bits -> count
    std::unordered_map<u16, u32> edgeCounts;
    for (auto& tile : tiles) {
        for (auto& bits : tile.edgeBits) {
            edgeCounts[bits]++;
            edgeCounts[bitReverse10(bits)]++;
        }
    }

    u64 total = 1;
    for (auto& tile : tiles) {
        // Find non-shared edges
        std::unordered_set<u16> uniqueEdges;
        u16 uniqueEdgeMask = 0;
        for (size_t i = 0; i < 4; i++) {
            auto edgeBits = tile.edgeBits[i];
            auto edgeCount = edgeCounts[edgeBits];
            auto revEdgeCount = edgeCounts[bitReverse10(edgeBits)];
            if (edgeCount == 1 && revEdgeCount == 1) {
                uniqueEdgeMask |= (1 << i);
                uniqueEdges.insert(edgeBits);
                uniqueEdges.insert(bitReverse10(edgeBits));
            }
        }

        // Corner tiles will have two unique edges
        if (std::popcount(uniqueEdgeMask) == 2) {
            total *= tile.id;
        }
    }

    std::cout << "part 1: " << total << '\n';
}

void part2(std::vector<Tile>& tiles) {
    // Build lookup tables for edge bits -> count and edge bits -> tiles
    std::unordered_map<u16, u32> edgeCounts;
    std::unordered_multimap<u16, Tile*> tileLookup;
    for (auto& tile : tiles) {
        for (auto& bits : tile.edgeBits) {
            edgeCounts[bits]++;
            edgeCounts[bitReverse10(bits)]++;
            tileLookup.insert({ bits, &tile });
            tileLookup.insert({ bitReverse10(bits), &tile });
        }
    }

    // Find any corner tile
    Tile* cornerTile = nullptr;
    for (auto& tile : tiles) {
        // Find non-shared edges
        std::unordered_set<u16> uniqueEdges;
        u16 uniqueEdgeMask = 0;
        for (size_t i = 0; i < 4; i++) {
            auto edgeBits = tile.edgeBits[i];
            auto edgeCount = edgeCounts[edgeBits];
            auto revEdgeCount = edgeCounts[bitReverse10(edgeBits)];
            if (edgeCount == 1 && revEdgeCount == 1) {
                uniqueEdgeMask |= (1 << i);
                uniqueEdges.insert(edgeBits);
                uniqueEdges.insert(bitReverse10(edgeBits));
            }
        }

        // Use the first corner tile we can find
        if (std::popcount(uniqueEdgeMask) == 2) {
            cornerTile = &tile;
            break;
        }
    }

    // Stitch image
    std::array<std::array<const Tile*, 12>, 12> stitchedImage;
    for (size_t y = 0; y < 12; y++) {
        for (size_t x = 0; x < 12; x++) {
            if (x == 0 && y == 0) {
                // Top-left corner
                // Rotate/flip the corner tile to place the unique edges at the top and left
                for (size_t i = 0; i < 8; i++) {
                    if (edgeCounts[cornerTile->topBits()] == 1 && edgeCounts[cornerTile->lftBits()]) {
                        // Found the correct orientation
                        break;
                    }
                    cornerTile->rotateCW();
                    if (i == 4) cornerTile->horzFlip();
                }
                stitchedImage[0][0] = cornerTile;
            }
            else if (y == 0) {
                // Top row
                // Match the left edge of the tile with the right edge of the tile to its left
                Tile* tile = nullptr;
                auto* leftTile = stitchedImage[0][x - 1];
                auto edgeBits = leftTile->rgtBits();
                auto range = tileLookup.equal_range(edgeBits);
                for (auto it = range.first; it != range.second; it++) {
                    if (it->second != leftTile) {
                        tile = it->second;
                        break;
                    }
                }
                tile->matchLeft(edgeBits);
                stitchedImage[0][x] = tile;
                tileLookup.erase(leftTile->topBits());
            }
            else {
                // Remaining rows
                // Match the top edge of the tile with the bottom edge of the tile above it
                Tile* tile = nullptr;
                auto* aboveTile = stitchedImage[y - 1][x];
                auto edgeBits = aboveTile->btmBits();
                auto range = tileLookup.equal_range(edgeBits);
                for (auto it = range.first; it != range.second; it++) {
                    if (it->second != aboveTile) {
                        tile = it->second;
                        break;
                    }
                }
                tile->matchTop(edgeBits);
                stitchedImage[y][x] = tile;
            }
        }
    }

    // Construct full image, discarding edges, and count number of marked cells
    std::array<std::array<bool, 96>, 96> image;
    u32 cellCount = 0;
    for (size_t ty = 0; ty < 12; ty++) {
        for (size_t tx = 0; tx < 12; tx++) {
            auto& tile = *stitchedImage[ty][tx];
            for (size_t y = 0; y < 8; y++) {
                for (size_t x = 0; x < 8; x++) {
                    bool filled = (tile.map[y + 1][x + 1] == '#');
                    image[ty * 8 + y][tx * 8 + x] = filled;
                    if (filled) {
                        cellCount++;
                    }
                }
            }
        }
    }

    // The sea monster
    static constexpr std::array<const char*, 3> monster{
        "..................#.",
        "#....##....##....###",
        ".#..#..#..#..#..#..."
    };
    static constexpr u64 monsterCellCount = 15;
    static constexpr auto monsterHeight = monster.size();
    static constexpr auto monsterWidth = 20;

    // Exclude sea monsters from the cell count
    u32 totalMonsterSum = 0;
    for (size_t h = 0; h < 2; h++) {
        for (size_t r = 0; r < 4; r++) {
            // Scan map for the sea monster
            for (size_t y = 0; y < 96 - monsterHeight; y++) {
                for (size_t x = 0; x < 96 - monsterWidth; x++) {
                    u32 count = 0;
                    for (size_t my = 0; my < monsterHeight; my++) {
                        for (size_t mx = 0; mx < monsterWidth; mx++) {
                            if ((monster[my][mx] == '#') && image[y + my][x + mx]) count++;
                        }
                    }
                    if (count == monsterCellCount) {
                        totalMonsterSum += monsterCellCount;
                    }
                }
            }

            // Stop if we already found sea monsters in the image
            if (totalMonsterSum > 0) break;

            // Rotate image clockwise in place
            auto sz = image.size();
            for (size_t y = 0; y < sz / 2; y++) {
                auto fy = y;
                auto ry = sz - 1 - y;
                for (size_t x = y; x < sz - 1 - y; x++) {
                    auto fx = x;
                    auto rx = sz - 1 - x;
                    auto tmp = image[fy][fx];
                    image[fy][fx] = image[rx][fy];
                    image[rx][fy] = image[ry][rx];
                    image[ry][rx] = image[fx][ry];
                    image[fx][ry] = tmp;
                }
            }
        }

        // Flip image horizontally
        for (auto& row : image) {
            std::reverse(row.begin(), row.end());
        }
    }

    std::cout << "part 2: " << (cellCount - totalMonsterSum) << '\n';
}

std::vector<Tile> loadInput() {
    std::vector<Tile> tiles;
    std::fstream f{ "input.txt" };
    std::string line;
    Tile tile;
    while (std::getline(f, line)) {
        if (line.empty()) {
            tile.calcEdgeBits();
            tiles.push_back(tile);
            tile.map.clear();
        }
        else if (line.starts_with("Tile")) {
            tile.id = std::stoul(line.substr(5, line.find(':') - 5));
        }
        else {
            tile.map.push_back(line);
        }
    }
    return tiles;
}

int main() {
    auto tiles = loadInput();
    part1(tiles);
    part2(tiles);
    return 0;
}
