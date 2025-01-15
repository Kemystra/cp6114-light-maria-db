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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// This is called a type alias
// We can use Row as if we are using vector<string>
// It's just to make part of the code more readable
typedef vector<string> Row;

// -- Constant Definitions --
const string KEYWORDS[] = {
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
    "TEXT",
    "UPDATE",
    "SET",
    "WHERE",
    "DELETE",
    "FROM",
    "COUNT"
};

// Even though each string has a different sizes
// The class string only stores the POINTER to the actual string
// so the size is constant
const int KEYWORDS_SIZE = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

// Open and close parentheses, wildcard, comma, and semicolon
const char SPECIAL_CHARACTERS[] = {
    '(', ')', '*', ',', ';'
};

const int SPECIAL_CHARACTERS_SIZE = sizeof(SPECIAL_CHARACTERS) / sizeof(SPECIAL_CHARACTERS[0]);

// If you want to define empty values (i.e: NULL in SQL)
// set it to this string
// '\0' is escape sequence for the NULL character in the ASCII table (number 0)
const string NULL_STR = "\0\0\0";

enum FieldDataType {
    INT, TEXT
};

struct FieldData {
    string name;
    FieldDataType dataType;
    int columnIndex;
};

// Table class definition
// The actual database
class Table {
    private:
        // Anything under private can only be accessed by the class itself
        vector<FieldData> fieldDataList;
        // We can access the data with one data type only
        // and convert as needed.
        vector<Row> rowList;
    public:
        // Anything under public can be accessed outside of the class
        string name;

        // This is a constructor
        // This is what allows you to create a class
        // It can also take parameters like functions do
        Table(string s) {
            name = s;

            // We just need to create empty fields here
            fieldDataList = vector<FieldData>();
            rowList = vector<Row>();
        }

        // These are methods; functions that automatically have access to the class Table
        void setName(string name) {
            // You can also do:
            // name = name;
            // but that's confusing
            //
            // This line set the variable name of the created Table class
            this->name = name;
        }

        string getName() const {
            return name;
        }

        FieldData getFieldData(string &fieldName) const {
            // This is a short way of doing loops
            for (FieldData fd : this->fieldDataList) {
                if (fd.name == fieldName)
                    return fd;
            }

            // TODO: replace this with more proper error handling
            cout << "TableError: No field with the name " << fieldName;
            exit(1);
        }

        void addColumn(FieldData fd) {
            // Add a new column, and put its index into field
            fieldDataList.push_back(fd);

            // Temporary code to output fieldDataList
            // for (int i = 0; i < fieldDataList.size(); i++) {
            // cout << "Field #" << i+1 << '\n';
            // FieldData field = fieldDataList[i];
            // cout << "Name: " << field.name << '\n';
            // cout << "Data Type: " << field.dataType << '\n';
            // cout << "Column Index: " << field.columnIndex << '\n';
            // cout << '\n';
            // }
        }

        void selectRows() {}
        void insertRows(Row row) {
            rowList.push_back(row);
        }
        void deleteRows() {}
        void updateRows() {}

