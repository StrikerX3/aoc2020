#include <iostream>
#include <fstream>
#include <string>
#include <vector>

template <typename SeatCountFunc>
bool simulate(std::vector<std::string>& seats, size_t occupiedCount, SeatCountFunc&& seatCount) {
    bool stateChanged = false;
    std::vector<std::string> newState;
    newState.reserve(seats.size());
    for (size_t y = 0; y < seats.size(); y++) {
        auto& row = seats[y];
        std::string newRow = row;
        for (size_t x = 0; x < row.size(); x++) {
            size_t occupiedNeighbors = seatCount(seats, x, y);
            char ch = row[x];
            if (ch == 'L' && occupiedNeighbors == 0) {
                newRow[x] = '#';
                stateChanged = true;
            }
            else if (ch == '#' && occupiedNeighbors >= occupiedCount) {
                newRow[x] = 'L';
                stateChanged = true;
            }
        }
        newState.push_back(newRow);
    }
    seats = newState;
    return stateChanged;
}

void part1(const std::vector<std::string>& seats) {
    std::vector<std::string> state = seats;
    while (simulate(state, 4, [](auto& seats, size_t x, size_t y) -> size_t {
        size_t occupiedNeighbors = 0;
        for (int dy = -1; dy <= 1; dy++) {
            auto ny = y + dy;
            if (ny >= seats.size()) continue;
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                auto nx = x + dx;
                if (nx >= seats.size()) continue;
                if (seats[ny][nx] == '#') occupiedNeighbors++;
            }
        }
        return occupiedNeighbors;
    })) {
    }
    size_t count = 0;
    for (auto& row : state) {
        for (char ch : row) {
            if (ch == '#') count++;
        }
    }
    std::cout << "part 1: " << count << '\n';
}

void part2(const std::vector<std::string>& seats) {
    std::vector<std::string> state = seats;
    while (simulate(state, 5, [](auto& seats, size_t x, size_t y) -> size_t {
        size_t occupiedNeighbors = 0;
        constexpr std::pair<int, int> directions[] = {
            {-1, -1}, { 0, -1}, {+1, -1},
            {-1,  0}, /*0,  0*/ {+1,  0},
            {-1, +1}, { 0, +1}, {+1, +1}
        };
        size_t sy = seats.size();
        size_t sx = seats[0].size();
        for (auto& dir : directions) {
            size_t nx = x + dir.first;
            size_t ny = y + dir.second;
            while (nx < sx && ny < sy) {
                if (seats[ny][nx] == '#') {
                    occupiedNeighbors++;
                    break;
                }
                else if (seats[ny][nx] == 'L') {
                    break;
                }
                nx += dir.first;
                ny += dir.second;
            }
        }
        return occupiedNeighbors;
    })) {
    }
    size_t count = 0;
    for (auto& row : state) {
        for (char ch : row) {
            if (ch == '#') count++;
        }
    }
    std::cout << "part 2: " << count << '\n';
}

auto loadInput() {
    std::vector<std::string> seats;
    std::ifstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        seats.push_back(line);
    }
    return seats;
}

int main() {
    auto seats = loadInput();
    part1(seats);
    part2(seats);
    return 0;
}
