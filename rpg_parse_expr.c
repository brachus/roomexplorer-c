

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"


#define P_EOF 				32
#define P_NEG_GET_INT_FLOAT	33
#define P_ARRAY_GET_ITEM	34
#define P_ARRAY_GET_SYM		35
#define P_OPEN_MEDIA		36
#define P_IMG_GETSTR		37
#define P_IMG_GETSTR_CLOSE	38
#define P_SND_GETSTR		39
#define P_SND_GETSTR_CLOSE	40


/*								var struct needs to be used for this.    */

void parse_literal_expr(struct token *tokens)
{	
	int md, ttype, tminus, hold;
	struct token_l *tmp;
	
	add_token(tokens, T_EOF, 0, 0, "");
	
	md = P_OPEN;
	ttype = NULL;
	tminus = 0;
	hold = 0;
	
	tmp = tokens->tok_first;
	
	while (tmp != NULL)
	{
		
		switch(md)
		{
		case P_OPEN:
			switch(tmp->type)
			{
			case T_NAME:
				vm_err(tmp->fname, tmp->line, tmp->col, "names are not allowed.");
				break;
			case T_INT:
			case T_FLOAT:
			case T_STR:
				
			}
			default;
		break:
			default;
		}
	}
}
