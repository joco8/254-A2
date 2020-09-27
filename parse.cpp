#include <cstdio>
#include <cstdlib> 
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include "scan.h"

using namespace std;

const char* names[] = {"read", "write", "id", "literal", ":=",
                       "+", "-", "*", "/", "(", ")", "eof",
                       "=", "<>", "<", ">", ">=", "<=",
                       "if", "while", "end"};

static token input_token;

string syntax_tree = "";
static int totalErrors;
static int position;

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

static void print_expectations(std::string symbol) {
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
        cout << names[i] << ", ";

    }

}

void error () {
    cout << names[input_token];
    cout << "syntax error" << endl;
    if(input_token == t_eof) {
       
        cout << "could not recover, exiting" << endl;
        exit(1);
    }
    
    throw 1;
    return;
}

void report_error(std::string symbol) {
    cout << "Error at position " << position << " with token " <<  names[input_token] << endl;
    cout << "Expected token of the form ";
    print_expectations(symbol);
    cout << "\n";
    totalErrors ++;
}

void check_for_errors(std::string symbol) {
    if(!(FIRST(symbol) || EPS(symbol))) {
        
        report_error (symbol);
        input_token = scan();
        position ++;
        while (!FIRST(symbol) && !FOLLOW(symbol) && input_token != t_eof) {
            cout << "input token in error, skipping and moving on from token: " << names[input_token] << endl;
            input_token = scan();
            position ++;
        }
    }
    cout << "\n";
}

