#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <array>
#include <map>
#include <stack>
#include <vector>
using namespace std;

                                                            //////////////////////
                                                            //  Global Variable  //
                                                            //////////////////////


int i;
int state = 0;
int tokenCount = 0;
string tokentype;
string token;
string temp;
string temp2;
string temp3;
bool withinStringLiteral = false;
bool withinCharacterLiteral = false;
bool checkDoubledot = false;
bool flag15 = false;
stack<string> parsingStack;
int totalIdentifiers = 0;
string DataTypee;
int scope1 = 0;
int scope2 = 0;
const int MAX = 100;
array<string, MAX> TokenArray;
int ArrayIndex = 0;
array<string, MAX> TokenArray2;
int ArrayIndex2;
array<string, MAX> IdentifierInSymbolTable;
int IdentifierInSymbolTableArray;
array<int, MAX> StoringIndex;
int StoringIndexArray;
array<int, MAX> VariableAlreadyDeclared;
int VariableAlreadyDeclaredArray;

class SymbolEntry
{
public:
    string identifier;
    int type;
    string Datatype;
    SymbolEntry *next;
    SymbolEntry *prev;

    SymbolEntry(const string &id, const int &IDtype, const string &datatype)
    {
        identifier = id;
        type = IDtype;
        Datatype = datatype;
        next = nullptr;
        prev = nullptr;
    }
};

class SymbolTable
{
public:
    SymbolEntry *head;
    SymbolEntry *tail;

public:
    SymbolTable()
    {
        head = nullptr;
        tail = nullptr;
    }

    void insert(const string &identifier, int &type, string &Datatype)
    {
        SymbolEntry *current = head;

        // Check if the identifier already exists in the symbol table
        while (current != nullptr)
        {
            if (current->identifier == identifier)
            {
                cout << identifier << endl;
                return;
            }
            current = current->next;
        }

        SymbolEntry *newEntry = new SymbolEntry(identifier, type, Datatype);

        if (head == nullptr)
        {
            head = newEntry;
            tail = newEntry;
            totalIdentifiers++;
        }
        else
        {
            tail->next = newEntry;
            newEntry->prev = tail;
            tail = newEntry;
            totalIdentifiers++;
        }
    }

    void display()
    {
        SymbolEntry *current = head;

        cout<<endl;
        cout << "---------------------------------" << endl;
        cout << "|         SYMBOL TABLE          |" << endl;
        cout << "---------------------------------" << endl;
        cout << endl;

        cout << "-------------------------------------------" << endl;
        cout << "| Identifier  |  Scope  | Datatype        |" << endl;
        cout << "-------------------------------------------" << endl;

        while (current != nullptr)
        {
            cout << "| " << setw(11) << current->identifier << "| " << setw(8) << current->type
                 << "| " << setw(17) << current->Datatype << "| " << endl;
            current = current->next;
        }

        cout << "-------------------------------------------" << endl;

        cout << endl;
        cout << "Total Identifiers:"
             << " "
             << "(" << totalIdentifiers << ")" << endl;
        cout << endl;
    }

    // ScopeINC
    void ScopeINC()
    {
        scope1++;
        scope2 = scope1;
    }

    void ScopeDEC()
    {
        scope2--;
    }

    string SemanticChecking(string IdentifierName, string Datatype, int SCOPE)
    {
        SymbolEntry *current = head;

        if (current == nullptr)
        {
            cout << "NULL HAI";
        }
        else
        {
            while (current != nullptr)
            {
                if (current->identifier == IdentifierName && current->Datatype == Datatype && current->type == SCOPE)
                {
                    StoringIndex[StoringIndexArray++] = current->type;
                    return IdentifierName;
                }
                current = current->next;
            }
        }
    }
};

SymbolTable object1;
SymbolTable globalSymbolTable;

                                                    //////////////////////////
                                                    //  Function Declaration //
                                                    //////////////////////////

