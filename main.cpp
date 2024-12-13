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
    "FROM"
};

// -- Function Prototype --
// Technically, you don't need to put the parameter name here
// But it's good for documentation
void stringToTokens(string rawStatement, vector<Token> &tokenList);
Token parseWord(string &rawStatement, int &char_pos);
void printToken(Token token);
string stringifyTokenType(TokenType tokenType);

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
        // parse the rest of them as words
        // Note that the first letter of a 'word' cannot be a number
        // so we use isalpha() here instead of alnum() (which checks for letters and numbers)
        if (isalpha(current_char) || current_char == '_' || current_char == '.') {
            token = parseWord(rawStatement, char_pos);

            // We don't increment char_pos or skip the loop here
            // because parseWord() will always stop AFTER the last word's character
            // so char_pos is already pointing to the next character
        }

        // If it's whitespace, skip it
        if (isspace(current_char)) {
            char_pos++;
            continue;
        }

        // Check for special characters
        switch (current_char) {
            case '(':
                token.type = TokenType::OpenBracket;
                token.value = '(';
                break;
            case ')':
                token.type = TokenType::CloseBracket;
                token.value = ')';
                break;
            case ';':
                token.type = TokenType::Semicolon;
                token.value = ';';
                break;
            case ',':
                token.type = TokenType::Comma;
                token.value = ',';
                break;
            case '*':
                token.type = TokenType::Wildcard;
                token.value = '*';
                break;
            default:
                cout << "Unknown token. Exiting...\n";
                exit(1);
        }

        tokenList.push_back(token);

        char_pos++;
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
        current_char = rawStatement[char_pos];

        if (current_char == '.')
            hasDot = true;

        word += current_char;

        char_pos++;
    }

    token.value = word;

    // More guard clauses

    // Check if it's a filename
    if (hasDot) {
        token.type = TokenType::Filename;
        return token;
    }

    // Check if it's a keyword
    for (int i = 0; i < size(KEYWORD_LIST); i++) {
        if (word == KEYWORD_LIST[i]) {
            token.type = TokenType::Keyword;
            return token;
        }
    }

    // If nothing else, then it must be an identifier
    token.type = TokenType::Identifier;
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
