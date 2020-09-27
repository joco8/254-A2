#include <cstdio>
#include <cstdlib> 
#include <iostream>
#include <vector>
#include <map>
#include "scan.h"

using namespace std;

const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                       "equal", "nequal", "lthan", "gthan", "goreq", "loreq",
                       "if", "while", "end"};

static token input_token;

string syntax_tree = "";

// Create dictionaries for first and follow sets of each production 
static bool FIRST(std::string symbol){
    vector<token> p = {t_id, t_read, t_write, t_if, t_while, t_eof};
    vector<token> stmt_list = {t_id, t_read, t_write, t_if, t_while};
    vector<token> stmt = {t_id, t_read, t_write, t_if, t_while};
    vector<token> expr = {t_lparen, t_id, t_literal};
    vector<token> term_tail = {t_add, t_sub};
    vector<token> term = {t_lparen, t_id, t_literal};
    vector<token> factor_tail = {t_mul, t_div};
    vector<token> factor = {t_lparen, t_id, t_literal};
    vector<token> add_op = {t_add, t_sub};
    vector<token> mul_op = {t_mul, t_div};
    vector<token> r_op = {t_equal, t_nequal, t_goreq, t_gthan, t_loreq, t_lthan};
    vector<token> c = {t_lparen, t_id, t_literal};
    
    map<std::string, vector<token> > symbolTable;
    symbolTable = {
        {"p", p}, {"stmt_list", stmt_list}, {"stmt", stmt}, {"expr", expr}, {"term_tail", term_tail},
     {"term", term}, {"factor_tail", factor_tail}, {"factor", factor}, {"add_op", add_op}, {"mul_op", mul_op}, {"c", c}, {"r_op", r_op}
     
    };

    
    for(token i : symbolTable[symbol]) {
        
        if(input_token == i) {
            return true;
        }
    }

    return false;
}

static bool FOLLOW(std::string symbol) {
    // Create Dictionaries for follow sets 
    vector<token> p = {};
    vector<token> stmt_list = {t_eof};
    vector<token> stmt = {t_id, t_read, t_write, t_eof};
    vector<token> expr = {t_rparen, t_id, t_read, t_write, t_eof, t_equal, t_nequal, t_goreq, t_gthan, t_loreq, t_lthan};
    vector<token> term_tail = {t_rparen, t_id, t_read, t_write, t_eof};
    vector<token> term = {t_add, t_sub, t_rparen, t_id, t_read, t_write ,t_eof};
    vector<token> factor_tail = {t_add, t_sub, t_rparen, t_id, t_read, t_write, t_eof};
    vector<token> factor = { t_add, t_sub, t_mul, t_div, t_rparen, t_id, t_read, t_write, t_eof};
    vector<token> add_op = {t_lparen, t_id, t_literal};
    vector<token> mul_op = {t_lparen, t_id, t_literal};
    vector<token> r_op = {t_rparen, t_id, t_read, t_write, t_eof};
    vector<token> c = {t_equal, t_nequal, t_goreq, t_gthan, t_loreq, t_lthan};
    


    map<std::string, vector<token> > symbolTable;
    symbolTable = {
        {"p", p}, {"stmt_list", stmt_list}, {"stmt", stmt}, {"expr", expr}, {"term_tail", term_tail},
     {"term", term}, {"factor_tail", factor_tail}, {"factor", factor}, {"add_op", add_op}, {"mul_op", mul_op}, {"c", c}, {"r_op", r_op}
     
    };

    for(token i : symbolTable[symbol]) {
        cout << i;
        if(input_token == i) {
            return true;
        }
    }

    return false;

}

// epsilon values for each production 
static bool EPS(std::string production){

    bool p = false;
    bool stmt_list = true; 
    bool stmt =  false;
    bool expr = false;
    bool term_tail = true;
    bool term = false;
    bool factor_tail = true; 
    bool factor = false;
    bool add_op = false;
    bool mul_op = false;

    const std::string cArr[] = {"p", "stmt_list", "stmt", "expr", "term_tail", "term", "factor_tail", "factor", "add_op", "mul_op"};
    const bool arr[] = {p, stmt_list, stmt, expr, term_tail, term, factor_tail, factor, add_op, mul_op};
    for(int i = 0; i < sizeof(cArr); i++){
        if (cArr[i] == production) {
            
            return arr[i];
        }
    }

    return false;
}