string Read();
void Translate(const string &content);
void Write();
void Lexical_Analyzer(const string &content);
void Token_Generator_By_Category(string content);
void OperatorDup(char cont);
bool isKeyword(char cont);
bool isSeparator(char cont);
bool isOperator(char cont);
void state0(char cont);
void state1(char cont);
void state2(char cont);
void state3(char cont);
void state4(char cont);
void state5(char cont);
void state6(char cont);
void state7(char cont);
void state8(char cont);
void state9(char cont);
void state10(char cont);
void state11(char cont);
void state12(char cont);
void state13(char cont);
void state14(char cont);
void state15(char cont);
void state16(char cont);
void state17(char cont);
void state18(char cont);
void state19(char cont);
void state20(char cont);
void state21(char cont);
void state22(char cont);
void state23(char cont);
void state24(char cont);
void state25(char cont);
void state26(char cont);
void state27(char cont);
void state28(char cont);

void displayKeywordTokenArray()
{
    cout << endl;
    cout << "Tokens Array:" << endl;
    for (int i = 0; i < ArrayIndex; ++i)
    {
        cout << "[" << i << "] " << TokenArray[i] << endl;
    }
}

void displayKeywordTokenArray2()
{
    cout << "Tokens Array2:" << endl;
    for (int i = 0; i < ArrayIndex2; ++i)
    {
        cout << "[" << i << "] " << TokenArray2[i] << endl;
    }
}

                                            //////////////////////////////////////
                                            //       Function definitions        //
                                            //////////////////////////////////////



// Define a set of keywords //set of ordered unique elements of type string
const set<string> keywords = {
    "int", "double", "struct", "break", "else", "long", "switch",
    "case", "enum", "register", "typedef", "char", "extern", "return",
    "union", "const", "float", "short", "unsigned", "continue", "for",
    "signed", "void", "default", "goto", "sizeof", "volatile", "do", "if",
    "static", "while", "asm", "bool", "catch", "class", "const_cast", "delete",
    "dynamic_cast", "explicit", "export", "false", "friend", "inline", "mutable",
    "namespace", "new", "operator", "private", "protected", "public",
    "reinterpret_cast", "static_cast", "template", "this", "throw", "true", "try",
    "typeid", "typename", "using", "virtual", "wchar_t", "string", "cin", "cout",
    "if", "main", "#include", "<iostream>", "<cctype>", "continue", "std"};

const set<char> separators = {' ', '(', ')', '{', '}', '[', ']', ':', ';', ',', '"', '"', '\n', '\0'};
const set<char> operators = {
    '+', '-', '*', '/', '%', '<', '>', '&', '|', '^', '=', '!', /*'<<','>>','+=','-=','*=',
         '/=','%=','&=','|=','^=','<<=','>>=','++','--','>>>','<<<', '==', '!=','<=','>=','&&','||'*/
};

bool isKeyword(const string &key)
{
    return keywords.count(key) > 0;
}

bool isSeparator(char ch)
{
    return separators.count(ch) > 0;
}

bool isOperator(char ch)
{
    return operators.count(ch) > 0;
}

                                                    ///////////////////////////////
                                                    //       Read Function       //
                                                    //////////////////////////////

string Read()
{
    ifstream FiletoRead("Code.cpp");
    if (!FiletoRead.is_open())
    {
        cout << "Error opening the file." << endl;
        return "";
    }

    stringstream Buffer; // reading file as string whole
    Buffer << FiletoRead.rdbuf();

    string content = Buffer.str();
    FiletoRead.close();
    return content;
}

                                            ////////////////////////////////////
                                            //       Translate Function       //
                                            ///////////////////////////////////

void Translate(const string &content)
{
    Lexical_Analyzer(content);
    TokenArray[ArrayIndex++] = "$";
    cout << endl;
    displayKeywordTokenArray();
}

                                            /////////////////////////////////
                                            //       Lexical Function       //
                                            //////////////////////////////////


