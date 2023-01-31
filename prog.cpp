/*
* Name: Lauren Morlock
* Email: lmorlock2021@fau.edu
*/

/*

    prog        ->  begin stmt_list end

    stmt_list   ->  stmt ; stmt_list
                 |  stmt
                 |	stmt;

    stmt        ->  var = expr

    var         ->  A | B | C

    expr        ->  var + expr
                 |  var - expr
                 |  var

*/

#include    <iostream>
#include    <fstream>
#include    <cctype>

#include    "token.h"
#include    "functions.h"

using namespace std;

ifstream ifs;                   // input file stream used by lexan
SymTab   symtab;                // global symbol table
Token    token;                 // global token
int      lookahead = 0;         // no look ahead token yet
bool     flag = 0;              //used for recursion in expr()

int      dbg = 1;               // debut is ON


int main()
{
    ifs = get_ifs();           // open an input file stream w/ the program
    init_kws();                // initialize keywords in the symtab
    match(lookahead);         // get the first input token
    prog();

    return 0;
}


void prog() {
    if (lookahead == KW) {
        emit(lookahead);
        cout << endl;

        match(lookahead);       //get next token
        stmt_list();            //parse stmt list
    }
    else {
        error(lookahead, KW, "begin");
    }

    if (lookahead == KW) {
        emit(lookahead);
    }
    else {
        error(lookahead, KW, "end");
    }
}

void stmt_list() {
    stmt();                 //parse first stmt

    if (lookahead != KW) {          //if not end
        if (lookahead == ';') {
            emit(lookahead);        //emit semicolon
        }

        match(lookahead);
    }
    if (lookahead != KW) {      //if still not end
        stmt_list();
    }
    else {
        cout << endl;
    }

}

void stmt() {
    int holder;             //holds op for postfix

    var();

    match(lookahead);           //get '='

    if (lookahead == '=') {
        holder = lookahead;     //hold '='

        match(lookahead);
        expr();

        emit(holder);           //emit '='
    }
    else {
        error(lookahead, '=');
    }
}

void var() {
    if (lookahead == ID) {
        emit(lookahead);
    }
    else {
        error(lookahead, ID);
    }
}

void expr() {
    int holder;

    var();
    
    if (flag == 0) {            //if first time in expr()
        match(lookahead);
    }

    while (lookahead == '+' || lookahead == '-' && flag == 0) {
        flag = 1;               //do not enter loop when recursing in expr; messes up postfix
        holder = lookahead;     //hold op
        match(lookahead);       //get next ID
        expr();                 //recurse on expr; do not enter this loop during recursion
        emit(holder);           //emit op
        match(lookahead);       //get next op
        flag = 0;               //able to enter this loop again
    }
    //Please note: I did not include specific error checking for '+' or '-', as they were not placed in a class,
    //and thus hard to check for. They were also not included as cases in the error function.
    //If there is an unrecognized operator, such as '/', it will be caught by var or prog anyway.
}

// utility methods

void emit(int t)
{
    switch (t)
    {
    case '+': case '-': case '=':
        cout << char(t) << ' ';
        break;

    case ';':
        cout << ";\n";
        break;

    case '\n':
        cout << "\n";
        break;

    case ID:
    case KW:
    case UID:
        cout << symtab.tokstr(token.tokvalue()) << ' ';
        break;

    default:
        cout << "'token " << t << ", tokvalue "
            << token.tokvalue() << "' ";
        break;
    }
}

void error(int t, int expt, const string& str)
{
    cerr << "\nunexpected token '";
    if (lookahead == DONE)
    {
        cerr << "EOF";
    }
    else
    {
        cerr << token.tokstr();
    }
    cerr << "' of type " << lookahead;

    switch (expt)
    {
    case 0:         // default value; nothing to do
        break;

    case ID:
        cout << " while looking for an ID";
        break;

    case KW:
        cout << " while looking for KW '" << str << "'";
        break;

    default:
        cout << " while looking for '" << char(expt) << "'";
        break;
    }

    cerr << "\nsyntax error.\n";

    exit(1);
}


void match(int t)
{
    if (lookahead == t)
    {
        token = lexan();               // get next token
        lookahead = token.toktype();   // lookahed contains the tok type
    }
    else
    {
        error(t);
    }
}