void error () {
    printf ("syntax error\n");
    return;
    // exit (1);
}

void report_error() {
    printf("\nWE GOT AN ERROR BABY WHAT DO I DO NOW?\n");
}

void check_for_errors(std::string symbol) {
    cout << symbol;
    if(!(FIRST(symbol) || EPS(symbol))) {
        report_error ();
        printf("input token in error %s \n", names[input_token]);
        input_token = scan();
        while (!FIRST(symbol) && !FOLLOW(symbol) && input_token != t_eof) {
            printf("input token in error %s \n", names[input_token]);
            input_token = scan();
        }
    }
}

void match (token expected) {
    
    if (input_token == expected) {
        printf ("matched %s\n", names[input_token]); // Remove printf
        if (input_token == t_id || input_token == t_literal)
            syntax_tree = syntax_tree + "\"" + token_image + "\" ";
        printf ("\n"); // Remove printf
        
        input_token = scan ();
    }
    else 
        error ();
}

void program ();
void stmt_list ();
void stmt ();
void expr ();
void term ();
void term_tail ();
void factor ();
void factor_tail ();
void add_op ();
void mul_op ();
void r_op();
void condition();

// TODO: Delete "predict program" print statements when no longer useful - can not use 'printf'

void program () {
    check_for_errors("p");
    syntax_tree = syntax_tree + "(program";
    switch (input_token) {
        case t_id:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list();
            break;
        case t_read:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list();
            break;
        case t_write:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list();
            break;
        case t_if:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list();
            break;
        case t_while:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list();
            break;
        case t_eof:
            // printf ("predict program --> stmt_list eof\n");
            stmt_list ();
            match (t_eof); // Do we only match when we consume a terminal in the line of the grammar we're in the method of?
            break;
        default:
            error ();
    }
    syntax_tree = syntax_tree + ")";
    printf("\n%s\n", syntax_tree.c_str());
}

void stmt_list () {
    check_for_errors("stmt_list");
    syntax_tree = syntax_tree + "[";
    switch (input_token) {
        case t_id:
            // printf ("predict stmt_list --> stmt stmt_list\n");
            stmt();
            stmt_list ();
            break;
        case t_read:
            // printf ("predict stmt_list --> stmt stmt_list\n");
            stmt();
            stmt_list ();
            break;
        case t_write:
            // printf ("predict stmt_list --> stmt stmt_list\n");
            stmt ();
            stmt_list ();
            break;
        case t_if:
            // printf ("predict stmt_list --> stmt stmt_list\n");
            stmt();
            stmt_list();
            break;
        case t_while:
            // printf ("predict stmt_list --> stmt stmt_list\n");
            stmt();
            stmt_list();
            break;
        case t_end:
        case t_eof:
            // printf ("predict stmt_list --> epsilon\n");
            break;          /* epsilon production */
        default:
            error ();
    }
    syntax_tree = syntax_tree + "]";
}

void stmt () {
    check_for_errors("stmt");
    syntax_tree = syntax_tree + "(";
    switch (input_token) {
        case t_id:
            // printf ("predict stmt --> id gets expr\n");
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
            // printf ("predict stmt --> read id\n");
            match (t_read);
            match (t_id);
            break;
        case t_write:
            // printf ("predict stmt --> write expr\n");
            match (t_write);
            expr ();
            break;
        case t_if:
            // printf ("predict stmt --> if cond\n");
            match (t_if);
            condition();
            stmt_list();
            match(t_end);
            break;
        case t_while:
            // printf ("predict stmt --> while cond\n");
            match (t_while);
            condition();
            stmt_list();
            match(t_end);
            break;
        default:
            error ();
    }
    syntax_tree = syntax_tree + "(";
}

void expr () {
    check_for_errors("expr");
    switch (input_token) {
        case t_id:
            // printf ("predict expr --> term term_tail\n");
            term();
            term_tail();
            break;
        case t_literal:
            // printf ("predict expr --> term term_tail\n");
            term();
            term_tail();
            break;
        case t_lparen:
            // printf ("predict expr --> term term_tail\n");
            term ();
            term_tail ();
            break;
        default:
            error ();
    }
}