void Lexical_Analyzer(const string &content)
{
    Token_Generator_By_Category(content);
    cout << endl;
    cout << "Total Tokens:"
         << " "
         << "(" << tokenCount << ")" << endl;
    cout << endl;
    cout << "------------------------------------------------" << endl;
}

// Define an array of function pointers, indexed by state number
void (*stateFunctions[])(char) =
    {
        state0, state1, state2, state3, state4, state5, state6, state7, state8, state9, state10, state11, state12, state13, state14,
        state15, state16, state17, state18, state19, state20, state21, state22, state23, state24, state25, state26, state27, state28};

                                            //////////////////////////////////////////
                                            //       Tokens_Generator Function       //
                                            /////////////////////////////////////////


void Token_Generator_By_Category(string content)
{
    content += '\0';
    for (i = 0; i < content.length(); ++i)
    {
        char cont = content[i];

        // Check either the state is from the array indexes or not
        // Dividing size of overall array from memory in bytes and takes size of first element of array
        // and divide it by total to check how many states or their in an array.
        if (state >= 0 && state < sizeof(stateFunctions) / sizeof(stateFunctions[0]))
        {
            stateFunctions[state](cont);
        }
        else
        {
            cout << "Error: Invalid state " << state << endl;
        }

        // Checks if content end and the flag of checking the closing quotes is still true so its mean
        //  closing quotes and out present so give an error.
        if (i == content.length() - 1 && withinStringLiteral)
        {
            cout << "\nError: Missing closing double-quote (\") for string literal" << endl;
        }
        else if (i == content.length() - 1 && withinCharacterLiteral)
        {
            cout << "\nError: Missing closing Single-quote (\') for Character literal" << endl;
        }
    }
}

                                                    /////////////////////////////////
                                                    //       States Function       //
                                                    /////////////////////////////////


                                                                ///////
                                                                // 0 //
                                                                //////

void state0(char cont)
{
    if (isalpha(cont) || cont == '_')
    {
        token += cont;
        state = 1;
    }
    else if (isdigit(cont))
    {
        token += cont;
        state = 3;
    }
    else if (cont == '"')
    {
        token += cont;
        withinStringLiteral = true;
        state = 6;
    }
    else if (cont == '\'')
    {
        token += cont;
        withinCharacterLiteral = true;
        state = 9;
    }
    else if (isOperator(cont))
    {
        if (cont == '+')
        {
            token += cont;
            state = 13;
        }
        else if (cont == '-')
        {
            token += cont;
            state = 16;
        }
        else if (cont == '!' || cont == '*' || cont == '%' || cont == '=' || cont == '^')
        {
            token += cont;
            state = 17;
        }
        else if (cont == '>')
        {
            token += cont;
            state = 18;
        }
        else if (cont == '<')
        {
            token += cont;
            state = 20;
        }
        else if (cont == '|')
        {
            token += cont;
            state = 22;
        }
        else if (cont == '&')
        {
            token += cont;
            state = 23;
        }
        else if (cont == '/')
        {
            token += cont;
            state = 24;
        }
    }
    else if (isSeparator(cont))
    {
        if (cont != ' ' || cont != '\n' || cont != '\0')
        {
            token += cont;
            state = 11;
            i--;
        }
    }
    else
    {
        cout << "Invalid Entry" << endl;
        cout << endl;
        state = 0;
    }
}
                                                                ///////
                                                                // 1 //
                                                                //////

void state1(char cont)
{
    if (isalpha(cont) || isdigit(cont) || cont == '_')
    {
        token += cont;
        state = 1;
    }
    else if (isSeparator(cont) || isOperator(cont))
    {
        state = 2;
        i--;
    }
    else
    {
        cout << "ERROR" << endl;
        token.clear();
        state = 0;
        i--;
    }
}

                                                                ///////
                                                                // 2 //
                                                                //////