        string printTable(string columnsName) {
            string print = "";
            //version 1 : csv format
            if(columnsName == "*"){
                // print the table header
                for(int i=0; i<fieldDataList.size(); i++){
                    print += fieldDataList[i].name;
                    if(i != fieldDataList.size()-1){
                        print += ',';
                    }
                }
                print += '\n';
                //print the table data
                int membersTotal = dataInt[0].size();
                for(int i=0; i < membersTotal ; i++){
                    for(int j=0; j<fieldDataList.size(); j++){
                        int columnIndex = fieldDataList[j].columnIndex;
                        if(fieldDataList[j].dataType == FieldDataType::INT){
                            if (dataInt[columnIndex][i] == -99){
                                print += "NULL";
                            } else
                                print += to_string(dataInt[columnIndex][i]);
                        } else {
                            print += dataStr[columnIndex][i];
                        }
                        if(j != fieldDataList.size()-1){
                            print += ',';
                        }
                    }
                    if( i!= membersTotal-1)
                        print += '\n';
                }
            } else {
                FieldData fd;
                fd.columnIndex = -1;
                for(int i=0; i<fieldDataList.size();i++){
                    if(columnsName == fieldDataList[i].name){
                        fd.columnIndex = fieldDataList[i].columnIndex;
                        fd.dataType = fieldDataList[i].dataType;
                        break;
                    }
                }
                print += columnsName + '\n';
                if(fd.dataType == FieldDataType::INT){
                    for(int i=0; i<dataInt[fd.columnIndex].size();i++){
                        if(dataInt[fd.columnIndex][i] != -99){
                            print += to_string(dataInt[fd.columnIndex][i])+'\n';
                        }
                    }
                    print.pop_back();
                } else {
                    for(int i=0; i<dataStr[fd.columnIndex].size();i++){
                        if(dataStr[fd.columnIndex][i] != "NULL"){
                            print += dataStr[fd.columnIndex][i]+'\n';
                        }
                    }
                    print.pop_back();
                }
            }

            //version 2 : table format

            // calculate the highest count
            vector<int> highestCount;
            for(int i=0; i<fieldDataList.size();i++){
                highestCount.push_back(fieldDataList[i].name.size());
            }
            for(int i=0; i<fieldDataList.size();i++){
                int columnIndex = fieldDataList[i].columnIndex;
                if(fieldDataList[i].dataType == FieldDataType::INT){
                    for(int j=0; j<dataInt[columnIndex].size(); j++){
                        if(dataInt[columnIndex][j] != -99){
                            int count = to_string(dataInt[columnIndex][j]).size();
                            if(count > highestCount[i]){
                                highestCount[i] = count;
                            }
                        }
                    }
                } else {
                    for(int j=0; j<dataStr[columnIndex].size(); j++){
                        if(dataStr[columnIndex][j] != "NULL"){
                            int count = dataStr[columnIndex][j].size();
                            if(count > highestCount[i]){
                                highestCount[i] = count;
                            }
                        }
                    }
                }
            }
            //print the table header
            if(columnsName == "*"){
                print += '\n';
                for(int i=0;i<highestCount.size();i++){
                    print += '+';
                    for(int j=0;j<highestCount[i] +2;j++){
                        print += '-';
                    }
                }
                print += "+\n";
                for(int i=0;i<fieldDataList.size();i++){
                    print += "| " + fieldDataList[i].name;
                    for(int j=0;j<highestCount[i]-fieldDataList[i].name.size();j++){
                        print += ' ';
                    }
                    print += " ";
                }
                print += "|\n";
                for(int i=0;i<highestCount.size();i++){
                    print +='+';
                    for(int j=0;j<highestCount[i]+2;j++){
                        print += '-';
                    }
                }
                print += "+\n";


                //print table data
                int membersTotal = dataInt[0].size();
                for(int i=0; i < membersTotal ; i++){
                    for(int j=0; j<fieldDataList.size(); j++){
                        int columnIndex = fieldDataList[j].columnIndex;
                        if(fieldDataList[j].dataType == FieldDataType::INT){
                            if (dataInt[columnIndex][i] == -99){
                                print+= "| NULL";
                                for(int k=0;k<highestCount[j]-4;k++){
                                    print += ' ';
                                }
                            } else
                                print += "| ";
                                for(int k=0;k<highestCount[j]-to_string(dataInt[columnIndex][i]).size();k++){
                                    print += ' ';
                                }
                                print += to_string(dataInt[columnIndex][i]) ;
                        } else {
                            print += "| " + dataStr[columnIndex][i];
                            for(int k=0;k<highestCount[j]-dataStr[columnIndex][i].size();k++){
                                print += ' ';
                            }
                        }
                        print += " ";
                    }
                    print+= "|\n";
                }
                for(int i=0;i<highestCount.size();i++){
                    print += '+';
                    for(int j=0;j<highestCount[i]+2;j++){
                        print += '-';
                    }
                }
                print += "+";
            } else {
                int highestCountIndex = -1;
                int columnIndex = -1;
                for(int i = 0; i<fieldDataList.size();i++){
                    if(columnsName == fieldDataList[i].name){
                        highestCountIndex = i;
                        columnIndex = fieldDataList[i].columnIndex;
                        break;
                    }
                }
                print += '\n';
                // print header
                print += '+';
                for(int i=0; i<highestCount[highestCountIndex]+2 ;i++){
                    print += '-';
                }
                print += "+\n";
                print += "| " + columnsName;
                for(int i=0;i<highestCount[highestCountIndex] - columnsName.size();i++){
                    print += ' ';
                }
                print += " |\n";
                print += '+';
                for(int i=0; i<highestCount[highestCountIndex]+2 ;i++){
                    print += '-';
                }
                print += "+\n";
                // print data
                if(fieldDataList[highestCountIndex].dataType == FieldDataType::INT){
                    for(int i=0; i<dataInt[columnIndex].size();i++){
                        if(dataInt[columnIndex][i] != -99){
                            print += "| ";
                            for(int j=0; j<highestCount[highestCountIndex]-to_string(dataInt[columnIndex][i]).size();j++){
                                print += ' ';
                            }
                            print += to_string(dataInt[columnIndex][i]) + " |\n";
                        }
                    }
                } else {
                    for(int i=0; i<dataStr[columnIndex].size();i++){
                        if(dataStr[columnIndex][i] != "NULL"){
                            print += "| " + dataStr[columnIndex][i];
                            for(int j=0; j<highestCount[highestCountIndex]-dataStr[columnIndex][i].size();j++){
                                print += ' ';
                            }
                            print += " |\n";
                        }
                    }
                }
                print += '+';
                for(int i=0; i<highestCount[highestCountIndex]+2 ;i++){
                    print += '-';
                }
                print += "+";
            }

            return print;
        }

