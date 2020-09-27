#include <cstdio>
#include <cstdlib> 
#include <cstring>
#include "scan.h"
#include <iostream>
#include <cwctype>
#include <cctype>

using namespace std;

char token_image[MAX_TOKEN_LEN]; 

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    /* skip white space */
    while (isspace(c)) {
        c = getchar();
    }
    if (c == EOF)
        return t_eof;
        
    if (isalpha(c)) { // is c an alphabetic letter
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
                cout << "max token length exceeded\n";
                exit(1);
            }
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "if")) return t_if;
        else if (!strcmp(token_image, "while")) return t_while;
        else if (!strcmp(token_image, "end")) return t_end;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if (char(c = getchar()) != '=') {
                fprintf(stderr, "error\n");
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        case '=': c = getchar(); return t_equal;
        case '<': 
            c = getchar();
            if (c == '>') {
                c = getchar();
                return t_nequal;
            } else if (c == '=') {
                c = getchar();
                return t_loreq;
            } else {
                return t_lthan;
            }
        case '>': 
            c = getchar();
            if (c == '=') {
                c = getchar();
                return t_goreq;
            } else {
                return t_gthan;
            }
        default:
            printf("error\n");
            exit(1);
    }
}

