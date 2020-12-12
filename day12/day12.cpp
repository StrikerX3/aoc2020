#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using s32 = int32_t;
using u32 = uint32_t;

struct Action {
    char type;
    u32 value;

    friend std::istream& operator>>(std::istream& is, Action& action) {
        return is >> action.type >> action.value;
    }
};

// Coordinate system:
//          North (+Y)
//             ^
// West (-X) <-+-> East (+X)
//             v
//          South (-Y)

enum class Direction {
    East,  // +X
    South, // -Y
    West,  // -X
    North  // +Y
};

Direction RotateCW(const Direction direction, const s32 degrees) {
    constexpr Direction rotations[4][4] = {
        { Direction::East,  Direction::South, Direction::West,  Direction::North, },
        { Direction::South, Direction::West,  Direction::North, Direction::East,  },
        { Direction::West,  Direction::North, Direction::East,  Direction::South, },
        { Direction::North, Direction::East,  Direction::South, Direction::West,  },
    };
    auto index = degrees / 90;
    return rotations[index][static_cast<size_t>(direction)];
}


struct Coord {
    s32 x = 0;
    s32 y = 0;

    Coord& operator+=(const Coord& coord) {
        x += coord.x;
        y += coord.y;
        return *this;
    }

    Coord operator*(s32 factor) const {
        return { x * factor, y * factor };
    }

    void Move(const Direction direction, const u32 units) {
        switch (direction) {
        case Direction::East: x += units; break;
        case Direction::South: y -= units; break;
        case Direction::West: x -= units; break;
        case Direction::North: y += units; break;
        }
    }

    u32 ManhattanDistance() const {
        return std::abs(x) + std::abs(y);
    }
};

class Waypoint {
public:
    void Execute(const Action& action) {
        switch (action.type) {
        case 'E': coord.x += action.value; break;
        case 'S': coord.y -= action.value; break;
        case 'W': coord.x -= action.value; break;
        case 'N': coord.y += action.value; break;
        case 'R': RotateCW(action.value); break;
        case 'L': RotateCW(360 - action.value); break;
        }
    }

    const Coord& Coordinates() const { return coord; }

private:
    void RotateCW(u32 degrees) {
        constexpr s32 rotationMatrices[4][2][2] = {
            // 0 degrees
            {
                { +1,  0 },
                {  0, +1 },
            },
            // 90 degrees
            {
                {  0, +1 },
                { -1,  0 },
            },
            // 180 degrees
            {
                { -1,  0 },
                {  0, -1 },
            },
            // 270 degrees
            {
                {  0, -1 },
                { +1,  0 },
            },
        };
        auto index = degrees / 90;
        auto& matrix = rotationMatrices[index];
        coord = {
            coord.x * matrix[0][0] + coord.y * matrix[0][1],
            coord.x * matrix[1][0] + coord.y * matrix[1][1],
        };
    }

    Coord coord{ 10, 1 };
};

class Ship {
public:
    void Execute(const Action& action) {
        switch (action.type) {
        case 'E': coord.x += action.value; break;
        case 'S': coord.y -= action.value; break;
        case 'W': coord.x -= action.value; break;
        case 'N': coord.y += action.value; break;
        case 'R': direction = RotateCW(direction, action.value); break;
        case 'L': direction = RotateCW(direction, 360 - action.value); break;
        case 'F': coord.Move(direction, action.value); break;
        }
    }

    void Move(const Waypoint& waypoint, u32 factor) {
        coord += waypoint.Coordinates() * factor;
    }

    u32 ManhattanDistance() { return coord.ManhattanDistance(); }

private:
    Direction direction = Direction::East;
    Coord coord;
};

void part1(const std::vector<Action>& actions) {
    Ship ship;
    for (auto& action : actions) {
        ship.Execute(action);
    }
    std::cout << "part 1: " << ship.ManhattanDistance() << '\n';
}

void part2(const std::vector<Action>& actions) {
    Ship ship;
    Waypoint waypoint;
    for (auto& action : actions) {
        if (action.type == 'F') {
            ship.Move(waypoint, action.value);
        }
        else {
            waypoint.Execute(action);
        }
    }
    std::cout << "part 1: " << ship.ManhattanDistance() << '\n';
}

std::vector<Action> loadInput() {
    std::vector<Action> actions;
    std::ifstream f{ "input.txt" };
    Action action;
    while (f >> action) {
        actions.push_back(action);
    }
    return actions;

}

int main() {
    auto actions = loadInput();
    part1(actions);
    part2(actions);
    return 0;
}
