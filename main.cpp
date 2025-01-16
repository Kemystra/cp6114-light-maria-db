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
#include <map>

using namespace std;

// This is called a type alias
// We can use Row as if we are using vector<string>
// It's just to make part of the code more readable
typedef vector<string> Row;

// -- Constant Definitions --
const string ACTION_KEYWORDS[] = {
    "CREATE",
    "DATABASES",
    "TABLE",
    "TABLES",
    "INSERT",
    "INTO",
    "VALUES",
    "SELECT",
    "FROM",
    "UPDATE",
    "SET",
    "WHERE",
    "DELETE",
    "FROM",
};

const string TYPE_KEYWORDS[] = {
    "TEXT",
    "INT"
};

const string FUNCTION_KEYWORDS[] = {
    "COUNT"
};

// Even though each string has a different sizes
// The class string only stores the POINTER to the actual string
// so the size is constant
const int ACTION_KEYWORDS_SIZE = sizeof(ACTION_KEYWORDS) / sizeof(ACTION_KEYWORDS[0]);
const int TYPE_KEYWORDS_SIZE = sizeof(TYPE_KEYWORDS) / sizeof(TYPE_KEYWORDS[0]);
const int FUNCTION_KEYWORDS_SIZE = sizeof(FUNCTION_KEYWORDS) / sizeof(FUNCTION_KEYWORDS[0]);

// Open and close parentheses, wildcard, comma, and semicolon
const char SPECIAL_CHARACTERS[] = {
    '(', ')', '*', ',', ';','='
};

const int SPECIAL_CHARACTERS_SIZE = sizeof(SPECIAL_CHARACTERS) / sizeof(SPECIAL_CHARACTERS[0]);

// Used in string trimming later
const std::string WHITESPACE = " \n\r\t\f\v";

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
};

enum LogicalOperator {
    Equal
};

// This is std::map
// Literally, it "maps" from one value to another
// Here, we map a string (the "=" symbol) to the LogicalOperator::Equal
const map<string, LogicalOperator> strToLogicalOps = {
    { "=", LogicalOperator::Equal }
};

struct ValueComparator {
    string valueStr;
    LogicalOperator op;
    string columnName;
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

        vector<FieldData> getAllFieldData() const{
            return fieldDataList;
        }

        int getFieldIndex(const string& fieldName) const {
            for (int i = 0; i < fieldDataList.size(); i++) {
                if(fieldDataList[i].name == fieldName)
                    return i;
            }

            // Indicate failure
            return -1;
        }

        void addColumn(FieldData fd) {
            // Add a new column, and put its index into field
            fieldDataList.push_back(fd);
        }

        vector<Row> selectRows(string column) {
            vector<Row> tableData;
            if(column == "*"){
                return rowList;
            } else {
                int fieldDataIndex = getFieldIndex(column);
                for(int i=0; i<rowList.size();i++){
                    tableData[0].push_back(rowList[i][fieldDataIndex]);
                }
                return tableData;
            }
        }

        void insertRows(Row row) {
            rowList.push_back(row);
        }

        // Return indices of rows that match the condition
        vector<int> findRow(ValueComparator comp) const {
            vector<int> result;

            // Loop through each row, and check if it's
            for (int i = 0; i < rowList.size(); i++) {
                int columnIndex = getFieldIndex(comp.columnName);
                bool isMatch = false;

                switch (comp.op) {
                    case LogicalOperator::Equal:
                        isMatch = rowList[i][columnIndex] == comp.valueStr;
                }

                if (isMatch)
                    result.push_back(i);
            }

            return result;
        }

        void updateRows(const string& columnName, const string& newValue, ValueComparator comp) {
            vector<int> indexList = findRow(comp); // find row that match condition

            int colIndex = getFieldIndex(columnName);

            // if (colIndex == -1) {
            //     cout << "Error: Column " << columnName << " not found." << endl; 
            //     return 1; 
            // }

            for (int i : indexList) {
                rowList[i][colIndex] = newValue;
            }
        }