        int count(string columnsName){
            int count = 0;
            if(columnsName == "*"){
                count = dataInt[0].size();
            } else {
                for(int i=0; i<fieldDataList.size(); i++){
                    if(columnsName == fieldDataList[i].name){
                        int columnIndex = fieldDataList[i].columnIndex;
                        if(fieldDataList[i].dataType == FieldDataType::INT){
                            for(int j=0; j<dataInt[columnIndex].size(); j++){
                                if(dataInt[columnIndex][j] != -99){
                                    count++;
                                }
                            }
                        } else {
                            for(int j=0; j<dataStr[columnIndex].size(); j++){
                                if(dataStr[columnIndex][j] != "NULL"){
                                    count++;
                                }
                            }
                        }
                        break;
                    }
                }
            }
            return count;
        }
};

// -- Function Prototype --
// Technically, you don't need to put the parameter name here
// But it's good for documentation
void stringToTokens(string rawStatement, vector<string> &tokenList);
string parseWord(string &rawStatement, int &char_pos);
string parseNumberLiteral(string &rawStatement, int &char_pos);
string parseStringLiteral(string &rawStatement, int &char_pos);
string parseSpecialCharacters(string &rawStatement, int &char_pos);

void createTable(vector<string> tokens, Table& table);
void printDatabases();
void insertIntoTable(vector<string> tokens, Table& table);
void selectFromTable();
void updateTable();
void deleteFromTable();
void countFromTable();

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
    Table table("");
    while (getline(inputFile, rawStatement, ';')) {
        // We need the semicolon to mark end of statement, so I readded it
        rawStatement += ';';
        cout << rawStatement << '\n';

        vector<string> statementTokens;
        stringToTokens(rawStatement, statementTokens);

        for (int i = 0; i < statementTokens.size(); i++) {
            if (statementTokens[0] == "CREATE")
            {
                if (statementTokens[1] == "TABLE") 
                    createTable(statementTokens, table);
            }
            else if (statementTokens[i] == "TABLES")
                cout << table.getName() << endl;
            else if (statementTokens[i] == "DATABASES")
                printDatabases();
            else if (statementTokens[i] == "INSERT")
                insertIntoTable(statementTokens, table);
            // else if (statementToken[i] == "VALUES")
            else if (statementTokens[i] == "SELECT")
                selectFromTable();
            else if (statementTokens[i] == "UPDATE")
                updateTable();
            else if (statementTokens[i] == "DELETE")
                deleteFromTable();
            else if (statementTokens[i] == "COUNT")
                countFromTable();
                
        }
        cout << '\n';

        // Empty table to store value
        // Table table("");
        // if (statementTokens[1] == "TABLE") {
        //     createTable(statementTokens, table);
        // }
    }


    return 0;
}


