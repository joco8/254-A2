/* Definitions the scanner shares with the parser
    Michael L. Scott, 2008-2020.
*/

typedef enum {t_read, t_write, t_id, t_literal, t_gets, // t_gets is ':='
              t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof,
              t_equal, t_nequal, t_lthan, t_gthan, t_goreq, t_loreq,
              t_if, t_while, t_end} token;

#define MAX_TOKEN_LEN 128
extern char token_image[MAX_TOKEN_LEN];

extern token scan();
