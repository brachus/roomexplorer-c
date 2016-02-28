

#define PARSE_DEBUG 0

#define NUMERAL     0
#define SYMBOL      1
#define QUOTE       2
#define LETTER      3
#define WHITESPACE  4

int is_numeral(char ch);
int is_symbol(char ch);
int is_quote(char ch);
int is_letter(char ch);
int is_whitespace(char ch);
int char_identify(char ch);
int parse_ret_reg( struct str *in );