// -- The lexer --
void stringToTokens(string rawStatement, vector<string> &tokenList) {
    int char_pos = 0;
    while (char_pos < rawStatement.length()) {
        string token = "";
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
string parseWord(string &rawStatement, int &char_pos) {
    string token = "";
    char current_char = rawStatement[char_pos];

    // While the current character is still valid, keep adding to word
    // Otherwise break the loop
    // Valid characters here are letters, numbers, underscores, and dot
    // Note that numbers can be used here if it's not the first character
    while (isalnum(current_char) || current_char == '_' || current_char == '.') {
        token += current_char;

        char_pos++;
        current_char = rawStatement[char_pos];
    }

    return token;
}


string parseNumberLiteral(string &rawStatement, int &char_pos) {
    string value = "";
    char current_digit = rawStatement[char_pos];

    // Notice that it's the same format as parseWord() above
    while (isdigit(current_digit)) {
        value += current_digit;

        char_pos++;
        current_digit = rawStatement[char_pos];
    }

    return value;
}


string parseStringLiteral(string &rawStatement, int &char_pos) {
    string value = "";
    char current_char = rawStatement[char_pos];

    // current_char right now is the quotation mark
    // since it might be single or double quote, we will be storing it for later
    char quotation_mark = current_char;

    // we manually add this. see below for why
    value += quotation_mark;

    // Because the char_pos is currently pointing to the quotation mark,
    // we have to point it to the next character
    // or else the while loop will not run
    // and it will go back to stringToToken() but still pointing to the quotation mark
    // creating an endless loop
    char_pos++;
    current_char = rawStatement[char_pos];

    // We use '&&' operator so that either one can trigger to terminate the loop
    while (current_char != '\'' && current_char != '"') {
        value += current_char;

        char_pos++;
        current_char = rawStatement[char_pos];
    }

    // Again, skip the current quotation mark to avoid endless loop
    char_pos++;

    // Since we skip the closing of quotation mark, we have to re-add it manually
    // which is why we store it BEFORE parsing
    value += quotation_mark;

    return value;
}


string parseSpecialCharacters(string &rawStatement, int &char_pos) {
    char current_char = rawStatement[char_pos];

    // I HAVE TO PUT THIS CUZ THERE'S NO GUD WAY TO CONVERT CHAR TO STRING
    // IT'S SOO GODDAMN UGLY
    string s = "";

    for (int i = 0; i < SPECIAL_CHARACTERS_SIZE; i++) {
        if (current_char == SPECIAL_CHARACTERS[i]) {
            // Point to the next character
            char_pos++;
            return s + current_char;
        }
    }

    cout << "Unknown token. Exiting...\n";
    exit(1);
}


// -- Processing functions --
void createTable(vector<string> tokens, Table& table) {
    // The switch statement in main should call this function
    // only if the statement start with "CREATE TABLE"
    // so we start at the 3rd token (index 2)
    int index = 2;

    // Main advantage of the former token system
    // is that checking if the tokens are valid will be much easier
    // For now we won't care too much on error handling
    // focus on getting it to work
    string tableName = tokens[index];
    table.setName(tableName);

    // Move to the next token but skip the opening bracket
    index += 2;

    // Parse the field name and its type
    // Stop when close bracket is found
    while (true) {
        string fieldName = tokens[index];

        index++;
        string fieldDataTypeStr = tokens[index];

        // This way of converting string to its enum type is kinda janky
        // Tho this is the easiest way
        FieldDataType fieldDataType;
        if (fieldDataTypeStr == "INT")
            fieldDataType = FieldDataType::INT;
        else if (fieldDataTypeStr == "TEXT")
            fieldDataType = FieldDataType::TEXT;
        else {
            cout << "Unknown field data type \'" << fieldDataTypeStr << "\'. Exiting...\n";
            exit(1);
        }

        FieldData field = FieldData();
        field.name = fieldName;
        field.dataType = fieldDataType;

        table.addColumn(field);

        // Skip the comma
        index++;

        // break if closing bracket is found
        if (tokens[index] == ")")
            break;

        // If not, point to the next token, i.e, the next field name
        index++;
    }
}

void printDatabases() {

}

void insertIntoTable(vector<string> tokens, Table& table) {
    int index = 2;

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "VALUES"){
            index = i;
            break;
        }
    }

    Row newRow;

    // if (index == -1) {
    //     cout << "Error : No VALUES keywords found." << endl;
    //     return;
    // }

    index += 2;

    vector<string> values;
    int valEnd = tokens.size() - 1;

    while (tokens[index] != ")") {

        if (tokens[index] == "," || tokens[index] != "'") {
            index++;
            continue;
        }

        else
            newRow.push_back(values[index]);
        
        index++;
    }

    table.insertRows(newRow);
}


void selectFromTable() {

}

void updateTable() {

}

void deleteFromTable() {

}

void countFromTable() {

}
