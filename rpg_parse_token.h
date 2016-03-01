


#define T_INT	0
#define T_FLOAT	1
#define T_STR	2
#define T_NAME	3
#define T_SYM	4
#define T_EOF	5
#define T_ILLEGAL	6


#define P_OPEN		0
#define P_INT		1
#define P_FLOAT		2
#define P_NAME		3
#define P_NAME_DOT	4
#define P_DOT		5
#define P_DOUBLEQUOTE		6
#define P_SINGLEQUOTE		7
#define P_DOUBLEQUOTE_ESC	8
#define P_SINGLEQUOTE_ESC	9
#define P_MACRO				10
#define P_COM_GET_STAR		11
#define P_COMMENT			12
#define P_COM_GET_SLASH		13


struct token_l
{
	struct token_l *next;
	
	int type;
	
	struct str *dat_str; /* for compound names (xx.xx) */
		/* for now, in ".xx", the first name will be a \0 character */
	int dat_int;
	float dat_float;
	
	int line;
	int col;
	char *fn;
};

struct token
{
	struct token_l *tok_first;
	struct token_l *tok;
};

void token_init(struct token* tokens);

void add_token(
	struct token* tokens,
	int type,
	int line,
	int col,
	char* fname);
	
void parse_tokenize(
	struct str *fn,
	struct token* tokens);
