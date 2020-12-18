#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>

using u32 = uint32_t;
using u64 = uint64_t;

struct Token {
    enum class Type { Number, Add, Multiply, OpenParenthesis, CloseParenthesis };
    Type type;
    u32 number;
};

using Expression = std::vector<Token>;

enum class Operation { Add, Multiply, SubExpr };

struct Evaluator {
    std::vector<u64> numStack;
    std::vector<Operation> opStack;

    void add() {
        u64 num1 = numStack.back(); numStack.pop_back();
        u64 num2 = numStack.back(); numStack.pop_back();
        numStack.push_back(num1 + num2);
    }

    void mul() {
        u64 num1 = numStack.back(); numStack.pop_back();
        u64 num2 = numStack.back(); numStack.pop_back();
        numStack.push_back(num1 * num2);
    }

    template <typename OpEval>
    u64 eval(const Expression& expr, OpEval&& opEval) {
        for (auto& token : expr) {
            switch (token.type) {
            case Token::Type::Number:
                numStack.push_back(token.number);
                opEval(*this, false);
                break;
            case Token::Type::Add: opStack.push_back(Operation::Add); break;
            case Token::Type::Multiply: opStack.push_back(Operation::Multiply); break;
            case Token::Type::OpenParenthesis: opStack.push_back(Operation::SubExpr); break;
            case Token::Type::CloseParenthesis:
                opEval(*this, true);
                opStack.pop_back();
                opEval(*this, false);
                break;
            }
        }
        opEval(*this, true);
        return numStack.back();
    }
};

void part1(const std::vector<Expression>& expressions) {
    auto opEval = [](Evaluator& ev, bool exprEnd) {
        auto& ops = ev.opStack;
        while (!ops.empty() && ops.back() != Operation::SubExpr) {
            auto op = ops.back(); ops.pop_back();
            switch (op) {
            case Operation::Add: ev.add(); break;
            case Operation::Multiply: ev.mul(); break;
            default: std::abort();
            }
        }
    };
    u64 sum = 0;
    for (auto& expr : expressions) {
        Evaluator evaluator;
        sum += evaluator.eval(expr, opEval);
    }
    std::cout << "part 1: " << sum << '\n';
}

void part2(const std::vector<Expression>& expressions) {
    auto opEval = [](Evaluator& ev, bool exprEnd) {
        auto& ops = ev.opStack;
        while (!ops.empty() && ops.back() != Operation::SubExpr) {
            auto op = ops.back();
            if (op == Operation::Multiply && !exprEnd) break;
            ops.pop_back();

            switch (op) {
            case Operation::Add: ev.add(); break;
            case Operation::Multiply: ev.mul(); break;
            default: std::abort();
            }
        }
    };
    u64 sum = 0;
    for (auto& expr : expressions) {
        Evaluator evaluator;
        sum += evaluator.eval(expr, opEval);
    }
    std::cout << "part 2: " << sum << '\n';
}

std::vector<Expression> loadInput() {
    std::vector<Expression> expressions;
    std::fstream f{ "input.txt" };
    std::string line;
    while (std::getline(f, line)) {
        Expression expr;
        for (auto ch : line) {
            switch (ch) {
            case '0' ... '9': expr.push_back(Token{ Token::Type::Number, (u32)ch - '0' }); break;
            case '+': expr.push_back(Token{ Token::Type::Add }); break;
            case '*': expr.push_back(Token{ Token::Type::Multiply }); break;
            case '(': expr.push_back(Token{ Token::Type::OpenParenthesis }); break;
            case ')': expr.push_back(Token{ Token::Type::CloseParenthesis }); break;
            }
        }
        expressions.push_back(expr);
    }
    return expressions;
}

int main() {
    auto expressions = loadInput();
    part1(expressions);
    part2(expressions);
    return 0;
}
