#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using s32 = int32_t;

struct Instruction {
    enum class Opcode { Acc, Jmp, Nop };
    Opcode opcode;
    s32 argument;
};

struct Interpreter {
    Interpreter(const std::vector<Instruction>& program)
        : program(program.begin(), program.end()) {
        visited.resize(program.size());
    }

    Interpreter(const Interpreter& interpreter)
        : program(interpreter.program.begin(), interpreter.program.end())
        , visited(interpreter.visited.begin(), interpreter.visited.end())
        , acc(interpreter.acc)
        , pc(interpreter.pc) {
    }

    std::vector<Instruction> program;
    std::vector<bool> visited;
    s32 acc = 0;
    size_t pc = 0;

    void Reset() { acc = 0; pc = 0; std::fill(visited.begin(), visited.end(), false); }

    Instruction CurrentInstruction() {
        // could be out-of-bounds!
        return program[pc];
    }

    void ReplaceInstruction(const Instruction& instruction) {
        // could be out-of-bounds!
        program[pc] = instruction;
    }

    // returns true if the program finished, false on infinite loop
    bool Run() {
        while (!Finished()) {
            if (!RunOnce()) {
                return false;
            }
        }
        return true;
    }

    // returns true if a instruction was executed, false if an infinite loop was detected
    bool RunOnce() {
        if (Finished()) {
            return false;
        }

        auto& instr = program[pc];

        if (visited[pc]) {
            return false;
        }
        visited[pc] = true;

        switch (instr.opcode) {
        case Instruction::Opcode::Acc: acc += instr.argument; pc++; break;
        case Instruction::Opcode::Jmp: pc += instr.argument; break;
        case Instruction::Opcode::Nop: pc++; break;
        }

        return true;
    }

    bool Finished() {
        return pc >= program.size();
    }
};

void part1(const std::vector<Instruction>& program) {
    Interpreter interpreter{ program };
    interpreter.Run();
    std::cout << "part 1: " << interpreter.acc << "\n";
}

void part2(const std::vector<Instruction>& program) {
    Interpreter interpreter{ program };
    while (!interpreter.Finished()) {
        auto instr = interpreter.CurrentInstruction();
        bool patch = false;
        switch (instr.opcode) {
        case Instruction::Opcode::Jmp: patch = true; instr.opcode = Instruction::Opcode::Nop; break;
        case Instruction::Opcode::Nop: patch = true; instr.opcode = Instruction::Opcode::Jmp; break;
        case Instruction::Opcode::Acc: break;
        }
        if (patch) {
            Interpreter interpreterCopy{ interpreter };
            interpreterCopy.ReplaceInstruction(instr);
            if (interpreterCopy.Run()) {
                std::cout << "part 2: " << interpreterCopy.acc << "\n";
                return;
            }
        }
        interpreter.RunOnce();
    }
    std::cout << "part 2: " << interpreter.acc << "\n";
}

auto loadInput() {
    std::ifstream f{ "input.txt" };
    std::vector<Instruction> program;
    Instruction instruction;
    std::string line;
    while (std::getline(f, line)) {
        auto opcode = line.substr(0, 3);
        auto arg = line.substr(4);
        if (opcode == "acc") {
            instruction.opcode = Instruction::Opcode::Acc;
        }
        else if (opcode == "jmp") {
            instruction.opcode = Instruction::Opcode::Jmp;
        }
        else { // nop
            instruction.opcode = Instruction::Opcode::Nop;
        }
        instruction.argument = std::stoi(arg);
        program.push_back(instruction);
    }
    return program;
}

int main() {
    auto program = loadInput();
    part1(program);
    part2(program);
    return 0;
}
