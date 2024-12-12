#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// -- Macro definitions --
// Yes technically we won't learn about this
// This will turn an identifier into a quoted string
// see printToken() for example usage
#define getName(a) #a

using namespace std;


// Think of 'enum' as choices.
// A variable with a type TokenType can only have the value Identifier or Literal or... etc.
enum TokenType {
    Keyword,
    Identifier,
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

        // If it's whitespace, skip it
        if (isspace(rawStatement[char_pos])) {
            char_pos++;
            continue;
        }

        // If the character is alphanumeric (letters or numbers)
        // parse it as words
        // MUST BE BEFORE CHECKING SPECIAL CHARACTERS
        // see parseWord() for more info
        if (isalnum(rawStatement[char_pos])) {
            token = parseWord(rawStatement, char_pos);
        }

        // Check for special characters
        switch (rawStatement[char_pos]) {
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

    // While the current character is still alphanumeric, keep adding to word
    // Otherwise break the loop
    // The nice thing is that the char_pos variable will then point to the next character after the current word
    // So they can be matched for special characters in stringToToken()
    while (isalnum(rawStatement[char_pos])) {
        char current_char = rawStatement[char_pos];
        word += current_char;

        char_pos++;
    }

    token.value = word;

    for (int i = 0; i < size(KEYWORD_LIST); i++) {
        // Another guard clause
        // Return immediately if a match is found
        if (word == KEYWORD_LIST[i]) {
            token.type = TokenType::Keyword;
            return token;
        }
    }

    token.type = TokenType::Identifier;
    return token;
}


string stringifyTokenType(TokenType tokenType) {
    string str;
    switch (tokenType) {
        case TokenType::Keyword:
            str = "Keyword";
            break;
        case TokenType::Identifier:
            str = "Identifier";
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
