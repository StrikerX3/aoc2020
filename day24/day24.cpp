#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdint>

using s32 = int32_t;

// Directions:
//    nw / \ ne
//     w|   |e
//    sw \ / se

// Coordinate system:
//      ... ... ... ... ...
//      / \ / \ / \ / \ /
// ... |0,0|1,0|2,0|3,0| ...
//      \ / \ / \ / \ / \
//   ... |0,1|1,1|2,1|3,1| ...
//      / \ / \ / \ / \ /
// ... |0,2|1,2|2,2|3,2| ...
//      \ / \ / \ / \ / \
//   ... |0,3|1,3|2,3|3,3| ...
//      / \ / \ / \ / \ /
// ... |0,4|1,4|2,4|3,4| ...
//      \ / \ / \ / \ / \
//      ... ... ... ... ...

// Direction offsets:
//   East: +X
//   West: -X
//   SouthEast: +X when Y is odd; +Y
//   SouthWest: -X when Y is even; +Y
//   NorthEast: +X when Y is odd; -Y
//   NorthWest: -X when Y is even; -Y

enum class Direction {
    East, SouthEast, SouthWest, West, NorthWest, NorthEast
};

struct Coord {
    s32 x = 0, y = 0;

    Coord move(Direction direction) {
        switch (direction) {
        case Direction::East: return { x + 1, y };
        case Direction::West: return { x - 1, y };
        case Direction::SouthEast: return { x + (y & 1), y + 1 };
        case Direction::SouthWest: return { x - ((y & 1) ^ 1), y + 1 };
        case Direction::NorthEast: return { x + (y & 1), y - 1 };
        case Direction::NorthWest: return { x - ((y & 1) ^ 1), y - 1 };
        }
    }

    bool operator==(const Coord& coord) const {
        return x == coord.x && y == coord.y;
    }
};

namespace std {

template <>
struct hash<Coord> {
    std::size_t operator()(const Coord& s) const noexcept {
        size_t h = 17;
        h = 31 * h + std::hash<s32>{}(s.x);
        h = 31 * h + std::hash<s32>{}(s.y);
        return h;
    }
};

} // namespace std

std::unordered_set<Coord> flipTiles(const std::vector<std::vector<Direction>>& tiles) {
    std::unordered_set<Coord> flippedTiles;
    for (auto& tile : tiles) {
        Coord coord;
        for (auto dir : tile) {
            coord = coord.move(dir);
        }
        if (flippedTiles.contains(coord)) {
            flippedTiles.erase(coord);
        }
        else {
            flippedTiles.insert(coord);
        }
    }
    return flippedTiles;
}

void simulate(std::unordered_set<Coord>& blackTiles) {
    // Find the edges of the board
    Coord minCoord, maxCoord;
    for (auto& coord : blackTiles) {
        minCoord.x = std::min(minCoord.x, coord.x);
        minCoord.y = std::min(minCoord.y, coord.y);
        maxCoord.x = std::max(maxCoord.x, coord.x);
        maxCoord.y = std::max(maxCoord.y, coord.y);
    }

    // Simulate next step
    std::unordered_set<Coord> newBlackTiles;
    for (s32 y = minCoord.y - 1; y <= maxCoord.y + 1; y++) {
        for (s32 x = minCoord.x - 1; x <= maxCoord.x + 1; x++) {
            Coord coord{ x, y };

            // Count black neighbors
            s32 neighbors = 0;
            if (blackTiles.contains(coord.move(Direction::East))) neighbors++;
            if (blackTiles.contains(coord.move(Direction::West))) neighbors++;
            if (blackTiles.contains(coord.move(Direction::SouthEast))) neighbors++;
            if (blackTiles.contains(coord.move(Direction::SouthWest))) neighbors++;
            if (blackTiles.contains(coord.move(Direction::NorthEast))) neighbors++;
            if (blackTiles.contains(coord.move(Direction::NorthWest))) neighbors++;

            if (blackTiles.contains(coord)) {
                // Black flips to white if there are 0 or more than 2 black neighbors
                // In other words, black stays black if there is 1 or 2 black neighbors
                if (neighbors == 1 || neighbors == 2) newBlackTiles.insert(coord);
            }
            else {
                // White flips to black if there are 2 black neighbors
                if (neighbors == 2) newBlackTiles.insert(coord);
            }
        }
    }

    // Update tile map
    blackTiles = newBlackTiles;
}

void part1(const std::vector<std::vector<Direction>>& tiles) {
    std::unordered_set<Coord> blackTiles = flipTiles(tiles);
    std::cout << "part 1: " << blackTiles.size() << "\n";
}

void part2(const std::vector<std::vector<Direction>>& tiles) {
    std::unordered_set<Coord> blackTiles = flipTiles(tiles);
    for (size_t turn = 0; turn < 100; turn++) {
        simulate(blackTiles);
    }
    std::cout << "part 2: " << blackTiles.size() << "\n";
}

auto loadInput() {
    std::vector<std::vector<Direction>> tiles;
    std::ifstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        int y = 0;
        std::vector<Direction> tile;
        for (auto c : line) {
            switch (c) {
            case 'n': y = -1; break;
            case 's': y = 1; break;
            case 'e':
                switch (y) {
                case -1: tile.push_back(Direction::NorthEast); break;
                case 0: tile.push_back(Direction::East); break;
                case 1: tile.push_back(Direction::SouthEast); break;
                }
                y = 0;
                break;
            case 'w':
                switch (y) {
                case -1: tile.push_back(Direction::NorthWest); break;
                case 0: tile.push_back(Direction::West); break;
                case 1: tile.push_back(Direction::SouthWest); break;
                }
                y = 0;
                break;
            }
        }
        tiles.push_back(tile);
    }
    return tiles;
}

int main() {
    auto tiles = loadInput();
    part1(tiles);
    part2(tiles);
    return 0;
}
