

#define P_EOF 				32
#define P_NEG_GET_INT_FLOAT	33
#define P_ARRAY_GET_ITEM	34
#define P_ARRAY_GET_SYM		35
#define P_OPEN_MEDIA		36
#define P_IMG_GETSTR		37
#define P_IMG_GETSTR_CLOSE	38
#define P_SND_GETSTR		39
#define P_SND_GETSTR_CLOSE	40

#define MAX_LIST_NESTS		8

struct idnt *parse_lexpr_idnt(struct token *tokens);
struct var *parse_literal_expr(struct token *tokens);
