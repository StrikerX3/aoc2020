#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <deque>
#include <bitset>
#include <vector>
#include <cstdint>
#include <unordered_set>

using u32 = uint32_t;
using u64 = uint64_t;

struct Game {
    std::array<std::vector<u32>, 2> playerHands;
};

void part1(const Game& game) {
    std::deque<u32> p1{ game.playerHands[0].begin(), game.playerHands[0].end() };
    std::deque<u32> p2{ game.playerHands[1].begin(), game.playerHands[1].end() };

    while (!p1.empty() && !p2.empty()) {
        auto p1Card = p1.front(); p1.pop_front();
        auto p2Card = p2.front(); p2.pop_front();
        if (p1Card > p2Card) {
            p1.push_back(p1Card);
            p1.push_back(p2Card);
        }
        else {
            p2.push_back(p2Card);
            p2.push_back(p1Card);
        }
    }

    u32 score = 0;
    auto& winner = p1.empty() ? p2 : p1;
    for (size_t i = 0; i < winner.size(); i++) {
        score += winner[i] * (winner.size() - i);
    }

    std::cout << "part 1: " << score << '\n';
}

struct GameState {
    std::bitset<50> p1;
    std::bitset<50> p2;

    GameState(const std::deque<u32>& p1, const std::deque<u32>& p2) {
        for (auto c : p1) {
            this->p1.set(c - 1);
        }
        for (auto c : p2) {
            this->p2.set(c - 1);
        }
    }

    bool operator==(const GameState& state) const {
        return p1 == state.p1 && p2 == state.p2;
    }
};

template <>
struct std::hash<GameState> {
    std::size_t operator()(const GameState& s) const noexcept {
        size_t h = 17;
        h = 31 * h + std::hash<u64>{}(s.p1.to_ullong());
        h = 31 * h + std::hash<u64>{}(s.p2.to_ullong());
        return h;
    }
};


u32 recursiveCombat(std::deque<u32>& p1, std::deque<u32>& p2) {
    //static u32 gameCounter = 0;
    //u32 game = ++gameCounter;
    std::unordered_set<GameState> previousGameStates;

    //std::cout << "=== Game " << (game) << " ===\n\n";

    //auto printDeck = [](const std::deque<u32>& deck) {
    //    for (auto c : deck) {
    //        std::cout << ' ' << c;
    //    }
    //};

    //u32 round = 0;
    while (!p1.empty() && !p2.empty()) {
        //std::cout << "-- Round " << (++round) << " (Game " << game << ") --\n";
        //std::cout << "Player 1's deck:"; printDeck(p1); std::cout << '\n';
        //std::cout << "Player 2's deck:"; printDeck(p2); std::cout << '\n';

        // Player 1 automatically wins if the same game state was already played before
        if (!previousGameStates.insert({ p1, p2 }).second) {
            //std::cout << "The winner of game " << game << " is player 1!\n\n";
            return 1;
        }

        // Otherwise players draw a card
        auto p1Card = p1.front(); p1.pop_front();
        auto p2Card = p2.front(); p2.pop_front();
        //std::cout << "Player 1 plays: " << p1Card << '\n';
        //std::cout << "Player 2 plays: " << p2Card << '\n';

        u32 winner;
        if (p1.size() >= p1Card && p2.size() >= p2Card) {
            // Play a sub-game of Recursive Combat if both players have at least
            // as many cards as the value of the cards they drew to determine the winner
            //std::cout << "Playing a sub-game to determine the winner...\n\n";
            std::deque<u32> p1Copy{ p1.begin(), p1.begin() + p1Card };
            std::deque<u32> p2Copy{ p2.begin(), p2.begin() + p2Card };
            winner = recursiveCombat(p1Copy, p2Copy);
            //std::cout << "...anyway, back to game " << game << ".\n";
        }
        else {
            // Play a normal round of Combat
            winner = (p1Card > p2Card) ? 1 : 2;
        }
        //std::cout << "Player " << winner << " wins round " << round << " of game " << game << "!\n\n";

        // Move cards accordingly
        if (winner == 1) {
            p1.push_back(p1Card);
            p1.push_back(p2Card);
        }
        else {
            p2.push_back(p2Card);
            p2.push_back(p1Card);
        }
    }

    //std::cout << "The winner of game " << game << " is player " << (p1.empty() ? 2 : 1) << "!\n\n";
    return p1.empty() ? 2 : 1;
}

void part2(const Game& game) {
    std::deque<u32> p1{ game.playerHands[0].begin(), game.playerHands[0].end() };
    std::deque<u32> p2{ game.playerHands[1].begin(), game.playerHands[1].end() };

    u32 score = 0;
    auto& winner = (recursiveCombat(p1, p2) == 1) ? p1 : p2;
    for (size_t i = 0; i < winner.size(); i++) {
        score += winner[i] * (winner.size() - i);
    }

    std::cout << "part 2: " << score << '\n';
}

Game loadInput() {
    Game game;
    std::ifstream f{ "input.txt" };
    std::string line;
    std::vector<u32>* hand = &game.playerHands[0];
    std::getline(f, line); // skip "Player 1"
    while (std::getline(f, line)) {
        if (line.empty()) {
            continue;
        }
        else if (line.starts_with("Player 2")) {
            hand = &game.playerHands[1];
        }
        else {
            hand->push_back(std::stoul(line));
        }
    }
    return game;
}

int main() {
    auto game = loadInput();
    part1(game);
    part2(game);
    return 0;
}