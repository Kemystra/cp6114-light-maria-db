// ********************************************************************************

// Program : main.cpp
// Course : CCP6114 Programming Fundamentals
// Lecture Class : TC6L
// Tutorial Section : T11L
// Trimester : 2430 
// Member_1 : 242UC240JF | Izzminhal Akmal Bin Norhisyam | izzminhal.akmal.norhisyam1@student.mmu.edu.my | +60 11-4034 8650
// Member_1 : 243UC247CJ | Aimi Binti Mohd Faizal | aimi.mohd.faizal@student.mmu.edu.my | +60 13-907 1648
// Member_1 : 243UC247DP | Muhammad Afiq Bin Shahrin | muhammad.afiq.shahrin@student.mmu.edu.my | +60 18-977 0080
// Member_1 : 242UC240JF | Low Eng Sern | low.eng.sern@student.mmu.edu.my | +60 11-2150 0218

// ********************************************************************************

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


// Think of 'enum' as choices.
// A variable with a type TokenType can only have the value Identifier or Literal or... etc.
enum TokenType {
    Keyword,
    Identifier,
    Filename,
    Literal,
    Semicolon,
    OpenBracket,
    CloseBracket,
    Comma,
    Wildcard
};

// The Token 'box'
struct Token {
    TokenType type;
    string value;
};

const string KEYWORD_LIST[] = {
    "CREATE",
    "DATABASES",
    "TABLE",
    "TABLES",
    "INSERT",
    "INTO",
    "VALUES",
    "SELECT",
    "FROM",
    "INT",
    "TEXT"
};

// Even though each string has a different sizes
// The class string only stores the POINTER to the actual string
// so the size is constant
const int KEYWORD_LIST_SIZE = sizeof(KEYWORD_LIST) / sizeof(KEYWORD_LIST[0]);

// -- Function Prototype --
// Technically, you don't need to put the parameter name here
// But it's good for documentation
void stringToTokens(string rawStatement, vector<Token> &tokenList);
Token parseWord(string &rawStatement, int &char_pos);
Token parseNumberLiteral(string &rawStatement, int &char_pos);
Token parseStringLiteral(string &rawStatement, int &char_pos);
Token parseSpecialCharacters(string &rawStatement, int &char_pos);

string stringifyTokenType(TokenType tokenType);
void printToken(Token token);

int main (int argc, char *argv[]) {
    string inputFileName = "fileInput1.mdb";
    ifstream inputFile;

    inputFile.open(inputFileName);

    // These if statements are called 'guard clauses'
    // Instead of nesting code inside multiple 'if', we can detect errors and exit/return immediately
    // This is a very common programming pattern, and it helps to make code more readable

    if (!inputFile) {
        // We usually use cout, but for errors it's better to use cerr
        // I'll talk about why later, for now I will use cout
        cout << "Unable to open input file " << inputFileName << '\n';

        // Indicate exit with failure
        exit(1);
    }

    // Check if file is empty
    // Note that EOF is short for End Of File
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        cout << "File " << inputFileName << " is empty\n";
        exit(1);
    }

    // Split each statement with the semicolon, and store into rawStatement
    // One of the consequence of this code is that the newline character is not removed
    // We will handle it just like other whitespace, by ignoring it
    string rawStatement;
    while (getline(inputFile, rawStatement, ';')) {
        // We need the semicolon to mark end of statement, so I readded it
        rawStatement += ';';
        cout << rawStatement << '\n';

        vector<Token> statementTokens;
        stringToTokens(rawStatement, statementTokens);

        for (int i = 0; i < statementTokens.size(); i++) {
            // Temporary code to print out the tokens
            cout << "\n";
            cout << "Token #" << i+1 << "\n";
            cout << "----------------------\n";
            printToken(statementTokens[i]);
        }
    }


    return 0;
}


// -- The lexer --
void stringToTokens(string rawStatement, vector<Token> &tokenList) {
    int char_pos = 0;
    while (char_pos < rawStatement.length()) {
        Token token = Token();
        char current_char = rawStatement[char_pos];

        // If the character is a letter or an underscore or a dot
        // parse the rest of them as 'word'
        // Note that the first letter of a 'word' cannot be a number
        // so we use isalpha() here instead of alnum() (which checks for letters and numbers)
        if (isalpha(current_char) || current_char == '_' || current_char == '.') {
            token = parseWord(rawStatement, char_pos);
        }
        // Check for number literals
        else if (isdigit(current_char)) {
            token = parseNumberLiteral(rawStatement, char_pos);
        }
        // Check for string literals, using quotation marks
        else if (current_char == '\'' || current_char == '"') {
            token = parseStringLiteral(rawStatement, char_pos);
        }
        // If it's whitespace, skip it
        else if (isspace(current_char)) {
            char_pos++;
            continue;
        }
        else {
            token = parseSpecialCharacters(rawStatement, char_pos);
        }

        // All parse*() functions automatically point char_pos to the next character AFTER parsing
        // so we don't increment char_pos here except when checking for whitespace

        tokenList.push_back(token);
    }
}


