

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"


#define P_EOF 				32
#define P_NEG_GET_INT_FLOAT	33
#define P_ARRAY_GET_ITEM	34
#define P_ARRAY_GET_SYM		35
#define P_OPEN_MEDIA		36
#define P_IMG_GETSTR		37
#define P_IMG_GETSTR_CLOSE	38
#define P_SND_GETSTR		39
#define P_SND_GETSTR_CLOSE	40


/* creates new var from literal expression (tokens) */
struct var *parse_literal_expr(struct token *tokens)
{	
	
	int md, ttype, tminus, hold;
	struct token_l *tmp;
	struct var *nvar;
	
	add_token(tokens, T_EOF, 0, 0, "");
	
	md = P_OPEN;
	ttype = NULL;
	tminus = 0;
	hold = 0;
	
	tmp = tokens->tok_first;
	
	while (tmp != NULL)
	{
		printf("%d\n",md);
		switch(md)
		{
		case P_OPEN:
			switch(tmp->type)
			{
			
			case T_INT:
				nvar = new_var();
				nvar->type = V_INT;
				nvar->dat_int = tmp->dat_int;
				md = P_EOF;
				break;
			case T_FLOAT:
				nvar = new_var();
				nvar->type = V_FLOAT;
				nvar->dat_float = tmp->dat_float;
				md = P_EOF;
			case T_NAME:
				nvar = new_var();
				nvar->type = V_STR;
				nvar->dat_str = str_cpy(&tmp->dat_str[0]);
				
				if (tmp->dat_str[1].length > 0)
					nvar->dat_str_1 = str_cpy(&tmp->dat_str[1]);
				
				if (tmp->dat_str[2].length > 0)
					nvar->dat_str_2 = str_cpy(&tmp->dat_str[2]);
				
				md = P_EOF;
				break;
			case T_STR:
				nvar = new_var();
				nvar->type = V_STR;
				nvar->dat_str = str_cpy(tmp->dat_str);
				md = P_EOF;
				break;
			case T_SYM:
				break;
			default:
				break;
			}
			break;
		case P_EOF:
			if (tmp->type != T_EOF)
				vm_err(tmp->fn, tmp->line, tmp->col, "expected end of literal.");
			break;
		default:
			break;
		}
		
		tmp = tmp->next;
	}
	
	return nvar;
}