void term () {
    check_for_errors("term");
    switch (input_token) {
        case t_id:
            // printf ("predict term --> factor factor_tail\n");
            factor ();
            factor_tail ();
            break;
        case t_literal:
            // printf ("predict term --> factor factor_tail\n");
            factor ();
            factor_tail ();
            break;
        case t_lparen:
            // printf ("predict term --> factor factor_tail\n");
            factor ();
            factor_tail ();
            break;
        default:
            error ();
    }
}

void term_tail () {
    check_for_errors("term_tail");
    syntax_tree = syntax_tree + "(";
    switch (input_token) {
        case t_add:
            // printf ("predict term_tail --> add_op term term_tail\n");
            add_op ();
            term ();
            term_tail ();
            break;
        case t_sub:
            // printf ("predict term_tail --> sub_op term term_tail\n");
            add_op ();
            term ();
            term_tail ();
            break;
        case t_rparen: //How could this be a rparen? - if the expression is already finished - do nothing here?
        case t_id:
        case t_read:
        case t_write:
        case t_equal:
        case t_nequal:
        case t_gthan:
        case t_lthan:
        case t_goreq:
        case t_loreq:
        case t_end:
        case t_eof:
            // printf ("predict term_tail --> epsilon\n");
            break;          /* epsilon production */
        default:
            error ();
    }
    syntax_tree = syntax_tree + ")";
}

void factor () {
    check_for_errors("factor");
    syntax_tree = syntax_tree + "(";
    switch (input_token) {
        case t_literal:
            // printf ("predict factor --> literal\n");
            match (t_literal);
            break;
        case t_id :
            // printf ("predict factor --> id\n");
            match (t_id);
            break;
        case t_lparen:
            // printf ("predict factor --> lparen expr rparen\n");
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default:
            error ();
    }
    syntax_tree = syntax_tree + ")";
}

void factor_tail () {
    check_for_errors("factor_tail");
    syntax_tree = syntax_tree + "(";
    switch (input_token) {
        case t_mul:
        // printf ("predict factor_tail --> mul_op factor factor_tail\n");
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_div:
            // printf ("predict factor_tail --> div_op factor factor_tail\n");
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_equal:
        case t_nequal:
        case t_gthan:
        case t_lthan:
        case t_goreq:
        case t_loreq:
        case t_end:
        case t_eof:
            // printf ("predict factor_tail --> epsilon\n");
            break;          /* epsilon production */
        default:
            error ();
    }
    syntax_tree = syntax_tree + ")";
}

void add_op () {
    check_for_errors("add_op");
    switch (input_token) {
        case t_add:
            // printf ("predict add_op --> add\n");
            match (t_add);
            break;
        case t_sub:
            // printf ("predict add_op --> sub\n");
            match (t_sub);
            break;
        default:
            error ();
    }
}

void mul_op () {
    check_for_errors("mul_op");
    switch (input_token) {
        case t_mul:
            // printf ("predict mul_op --> mul\n");
            match (t_mul);
            break;
        case t_div:
            // printf ("predict mul_op --> div\n");
            match (t_div);
            break;
        default:
            error ();
    }
}

// Bare bones - will need more logic
void r_op () {
    check_for_errors("r_op");
    switch (input_token) {
        case t_equal:
            match (t_equal);
            break;
        case t_nequal:
            match (t_nequal);
            break;
        case t_lthan:
            match (t_lthan);
            break;
        case t_gthan:
            match (t_gthan);
            break;
        case t_loreq:
            match (t_loreq);
            break;
        case t_goreq:
            match (t_goreq);
            break;
        default:
            error ();
    }
}

void condition() {
    // return non-void? expr rop expr must be true for SL in S to execute
    check_for_errors("c");
    syntax_tree = syntax_tree + "(";
    switch (input_token) {
        case t_lparen:
            expr();
            r_op();
            expr();
            break;
        case t_id:
            // printf("Reading id for conditional\n");
            expr();
            r_op();
            expr();
            break;
        case t_literal:
            expr();
            r_op();
            expr();
            break;
        default:
            error();
    }
    syntax_tree = syntax_tree + ")";
}

int main () {
    input_token = scan ();
    program ();
    return 0;
}
