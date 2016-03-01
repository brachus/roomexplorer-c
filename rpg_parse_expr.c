

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"



/* creates new var from literal expression (tokens) */
struct var *parse_literal_expr(struct token *tokens)
{	
	
	int md, ttype, tminus, hold, i;
	struct token_l *tmp;
	struct var *nvar;
	
	/* used for keeping track multi-dimensional lists */
	struct var *map[MAX_LIST_NESTS];
	for (i=0;i<7;i++)
		map[i] = NULL;
	int map_lvl = 0;
	
	add_token(tokens, T_EOF, 0, 0, "");
	
	md = P_OPEN;
	ttype = -1;
	tminus = 1;
	hold = 0;
	
	tmp = tokens->tok_first;
	
	while (tmp != NULL)
	{
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
				if ( str_cmp_cstr(tmp->dat_str,"[") )
				{
					nvar = new_var();
					nvar->type = V_LIST;
					
					map_lvl = 0; /* 0 is 'outside' of list, where ONLY nvar occupies. */
									/* the 'base' var for a list always has its .list_next value NULL. */
					map[map_lvl] = nvar;
					
					map_lvl++;
					
					md = P_ARRAY_GET_ITEM;
				}
					
				break;
			default:
				break;
			}
			break;
		case P_ARRAY_GET_ITEM:
			if (tmp->type == T_FLOAT || tmp->type == T_INT || tmp->type == T_STR)
			{
				if (map[map_lvl] == NULL)
				{
					map[map_lvl-1]->dat_list = new_var();
					map[map_lvl] = map[map_lvl-1]->dat_list;
					
				}
				else
				{
					map[map_lvl]->list_next = new_var();
					map[map_lvl] = map[map_lvl]->list_next;
				}
				
				md = P_ARRAY_GET_SYM;
			}
			switch (tmp->type)
			{
			case T_FLOAT:
				map[map_lvl]->type = V_FLOAT;
				map[map_lvl]->dat_float = tmp->dat_float * tminus;
				
				tminus = 1;
				break;
			case T_INT:
				map[map_lvl]->type = V_INT;
				map[map_lvl]->dat_int = tmp->dat_int * tminus;
				
				tminus = 1;
				break;
			case T_STR:
				map[map_lvl]->type = V_STR;
				map[map_lvl]->dat_str = str_cpy(tmp->dat_str);
				tminus = 1;
				break;
			case T_NAME:
				vm_err(tmp->fn, tmp->line, tmp->col, "names aren't supported in lists.");
				break;
			case T_SYM:
				if ( str_cmp_cstr(tmp->dat_str,"]") )
				{
					
					/* step down list level */
					map[map_lvl] = NULL;
					map_lvl--;
					
					if (map_lvl == 0)
						md = P_EOF;
					
					md = P_ARRAY_GET_SYM;
				}
				else if ( str_cmp_cstr(tmp->dat_str,"[") )
				{
					if (map[map_lvl] == NULL)
					{
						map[map_lvl-1]->dat_list = new_var();
						map[map_lvl] = map[map_lvl-1]->dat_list;
					}
					else
					{
						map[map_lvl]->list_next = new_var();
						map[map_lvl] = map[map_lvl]->list_next;
					}
					
					map[map_lvl]->type = V_LIST;
					
					map_lvl++;
					
					if (map_lvl >= MAX_LIST_NESTS)
						vm_err(tmp->fn, tmp->line, tmp->col, "too nested lists.");
				}
				else if ( str_cmp_cstr(tmp->dat_str,"-") )
					tminus = -1;
				else
					vm_err(tmp->fn, tmp->line, tmp->col, "unexpected symbol.");
				break;
			}
			
			break;
		case P_ARRAY_GET_SYM:
			if (tmp->type == T_SYM)
			{
				if ( str_cmp_cstr(tmp->dat_str,"]") )
				{
					
					/* step down list level */
					map[map_lvl] = NULL;
					map_lvl--;
					
					if (map_lvl == 0)
						md = P_EOF;
					
				}
				else if ( str_cmp_cstr(tmp->dat_str,","))
				{
					md = P_ARRAY_GET_ITEM;
				}
				else
					vm_err(tmp->fn, tmp->line, tmp->col, "expected \",\" or \"]\".");
			}
			else
				vm_err(tmp->fn, tmp->line, tmp->col, "expected \",\" or \"]\".");
				
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