// Note that we also carried the full statement and the current character position
// This is to ensure that after parsing a word, we can advance to the next character after the word
Token parseWord(string &rawStatement, int &char_pos) {
    string word = "";
    Token token = Token();
    char current_char = rawStatement[char_pos];
    bool hasDot = false;

    // While the current character is still valid, keep adding to word
    // Otherwise break the loop
    // Valid characters here are letters, numbers, underscores, and dot
    // Note that numbers can be used here if it's not the first character
    while (isalnum(current_char) || current_char == '_' || current_char == '.') {
        if (current_char == '.')
            hasDot = true;

        word += current_char;

        char_pos++;
        current_char = rawStatement[char_pos];
    }

    token.value = word;

    // More guard clauses

    // Check if it's a filename
    if (hasDot) {
        token.type = TokenType::Filename;
        return token;
    }

    // Check if it's a keyword
    for (int i = 0; i < KEYWORD_LIST_SIZE; i++) {
        if (word == KEYWORD_LIST[i]) {
            token.type = TokenType::Keyword;
            return token;
        }
    }

    // If nothing else, then it must be an identifier
    token.type = TokenType::Identifier;
    return token;
}


Token parseNumberLiteral(string &rawStatement, int &char_pos) {
    Token token = Token();
    string value = "";
    char current_digit = rawStatement[char_pos];

    // Notice that it's the same format as parseWord() above
    while (isdigit(current_digit)) {
        value += current_digit;

        char_pos++;
        current_digit = rawStatement[char_pos];
    }

    token.type = TokenType::Literal;
    token.value = value;

    return token;
}


Token parseStringLiteral(string &rawStatement, int &char_pos) {
    Token token = Token();
    string value = "";

    // We only want the string values, not the quotation mark
    // So we skip it
    char_pos++;
    char current_char = rawStatement[char_pos];

    // We use '&&' operator so that either one can trigger to terminate the loop
    while (current_char != '\'' && current_char != '"') {
        value += current_char;

        char_pos++;
        current_char = rawStatement[char_pos];
    }

    token.type = TokenType::Literal;
    token.value = value;

    // After parsing, char_pos will point to the character AFTER the string literal
    // Which is the closing quotation mark
    // We will ignore that too
    char_pos++;

    return token;
}


Token parseSpecialCharacters(string &rawStatement, int &char_pos) {
    char current_char = rawStatement[char_pos];
    Token token = Token();

    // Since each character has, well, 1 character
    // We don't need a loop
    switch (current_char) {
        case '(':
            token.type = TokenType::OpenBracket;
            break;
        case ')':
            token.type = TokenType::CloseBracket;
            break;
        case ';':
            token.type = TokenType::Semicolon;
            break;
        case ',':
            token.type = TokenType::Comma;
            break;
        case '*':
            token.type = TokenType::Wildcard;
            break;
        default:
            cout << "Unknown token. Exiting...\n";
            exit(1);
    }

    // Point to the next character
    char_pos++;

    token.value = current_char;
    return token;
}


// There may be a way to do this concisely
// But hey, it works
string stringifyTokenType(TokenType tokenType) {
    string str;
    switch (tokenType) {
        case TokenType::Keyword:
            str = "Keyword";
            break;
        case TokenType::Identifier:
            str = "Identifier";
            break;
        case TokenType::Filename:
            str = "Filename";
            break;
        case TokenType::Literal:
            str = "Literal";
            break;
        case TokenType::Semicolon:
            str = "Semicolon";
            break;
        case TokenType::OpenBracket:
            str = "OpenBracket";
            break;
        case TokenType::CloseBracket:
            str = "CloseBracket";
            break;
        case TokenType::Comma:
            str = "Comma";
            break;
        case TokenType::Wildcard:
            str = "Wildcard";
            break;
    }

    return str;
}


// helper to print tokens to terminal
void printToken(Token token) {
    cout << "Type: " << stringifyTokenType(token.type) << "\n";
    cout << "Value: " << token.value << "\n";
}
