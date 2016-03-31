

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_media.h"
#include "rpg_parse_expr.h"


struct idnt *parse_lexpr_idnt(struct token *tokens, struct media_lib *md_lib)
{
	struct idnt *nidnt = new_idnt();
	struct var *nvar;
	
	
	nvar = parse_literal_expr(tokens, md_lib);
	
	if (nvar->type == V_NAME)
	{
		if (!nvar->dat_str_1)
		{
			if (parse_ret_reg(nvar->dat_str) != -1)
			{
				nidnt->type = IDNT_REG;
				nidnt->idx = parse_ret_reg(nvar->dat_str);
				nidnt->use_var = nvar;
			}
			else
			{
				nidnt->type = IDNT_OBJ;
				nidnt->obj_name = str_cpy(nvar->dat_str);
				nidnt->use_var = nvar;
			}
			
		}
		else
		{
			nidnt->type = IDNT_OBJVAR;
			nidnt->obj_name = str_cpy(nvar->dat_str);
			nidnt->var_name = str_cpy(nvar->dat_str_1);
			nidnt->use_var = nvar;
		}
		
	}
	else
	{
		nidnt->type = IDNT_VAR;
		nidnt->use_var = nvar;
	}
	
	return nidnt;
}

/* creates new var from literal expression (tokens) */
struct var *parse_literal_expr(struct token *tokens, struct media_lib *md_lib)
{	
	int md, ttype, tminus, hold, i;
	struct token_l *tmp;
	struct var *nvar;
	struct var *vtmp = 0;
	
	/* used for keeping track multi-dimensional lists */
	struct var *map[MAX_LIST_NESTS];
	for (i=0; i<MAX_LIST_NESTS; i++)
		map[i] = 0;
		
	int map_lvl = 0;

	add_token(tokens, T_EOF, 0, 0, "");
	
	
	
	md = P_OPEN;
	ttype = -1;
	tminus = 1;
	hold = 0;
	
	tmp = tokens->first;
	
	while (tmp != 0)
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
				break;
			case T_NAME:
				nvar = new_var();
				nvar->type = V_NAME;
				nvar->dat_str = str_cpy(tmp->dat_str[0]);
				
				if (tmp->dat_str[1]->length > 0)
					nvar->dat_str_1 = str_cpy(tmp->dat_str[1]);
				
				if (tmp->dat_str[2]->length > 0)
					nvar->dat_str_2 = str_cpy(tmp->dat_str[2]);
				
				md = P_EOF;
				break;
			case T_STR:
				nvar = new_var();
				nvar->type = V_STR;
				nvar->dat_str = str_cpy(tmp->dat_str[0]);
				md = P_EOF;
				break;
			case T_SYM:
				if ( str_cmp_cstr(tmp->dat_str[0],"[") )
				{
					nvar = new_var();
					nvar->type = V_LIST;
					
					map_lvl = 0; /* 0 is 'outside' of list, where ONLY nvar occupies. */
									/* the 'base' var for a list always has its .list_next value NULL. */
					map[map_lvl] = nvar;
					
					map_lvl++;
					
					md = P_ARRAY_GET_ITEM;
				}
				else if ( str_cmp_cstr(tmp->dat_str[0],"{") )
					md = P_OPEN_MEDIA;
				else if ( str_cmp_cstr(tmp->dat_str[0],"-") )
					md = P_NEG_GET_INT_FLOAT;
				else
					vm_err(tmp->fn, tmp->line, tmp->col, "unexpected token.");
				break;
			default:
				break;
			}
			break;
		case P_NEG_GET_INT_FLOAT:
			if (tmp->type == T_INT)
			{
				nvar = new_var();
				nvar->type = V_INT;
				nvar->dat_int = -(tmp->dat_int);
				md = P_EOF;
			}
			else if (tmp->type == T_FLOAT)
			{
				nvar = new_var();
				nvar->type = V_INT;
				nvar->dat_int = -(tmp->dat_int);
				md = P_EOF;
			}
			else
				vm_err(tmp->fn, tmp->line, tmp->col, "expected numeral.");
			break;
		case P_ARRAY_GET_ITEM:
			if (tmp->type == T_FLOAT || tmp->type == T_INT || tmp->type == T_STR || tmp->type == T_NAME)
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
				map[map_lvl]->dat_str = str_cpy(tmp->dat_str[0]);
				tminus = 1;
				break;
			case T_NAME:
				map[map_lvl]->type = V_NAME;
				map[map_lvl]->dat_str = str_cpy(tmp->dat_str[0]);
				
				if (tmp->dat_str[1]->length > 0)
					map[map_lvl]->dat_str_1 = str_cpy(tmp->dat_str[1]);
				
				if (tmp->dat_str[2]->length > 0)
					map[map_lvl]->dat_str_2 = str_cpy(tmp->dat_str[2]);
				
				tminus = 1;
					
				break;
			case T_SYM:
				if ( str_cmp_cstr(tmp->dat_str[0],"]") )
				{
					/* step down list level */
					map[map_lvl] = 0;
					map_lvl--;
					
					if (!map_lvl)
						md = P_EOF;
					else
						md = P_ARRAY_GET_SYM;
				}
				else if ( str_cmp_cstr(tmp->dat_str[0],"[") )
				{
					if (!map[map_lvl])
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
						vm_err(tmp->fn, tmp->line, tmp->col, "too many nested lists.");
				}
				else if ( str_cmp_cstr(tmp->dat_str[0],"-") )
					tminus = -1;
				else
					vm_err(tmp->fn, tmp->line, tmp->col, "unexpected symbol.");
				break;
			}
			
			break;
		case P_ARRAY_GET_SYM:
			
			if (tmp->type == T_SYM)
			{
				if ( str_cmp_cstr(tmp->dat_str[0],"]") )
				{
					
					/* step down list level */
					map[map_lvl] = NULL;
					map_lvl--;
					
					if (map_lvl == 0)
						md = P_EOF;
					
				}
				else if ( str_cmp_cstr(tmp->dat_str[0],","))
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
		case P_OPEN_MEDIA:
			nvar = new_var();
			nvar->type = V_LIST;
			vtmp = 0;
			
			if (tmp->type == T_NAME && str_cmp_cstr(tmp->dat_str[0],"img"))
				md = P_IMG_GETSTR;
			else if (tmp->type == T_NAME && str_cmp_cstr(tmp->dat_str[0],"snd"))
				md = P_SND_GETSTR;
			else
				vm_err(tmp->fn, tmp->line, tmp->col, "expected name \"img\" or \"snd\".");
			break;
		case P_IMG_GETSTR:
			/* for media, create a list of pointers to media lib.  */
			if (tmp->type == T_STR)
			{
				if (!vtmp)
				{
					nvar->dat_list = new_var();
					vtmp = nvar->dat_list;
				}
				else
				{
					vtmp->list_next = new_var();
					vtmp = vtmp->list_next;
				}
				
				if (md_lib != 0)
				{
					
					
					vtmp->type = V_INT;
				
					ml_add_img(md_lib, tmp->dat_str[0]);
					
					vtmp->dat_media = md_lib->last;
				}
				
			}
			else if (tmp->type == T_SYM && str_cmp_cstr(tmp->dat_str[0],"}"))
				md = P_EOF;
			else
				vm_err(tmp->fn, tmp->line, tmp->col,
					"expected str or closing bracket for media expression.");
			
			break;
		case P_SND_GETSTR:
			break;
		default:
			break;
		}
		
		tmp = tmp->next;
	}
	
	return nvar;
}
