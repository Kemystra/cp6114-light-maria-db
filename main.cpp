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
    Literal,
    Semicolon
};

// The Token 'box'
struct Token {
    TokenType type;
    string value;
};


int main (int argc, char *argv[]) {
    string inputFileName = "fileInput1.mdb";
    ifstream inputFile;

    inputFile.open(inputFileName);

    // These if statements are called 'guard clauses'
    // Instead of nesting code inside 'else', we can detect errors and exit/return immediately
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

        // We will be returning by reference after lexer, so we gonna store it here
        vector<Token> statement;
    }

    

    return 0;
}