void state2(char cont)
{
    i--;
    if (isKeyword(token))
    {
        if (token == "int" || token == "float" || token == "char" || token == "bool")
        {
            DataTypee = token;
        }
        tokentype = "keyword";
        cout << "token : " << token << "\n"
             << "Type  : " << tokentype << endl;
        TokenArray[ArrayIndex++] = token;
        TokenArray2[ArrayIndex2++] = token;
    }

    else
    {
        tokentype = "identifier";
        cout << "token : " << token << "\n"
             << "Type  : " << tokentype << endl;
        globalSymbolTable.insert(token, scope2, DataTypee);
        TokenArray[ArrayIndex++] = tokentype;
        TokenArray2[ArrayIndex2++] = token;
        DataTypee.clear();
    }
    cout << "*************************" << endl;
    cout << endl;
    tokenCount++;
    token.clear();
    tokentype.clear();
    state = 0;
}
                                                                ///////
                                                                // 3 //
                                                                //////

void state3(char cont)
{
    if (isdigit(cont))
    {
        token += cont;
        state = 3;
    }
    else if (cont == '.')
    {
        token += cont;
        state = 4;
    }
    else if (isSeparator(cont) || isOperator(cont))
    {
        state = 5;
        i--;
    }
    else
    {
        cout << "INVALID" << endl;
        cout << endl;
    }
}

                                                                ///////
                                                                // 4 //
                                                                //////
void state4(char cont)
{
    if (isdigit(cont))
    {
        token += cont;
        state = 12;
    }
    else
    {
        cout << "INVALID TOKEN" << endl;
        cout << endl;
        i--;
        token.clear();
        state = 0;
    }
}

                                                                ///////
                                                                // 12 //
                                                                //////

void state12(char cont)
{
    if (isdigit(cont))
    {
        token += cont;
        state = 12;
    }
    else if (cont == '.')
    {
        checkDoubledot = true;
        state = 12;
    }
    else if (isSeparator(cont) || isOperator(cont))
    {
        if (checkDoubledot == true)
        {
            cout << "INVALID TOKEN" << endl;
            cout << endl;
            i--;
            token.clear();
            state = 0;
        }
        else
        {
            state = 5;
            i--;
        }
    }
}

                                                                ///////
                                                                // 5 //
                                                                //////

void state5(char cont)
{
    i--;
    tokentype = "Constant";
    cout << "token : " << token << "\n"
         << "Type  : " << tokentype << endl;
    TokenArray[ArrayIndex++] = tokentype;
    TokenArray2[ArrayIndex2++] = tokentype;
    cout << "*************************" << endl;
    cout << endl;
    tokenCount++;
    token.clear();
    tokentype.clear();
    state = 0;
}

                                                                ///////
                                                                // 6 //
                                                                //////

void state6(char cont)
{
    if (cont == '"')
    {
        token += cont;
        withinStringLiteral = false;
        state = 8;
    }
    else if (cont == '\\')
    {
        state = 7;
    }
    else
    {
        token += cont;
        state = 6;
    }
}

                                                                ///////
                                                                // 8 //
                                                                //////

void state8(char cont)
{
    i--;
    tokentype = "String";
    cout << "token : " << token << "\n"
         << "Type  : " << tokentype << endl;
    TokenArray[ArrayIndex++] = tokentype;
    TokenArray2[ArrayIndex2++] = tokentype;
    cout << "*************************" << endl;
    cout << endl;
    tokenCount++;
    token.clear();
    tokentype.clear();
    state = 0;
}

                                                                ///////
                                                                // 7 //
                                                                //////

void state7(char cont)
{
    token += cont;
    state = 6;
}

                                                                ///////
                                                                // 9 //
                                                                //////

void state9(char cont)
{
    if (cont == '\'')
    {
        token += cont;
        withinCharacterLiteral = false;
        state = 8;
    }
    else if (cont == '\\')
    {
        state = 10;
    }
    else
    {
        token += cont;
        state = 9;
    }
}

                                                                ///////
                                                                // 10 //
                                                                //////
