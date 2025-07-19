#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Parser {
    string expr;
    size_t pos;

    char peek() {
        return pos < expr.size() ? expr[pos] : '\0';
    }

    char get() {
        return pos < expr.size() ? expr[pos++] : '\0';
    }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

    // Parses numbers
    int number() {
        skipWhitespace();
        int result = 0;
        if (!isdigit(peek())) {
            throw runtime_error("Expected number at position " + to_string(pos));
        }
        while (isdigit(peek())) {
            result = result * 10 + (get() - '0');
        }
        return result;
    }

    // Parses factors: number or parenthesized expression
    int factor() {
        skipWhitespace();
        if (peek() == '(') {
            get(); // consume '('
            int result = expression();
            if (peek() == ')') {
                get(); // consume ')'
            } else {
                throw runtime_error("Expected ')' at position " + to_string(pos));
            }
            return result;
        } else {
            return number();
        }
    }

    // Parses terms: factors connected by * or /
    int term() {
        int result = factor();
        while (true) {
            skipWhitespace();
            if (peek() == '*') {
                get();
                result *= factor();
            } else if (peek() == '/') {
                get();
                int divisor = factor();
                if (divisor == 0) {
                    throw runtime_error("Division by zero");
                }
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    // Parses full expression: terms connected by + or -
    int expression() {
        int result = term();
        while (true) {
            skipWhitespace();
            if (peek() == '+') {
                get();
                result += term();
            } else if (peek() == '-') {
                get();
                result -= term();
            } else {
                break;
            }
        }
        return result;
    }

public:
    int parse(const string& s) {
        expr = s;
        pos = 0;
        int result = expression();
        skipWhitespace();
        if (pos != expr.length()) {
            throw runtime_error("Unexpected character at position " + to_string(pos));
        }
        return result;
    }
};

int main() {
    Parser parser;
    string input;

    cout << "Simple Arithmetic Compiler (type 'exit' to quit)" << endl;
    while (true) {
        cout << "\n> ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            int result = parser.parse(input);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
