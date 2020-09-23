#include <cstdio>
#include <cstdlib> 
#include <iostream>
#include "scan.cpp"

using namespace std;

const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                       "equal", "nequal", "lthan", "gthan", "goreq", "loreq",
                       "if", "while", "end"};

static token input_token;

void error () {
    printf ("syntax error\n");
    exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        printf ("matched %s", names[input_token]);
        if (input_token == t_id || input_token == t_literal)
            printf (": %s", token_image);
        printf ("\n");
        input_token = scan ();
    }
    else error ();
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
void r_op(); // TODO: Figure out what r stands for
void condition();

// TODO: Delete "predict program" print statements when no longer useful

void program () {
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
        case t_eof:
            printf ("predict program --> stmt_list eof\n");
            stmt_list ();
            match (t_eof); // Do we only match when we consume a terminal in the line of the grammar we're in the method of?
            break;
        default: error ();
    }
}

void stmt_list () {
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
            printf ("predict stmt_list --> stmt stmt_list\n");
            stmt ();
            stmt_list ();
            break;
        case t_eof:
            printf ("predict stmt_list --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void stmt () {
    switch (input_token) {
        case t_id:
            printf ("predict stmt --> id gets expr\n");
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
            printf ("predict stmt --> read id\n");
            match (t_read);
            match (t_id);
            break;
        case t_write:
            printf ("predict stmt --> write expr\n");
            match (t_write);
            expr ();
            break;
        case t_if:
            match (t_if);
            condition();
            // Have conditional return T/F and call next line only if T?
            stmt_list();
            match(t_end);
            break;
        case t_while:
            match (t_while);
            condition();
            // Have conditional return T/F and call next line only if T?
            stmt_list();
            match(t_end);
            break;
        default: error ();
    }
}

void expr () {
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
            printf ("predict expr --> term term_tail\n");
            term ();
            term_tail ();
            break;
        default: error ();
    }
}

void term () {
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
            printf ("predict term --> factor factor_tail\n");
            factor ();
            factor_tail ();
            break;
        default: error ();
    }
}

void term_tail () {
    switch (input_token) {
        case t_add:
            add_op ();
            term ();
            term_tail ();
            break;
        case t_sub:
            printf ("predict term_tail --> add_op term term_tail\n");
            add_op ();
            term ();
            term_tail ();
            break;
        case t_rparen: //How could this be a rparen?
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
            printf ("predict term_tail --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void factor () {
    switch (input_token) {
        case t_literal:
            printf ("predict factor --> literal\n");
            match (t_literal);
            break;
        case t_id :
            printf ("predict factor --> id\n");
            match (t_id);
            break;
        case t_lparen:
            printf ("predict factor --> lparen expr rparen\n");
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default: error ();
    }
}

void factor_tail () {
    switch (input_token) {
        case t_mul:
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_div:
            printf ("predict factor_tail --> mul_op factor factor_tail\n");
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
        case t_eof:
            printf ("predict factor_tail --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void add_op () {
    switch (input_token) {
        case t_add:
            printf ("predict add_op --> add\n");
            match (t_add);
            break;
        case t_sub:
            printf ("predict add_op --> sub\n");
            match (t_sub);
            break;
        default: error ();
    }
}

void mul_op () {
    switch (input_token) {
        case t_mul:
            printf ("predict mul_op --> mul\n");
            match (t_mul);
            break;
        case t_div:
            printf ("predict mul_op --> div\n");
            match (t_div);
            break;
        default: error ();
    }
}

// Bare bones - will need more logic
void r_op () {
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
        default: error ();
    }
}

void condition() {
    // only execute if statement if this condi is true - make this non-void?
    // how do we know what r_op verifies the token is?
    switch (input_token) {
    }
}

int main () {
    input_token = scan ();
    cout << input_token;
    program ();
    return 0;
}