void match (token expected) {
    if (input_token == expected) {
        if (input_token == t_id)
            syntax_tree = syntax_tree + "\"" + token_image + "\" ";
        else if (input_token == t_literal)
        {
            syntax_tree = syntax_tree + "num \"" + token_image + "\" ";
        }
        else
        {
            syntax_tree = syntax_tree + names[input_token] + " ";
        }
        input_token = scan ();
        position++;
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

void program () {
    syntax_tree = syntax_tree + "(program";
    try {
        switch (input_token) {
            case t_id:
                stmt_list();
                break;
            case t_read:
                stmt_list();
                break;
            case t_write:
                stmt_list();
                break;
            case t_if:
                stmt_list();
                break;
            case t_while:
                stmt_list();
                break;
            case t_eof:
                stmt_list ();
                match (t_eof);
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("p");
        program();
        return;
    }
    syntax_tree = syntax_tree + ")";
    regex pattern_one("\\(\\)");
    regex pattern_two("\\s\\)");
    regex pattern_three("\\)=");
    regex pattern_four("\\(\\s\\(");
    regex pattern_five("\\)<>");
    regex pattern_six("\\)<");
    regex pattern_seven("\\)>");
    regex pattern_eight("\\)<=");
    regex pattern_nine("\\)>=");
    syntax_tree = regex_replace(syntax_tree, pattern_one, "");
    syntax_tree = regex_replace(syntax_tree, pattern_two, ")");
    syntax_tree = regex_replace(syntax_tree, pattern_three, ") =");
    syntax_tree = regex_replace(syntax_tree, pattern_four, "( (");
    syntax_tree = regex_replace(syntax_tree, pattern_five, ") <>");
    syntax_tree = regex_replace(syntax_tree, pattern_six, ") <");
    syntax_tree = regex_replace(syntax_tree, pattern_seven, ") >");
    syntax_tree = regex_replace(syntax_tree, pattern_eight, ") <=");
    syntax_tree = regex_replace(syntax_tree, pattern_nine, ") >=");

    printf("\n%s\n", syntax_tree.c_str());
}

void stmt_list () {
    syntax_tree = syntax_tree + "(";
    try {
        switch (input_token) {
            case t_id:
                stmt();
                stmt_list ();
                break;
            case t_read:
                stmt();
                stmt_list ();
                break;
            case t_write:
                stmt ();
                stmt_list ();
                break;
            case t_if:
                stmt();
                stmt_list();
                break;
            case t_while:
                stmt();
                stmt_list();
                break;
            case t_end:
            case t_eof:
                break; /* epsilon production */
            default:
                throw 2;
                error ();
                
        }
    } catch(int x) {
        if(x == 2) {
            cout << "Error in matching at position " << position << " with token " << names[input_token] << ", Skipping and moving on" << endl;
            
            cout << "\n";
            input_token = scan();
            position ++;
            totalErrors ++;
        }
        check_for_errors("stmt_list");
        stmt_list();
        return;
    }
    syntax_tree = syntax_tree + ")";
}

void stmt () {
    syntax_tree = syntax_tree + "(";
    try {
        switch (input_token) {
            case t_id:
                match (t_id);
                match (t_gets);
                expr ();
                break;
            case t_read:
                match (t_read);
                match (t_id);
                break;
            case t_write:
                match (t_write);
                expr ();
                break;
            case t_if:
                match (t_if);
                condition();
                stmt_list();
                match(t_end);
                break;
            case t_while:
                match (t_while);
                condition();
                stmt_list();
                match(t_end);
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("stmt");
        stmt();
        return;
    }
    syntax_tree = syntax_tree + ")";
}

void expr () {
    try{
        switch (input_token) {
            case t_id:
                term();
                term_tail();
                break;
            case t_literal:
                term();
                term_tail();
                break;
            case t_lparen:
                term ();
                term_tail ();
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("expr");
        expr();
        return;
    }
}

void term () {
    syntax_tree = syntax_tree + "(";
    try{ 
        switch (input_token) {
            case t_id:
                factor ();
                factor_tail ();
                break;
            case t_literal:
                factor ();
                factor_tail ();
                break;
            case t_lparen:
                factor ();
                factor_tail ();
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("term");
        term();
        return;
    }
    syntax_tree = syntax_tree + ")";
}

void term_tail () {
    try {  
        switch (input_token) {
            case t_add:
                add_op ();
                term ();
                term_tail ();
                break;
            case t_sub:
                add_op ();
                term ();
                term_tail ();
                break;
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
                break; /* epsilon production */
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("term_tail");
        term_tail();
        return;
    }
}

void factor () {
    
    try{      
        switch (input_token) {
            case t_literal:
                match (t_literal);
                break;
            case t_id :
                match (t_id);
                break;
            case t_lparen:
                match (t_lparen);
                expr ();
                match (t_rparen);
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("factor");
        factor();
        return;
    }
}

void factor_tail () {
    try {
        switch (input_token) {
            case t_mul:
                mul_op ();
                factor ();
                factor_tail ();
                break;
            case t_div:
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
                break;          /* epsilon production */
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("factor_tail");
        factor_tail();
        return;
    }
}

void add_op () {
    try {
            switch (input_token) {
            case t_add:
                
                match (t_add);
                break;
            case t_sub:
                
                match (t_sub);
                break;
            default:
                error ();
        }
    } catch (int x) {
        check_for_errors("add_op");
        add_op();
        return;
    }
}

void mul_op () {
    try {
        switch (input_token) {
        case t_mul:
            match (t_mul);
            break;
        case t_div:
            match (t_div);
            break;
        default:
            error ();
        }
    }  catch(int x) {
        check_for_errors("mul_op");
        mul_op();
        return;
    } 
       
}

void r_op () {
    try {
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
    } catch (int x) {
        check_for_errors("r_op");
        r_op();
        return;
    }

}

void condition() {
    syntax_tree = syntax_tree + "(";
    try {
        switch (input_token) {
            case t_lparen:
                expr();
                r_op();
                expr();
                break;
            case t_id:
                
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
    } catch (int x) {
        check_for_errors("c");
        condition();
        return;
    }
    syntax_tree = syntax_tree + ")";
}

int main () {
    totalErrors = 0;
    position = 1;

    input_token = scan ();
    program ();

    if(totalErrors > 0) {
        cout << totalErrors << " Errors found " << endl;
    }
    return 0;
}
