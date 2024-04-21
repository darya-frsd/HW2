#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

struct OpeningBracket {
};
struct ClosingBracket {
};
struct Number {
    int value;
};
struct UnknownToken {
    std::string value;
};
struct MinToken {
};
struct AbsToken {
};
struct MaxToken {
};
struct SqrToken {
};
struct Plus {
};
struct Minus {
};
struct Multiply {
};
struct Modulo {
};
struct Divide {
};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, MaxToken, SqrToken, Plus, Minus, Multiply, Divide, Modulo>;

std::unordered_map<char, Token> InitializeSymbol2Token() {
    std::unordered_map<char, Token> symbol_map;
    symbol_map.emplace('+', Plus{});
    symbol_map.emplace('-', Minus{});
    symbol_map.emplace('*', Multiply{});
    symbol_map.emplace('/', Divide{});
    symbol_map.emplace('%', Modulo{});
    return symbol_map;
}

const std::unordered_map<char, Token> kSymbol2Token = InitializeSymbol2Token();

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string &input, size_t &pos) {
    int value = 0;
    char symbol = input[pos];
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            break;
        }
        symbol = input[++pos];
    }
    return Number{value};
}

Token ParseName(const std::string &input, size_t &pos) {
    std::string name;
    char symbol = input[pos]; 
    while (std::isalpha(symbol)) {
        name.push_back(symbol);
        if (pos == input.size() - 1) {
            break;
        }
        symbol = input[++pos];
    }

    std::unordered_map<std::string, Token> name2Token = {
            {"min", MinToken{}},
            {"abs", AbsToken{}},
            {"max", MaxToken{}},
            {"sqr", SqrToken{}},
            {"+",   Plus{}},
            {"-",   Minus{}},
            {"*",   Multiply{}},
            {"/",   Divide{}},
            {"%",   Modulo{}}
    };

    auto it = name2Token.find(name);
    if (it != name2Token.end()) {
        return it->second;
    } else {
        return UnknownToken{name};
    }
}

std::vector<Token> Tokenize(const std::string &input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size) {
        const char symbol = input[pos];
        if (std::isspace(symbol)) {
            ++pos;
        } else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        } else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        } else {
            auto it = kSymbol2Token.find(symbol);
            if (it != kSymbol2Token.end()) {
                tokens.emplace_back(it->second);
            }
            ++pos;
        }
    }

    return tokens;
}


/*
// Код написан для проверки работоспособности программы
void print_res(const std::vector<Token> &tokens) {
    for (const auto &token: tokens) {
        if (std::holds_alternative<Number>(token)) {
            std::cout << "Number: " << std::get<Number>(token).value << std::endl;
        } else if (std::holds_alternative<UnknownToken>(token)) {
            std::cout << "Unknown token: " << std::get<UnknownToken>(token).value << std::endl;
        } else if (std::holds_alternative<MinToken>(token)) {
            std::cout << "MinToken" << std::endl;
        } else if (std::holds_alternative<MaxToken>(token)) {
            std::cout << "MaxToken" << std::endl;
        } else if (std::holds_alternative<SqrToken>(token)) {
            std::cout << "SqrToken" << std::endl;
        } else if (std::holds_alternative<AbsToken>(token)) {
            std::cout << "AbsToken" << std::endl;
        } else if (std::holds_alternative<OpeningBracket>(token)) {
            std::cout << "Opening Bracket" << std::endl;
        } else if (std::holds_alternative<ClosingBracket>(token)) {
            std::cout << "Closing Bracket" << std::endl;
        } else if (std::holds_alternative<Plus>(token)) {
            std::cout << "Plus" << std::endl;
        } else if (std::holds_alternative<Minus>(token)) {
            std::cout << "Minus" << std::endl;
        } else if (std::holds_alternative<Multiply>(token)) {
            std::cout << "Multiply" << std::endl;
        } else if (std::holds_alternative<Divide>(token)) {
            std::cout << "Divide" << std::endl;
        } else if (std::holds_alternative<Modulo>(token)) {
            std::cout << "Modulo" << std::endl;
        } else {
            std::cout << "Other token" << std::endl;
        }
    }
}

int main() {
    std::string input = "min(3, 5) + abs(-7) + max(10, 20) - sqr(4) ";
    auto tokens = Tokenize(input);
    print_res(tokens);
    return 0;
}
*/