        void deleteRows(ValueComparator comp) {
            vector<int> row_indices = findRow(comp);
            for (int i : row_indices) {
                this->rowList.erase(rowList.begin() + i);
            }
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

string trim(const string &s);
string extractStr(string);

void createTable(vector<string> tokens, Table& table);
void printDatabases();
void insertIntoTable(vector<string> tokens, Table& table);
void updateTable(vector<string> tokens, Table& table);
string selectFromTable(vector<string> tokens, Table& table);
void updateTable();
void deleteFromTable(const vector<string>& tokens, Table& table);

string formatCSV(vector<string> header, vector<vector<string>> tableData);

ValueComparator whereKeywordParser(const vector<string>& tokens, int& index);

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

        if (statementTokens[0] == "CREATE")
        {
            if (statementTokens[1] == "TABLE") 
                createTable(statementTokens, table);
        }
        else if (statementTokens[0] == "TABLES")
            cout << table.getName() << endl;
        else if (statementTokens[0] == "DATABASES")
            printDatabases();
        else if (statementTokens[0] == "INSERT")
            insertIntoTable(statementTokens, table);
        // else if (statementToken[i] == "VALUES")
        else if (statementTokens[0] == "SELECT")
            selectFromTable(statementTokens, table);
        else if (statementTokens[0] == "UPDATE")
            updateTable();
        else if (statementTokens[0] == "DELETE")
            deleteFromTable(statementTokens, table);

        cout << '\n';
    }

    return 0;
}

// Trimming functions
string ltrim(const string &s)
{
    // Literally, find the position of the first character that is not in WHITESPACE
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}
 
string rtrim(const string &s)
{
    // Kinda like above but searching from behind
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
 
string trim(const string &s) {
    return rtrim(ltrim(s));
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

string extractStr(string token) {

    if (token[0] == '\'') {
        return (token.substr(1, token.size()-2));
    }

    else
        return token;
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

    index += 2;

    vector<string> values;

    while (tokens[index] != ")") {
        string currToken = tokens[index];

        if (currToken == ",") {
            index++;
            continue;
        }

        newRow.push_back(extractStr(currToken));
        
        index++;
    }

    table.insertRows(newRow);
}


string selectFromTable(vector<string> tokens, Table& table) {

    bool countOrNot = false;
    string column;

    //check for 2nd last token if it is the name of table
    // Note that the last token is always the semicolon
    if(tokens[tokens.size()-2] != table.getName()){
        cout << "TableError: Table not found." << endl;
        exit(1);
    }

    if(tokens[1] == "COUNT"){
        countOrNot = true;
        //take all inside brackets
        column = tokens[3];
    } else {
        column = tokens[1];
    }

    //take data from table
    string result = "";
    vector<Row> tableData;
    vector<string> header;
    if(column == "*"){
        for(FieldData fd : table.getAllFieldData()){
            header.push_back(fd.name);
        }
    } else {
        header.push_back(column);
    }
    

    tableData = table.selectRows(column);

    if(countOrNot){
        result = to_string(tableData.size());
    } else {
        result = formatCSV(header, tableData);
    }
    return result;

}

void updateTable(vector<string> tokens, Table& table ) {

    index = 3
    
    string colUpdate = tokens[index];

    index += 2;
    currToken = tokens[index]; 
    string newValue = extractStr(curr_token); 

    index ++;

    table.updateRows(colUpdate, newValue, whereKeywordParser(tokens, index));
    
}

void deleteFromTable(const vector<string>& tokens, Table& table) {
    // Since we don't have syntax error checking right now
    // we can skip directly to WHERE keyword
    int index = 3;
    ValueComparator comp = whereKeywordParser(tokens, index);

    table.deleteRows(comp);
}

ValueComparator whereKeywordParser(const vector<string>& tokens, int& index) {
    // Sanity check
    if (tokens[index] != "WHERE")
        exit(1);

    ValueComparator comp;

    index++;
    comp.columnName = tokens[index];

    index++;
    comp.op = strToLogicalOps.at(tokens[index]);

    index++;
    comp.valueStr = extractStr(tokens[index]);
    return comp;
}


string formatCSV(vector<string> header, vector<vector<string>> tableData){
    string csv="";

    //header
    for(int i = 0; i < header.size();i++){
        csv += header[i] + ",";
    }
    csv.pop_back();
    csv += '\n';

    //tableData
    for(int i = 0; i < tableData.size(); i++){
        for(int j = 0; j < tableData[i].size(); j++){
            csv += tableData[i][j] + ",";
        }
        csv.pop_back();
        csv += '\n';
    }

    return csv;

}