void state10(char cont)
{
    token += cont;
    state = 9;
}

                                                                ///////
                                                                // 11 //
                                                                //////
void state11(char cont)
{
    if (cont == ' ' || cont == '\n' || cont == '\0')
    {
        token.clear();
        tokentype.clear();
        state = 0;
    }
    else
    {
        if (cont == '{')
        {
            object1.ScopeINC();
        }
        else if (cont == '}')
        {
            object1.ScopeDEC();
        }
        tokentype = "Sparators";
        cout << "token : " << token << "\n"
             << "Type  : " << tokentype << endl;
        TokenArray[ArrayIndex++] = token;
        TokenArray2[ArrayIndex2++] = token;
        cout << "*************************" << endl;
        cout << endl;
        tokenCount++;
        token.clear();
        tokentype.clear();
        state = 0;
    }
}

                                                                ///////
                                                                // 13 //
                                                                //////

void state13(char cont)
{
    if (cont == '+' || cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 16 //
                                                                //////

void state16(char cont)
{
    if (cont == '-' || cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 17 //
                                                                //////

void state17(char cont)
{
    if (cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 18 //
                                                                //////

void state18(char cont)
{
    if (cont == '>')
    {
        token += cont;
        state = 19;
    }
    else if (cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 19 //
                                                                //////

void state19(char cont)
{
    if (cont == '>' || cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 20 //
                                                                //////

void state20(char cont)
{
    if (cont == '<')
    {
        token += cont;
        state = 21;
    }
    else if (cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 21 //
                                                                //////

void state21(char cont)
{
    if (cont == '<' || cont == '=')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 22 //
                                                                //////

void state22(char cont)
{
    if (cont == '=' || cont == '|')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}


                                                                ///////
                                                                // 23 //
                                                                //////

void state23(char cont)
{
    if (cont == '=' || cont == '&')
    {
        token += cont;
        state = 14;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 24 //
                                                                //////

void state24(char cont)
{
    if (cont == '=')
    {
        token += cont;
        state = 14;
    }
    else if (cont == '/')
    {
        token += cont;
        state = 25;
    }
    else if (cont == '*')
    {
        token += cont;
        state = 27;
    }
    else
    {
        state = 14;
        i--;
    }
}

                                                                ///////
                                                                // 25 //
                                                                //////

void state25(char cont)
{
    if (cont == '\n' || cont == '\0')
    {
        state = 26;
        i--;
    }
    else
    {
        token += cont;
        state = 25;
    }
}

                                                                ///////
                                                                // 26 //
                                                                //////

void state26(char cont)
{
    i--;
    tokentype = "Comment";
    cout << "token : " << token << "\n"
         << "Type  : " << tokentype << endl;
    cout << "*************************" << endl;
    cout << endl;
    token.clear();
    tokentype.clear();
    state = 0;
}

                                                                ///////
                                                                // 27 //
                                                                //////

void state27(char cont)
{
    if (cont == '*')
    {
        token += cont;
        state = 28;
    }
    else
    {
        token += cont;
        state = 27;
    }
}

                                                                ///////
                                                                // 28 //
                                                                //////

void state28(char cont)
{
    if (cont == '*')
    {
        token += cont;
        state = 28;
    }
    else if (cont == '/')
    {
        token += cont;
        state = 26;
    }
    else
    {
        token += cont;
        state = 27;
    }
}

                                                                ///////
                                                                // 14 //
                                                                //////

void state14(char cont)
{
    i--;
    tokentype = "Operator";
    cout << "token : " << token << "\n"
         << "Type  : " << tokentype << endl;
    TokenArray[ArrayIndex++] = token;
    TokenArray2[ArrayIndex2++] = token;
    cout << "*************************" << endl;
    cout << endl;
    tokenCount++;
    token.clear();
    tokentype.clear();
    state = 0;
}

void state15(char cont)
{
    if (isSeparator(cont) || isalpha(cont) || isdigit(cont))
    {
        if (flag15 == true)
        {
            cout << "ERROR" << endl;
            flag15 = false;
            cout << endl;
            token.clear();
            i--;
            state = 0;
        }
        else
        {
            state = 14;
            i--;
        }
    }
    else
    {
        state = 15;
        flag15 = true;
    }
}
/*void OperatorDup(char cont)
{
    if(isSeparator(cont) || isalpha(cont) || isdigit(cont))
    {
        state=14;
        i--;
    }else{
    cout<<"ERROR"<<endl;
    cout<<endl;
    token.clear();
    state=0;
    }
}*/

///////////////////////////////////////////////////////////////    SYNTAX SECTION      //////////////////////////////////////////////////////////////////

map<string, int> nonTerminals = {{"S", 0}, {"A", 1}, {"F", 2}, {"B", 3}, {"C", 4}, {"D", 5}, {"E", 6}, {"P", 7}, {"M", 8}, {"N", 9}, {"G", 10}, {"R", 11}, {"Q", 12}, {"T", 13}, {"O", 14}};
map<string, int> terminals = {{"$", 0}, {"int", 1}, {"identifier", 2}, {"=", 3}, {";", 4}, {"float", 5}, {"cout", 6}, {"<<", 7}, {"String", 8}, {"cin", 9}, {">>", 10}, {"if", 11}, {"(", 12}, {")", 13}, {"else", 14}, {"{", 15}, {"}", 16}, {"==", 17}, {"<", 18}, {">", 19}, {"<=", 20}, {">=", 21}, {"!=", 22}, {"Constant", 23}};
string parsingTable[15][24] = {
    {"", "S A", "0", "0", "0", "S B", "S C", "0", "0", "S D", "0", "S E", "0", "0", "0", "0", "", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "F identifier int", "0", "0", "0", "0", "", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "; O =", ";", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "F identifier float", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0", "; String << cout", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0", "0", "0", "0", "; identifier >> cin", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "N M ) G ( if", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "Q", "0", "0", "0", "Q", "Q", "0", "0", "Q", "0", "Q ) G ( if", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "Q", "0", "0", "0", "Q", "Q", "0", "0", "Q", "0", "0", "0", "0", "0", "R", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"", "", "0", "0", "0", "", "", "0", "0", "", "", "", "0", "0", "N P else", "0", "", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "O T identifier", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "O T Constant"},
    {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "} S {", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "A", "0", "0", "0", "B", "C", "0", "0", "D", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "==", "<", ">", "=<", "=>", "=!", "0"},
    {"0", "0", "identifier", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "Constant"}};

// Parser Function
void LL1Parser(string parsingTable[15][24], map<string, int> &nonTerminals,
               map<string, int> &terminals, array<string, MAX> &TokenArray)
{
    parsingStack.push("$");
    parsingStack.push("S");

    int currentIndex = 0;
    string currentToken = TokenArray[currentIndex];

    while (!parsingStack.empty())
    {
        string stackTop = parsingStack.top();
        parsingStack.pop();

        if (terminals.find(stackTop) == terminals.end())
        {
            //////////// Non-terminal symbol//////////////////////////
            int row = nonTerminals.at(stackTop);
            int col = terminals.at(currentToken);
            // cout << row << col << endl;

            string production = parsingTable[row][col];

            if (production != "0")
            {
                if (!production.empty())
                {
                    const int MaxSize = 15;
                    string Parray[MaxSize];
                    int arrayIndex = 0;
                    string currentWord;

                    for (char c : production)
                    {
                        if (c != ' ')
                        {
                            currentWord += c;
                        }
                        else
                        {
                            if (!currentWord.empty())
                            {
                                Parray[arrayIndex++] = currentWord;
                                currentWord = "";
                            }
                            if (arrayIndex >= MaxSize)
                            {
                                cout << "Error: Array size exceeded." << endl;
                                return;
                            }
                        }
                    }

                    if (!currentWord.empty())
                    {
                        Parray[arrayIndex++] = currentWord;
                    }

                    for (int i = 0; i < arrayIndex; ++i)
                    {
                        parsingStack.push(Parray[i]);
                    }
                }
            }
            else
            {
                cout << "Syntax error: Unexpected token '" << currentToken << "'"
                     << " "
                     << " Expected '" << stackTop << "'" << endl;
                break;
                return;
            }
        }
        // Terminal symbol
        else
        {
            if (stackTop == currentToken)
            {
                if (stackTop == "$" && currentToken == "$")
                {
                    cout << endl;
                    cout << "INPUT SUCCESSFULLY PARSED" << endl;
                    cout << endl;
                    break;
                }
                // cout << stackTop << " --->"<< " Accepted" << endl;
                currentIndex++;
                if (currentIndex < TokenArray.size())
                {
                    currentToken = TokenArray[currentIndex];
                }
            }
            else
            {
                cout << "Syntax error: Unexpected token '" << currentToken << "'"
                     << " "
                     << " Expected '" << stackTop << "'" << endl;

                return;
            }
        }
    }
}

///////SEMANTIC ANALYZER///////////////
void Semantic(array<string, MAX> &TokenArray, array<string, MAX> &TokenArray2)
{
    int SCOPE1 = 0;
    int SCOPE2 = 0;
    for (int i = 0; i < ArrayIndex2; i++)
    {
        string identifierName = TokenArray2[i];
        // string Datatype = TokenArray2[i - 1];

        if (TokenArray[i] == "identifier")
        {
            // cout<<identifierName<< " "<<TokenArray2[i-1]<<endl;
            if (TokenArray2[i - 1] == "int" || TokenArray2[i - 1] == "float" || TokenArray2[i - 1] == "char" || TokenArray2[i - 1] == "bool")
            {
                IdentifierInSymbolTable[IdentifierInSymbolTableArray++] = globalSymbolTable.SemanticChecking(identifierName, TokenArray2[i - 1], SCOPE2);
            }
            else
            {
                bool isDeclared = false;

                for (int i = 0; i < IdentifierInSymbolTableArray; i++)
                {
                    string A = IdentifierInSymbolTable[i];
                    if (identifierName == A)
                    {
                        // cout << identifierName << " " << "Accepted" << endl;
                        isDeclared = true;
                        int h = i;
                        if (SCOPE2 == StoringIndex[h])
                        {
                            // Accepted...
                        }
                        else if (StoringIndex[h] == 0)
                        {
                            // Accepted...
                        }
                        else
                        {
                            cout << "Variable Out Of Scope Error -> "
                                 << " " << identifierName << endl;
                        }
                        break;
                    }
                }

                if (!isDeclared)
                {
                    cout << "Variable Is Not Declared -> "
                         << " " << identifierName << endl;
                }
            }
        }
        else if (TokenArray[i] == "{")
        {
            SCOPE1++;
            SCOPE2 = SCOPE1;
        }
        else if (TokenArray[i] == "}")
        {
            SCOPE2--;
        }
    }
}

                                                        //////////////////////
                                                        //       Main        //
                                                        //////////////////////

int main()
{
    cout << "------------------------------------" << endl;
    cout << "|         LEXICAL ANALYZER         | " << endl;
    cout << "------------------------------------" << endl;
    string content = Read();
    Translate(content);
    globalSymbolTable.display();
    cout << endl;
    cout << "------------------------------------" << endl;
    cout << "|            SYNTAX ANALYZER       |" << endl;
    cout << "------------------------------------" << endl;
    LL1Parser(parsingTable, nonTerminals, terminals, TokenArray);
    cout << "------------------------------------" << endl;
    cout << "|           SEMANTIC ANALYZER      |" << endl;
    cout << "------------------------------------" << endl;
    cout << endl;
    Semantic(TokenArray, TokenArray2);
    return 0;
}
