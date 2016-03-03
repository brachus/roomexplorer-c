
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"
#include "rpg_parse_main.h"



struct obj_dat *parse_main(struct token *tokens)
{
	
	int md, ifcntr, ifstate,
		iflevel, hold, set_hold,
		script_type, didinit, didbody, didterm;
	
	
	
	md = P_OPEN;
	
	struct token l_expr;
	
	struct obj_dat dat;
	obj_dat_init(&dat);
	
	struct obj *cur_obj;
	struct var *cur_var;
	struct func *cur_func;
	
	struct token_l *tmp_tok;
	tmp_tok = tokens->tok_first;
	
	add_token(tokens, T_EOF, 0, 0, "");
	
	struct var *used_obj_names = new_var();
	struct var *used_predef_names = new_var();
	struct var *used_script_names = new_var();
	
	set_hold = 0;
	
	while (tmp != NULL)
	{
		
		switch (md)
		{
		case P_OPEN:
			if (tmp_tok->type == T_NAME)
			{
				if (token_nnames(tmp_tok) == 1)
				{
					add_obj(&dat);
					dat.last->type = str_cpy(&tmp_tok->dat_str[0]);
					md = P_GET_OBJ_NAME;
					
					free_var(used_predef_names);
					free_var(used_script_names);
					struct var *used_predef_names = new_var();
					struct var *used_script_names = new_var();
					
					didinit = 0;
					didbody = 0;
					didterm = 0;
				}
				else
					vm_err(	tmp_tok->fn,
							tmp_tok->line,
							tmp_tok->col,
						"only single names identifiers may be used for object type.");
			}
			else if (tmp_tok->type == T_EOF)
				break;
			else
				vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected name for object type.");
			break;
		case P_GET_OBJ_NAME:
			if (tmp_tok->type == T_NAME)
			{
				if (token_nnames(tmp_tok) == 1)
				{
					/* look for duplicate obj name. */
					if (var_search_str(used_obj_names, &tmp_tok->dat_str[0]) == 0)
					{
						var_add_str(used_obj_names, &tmp_tok->dat_str[0]);
						
						dat.last->name = str_cpy(&tmp_tok->dat_str[0]);
						
						md = P_GET_OBJ_OPEN_BRACE;
					}
					else
						vm_err(	tmp_tok->fn,
								tmp_tok->line,
								tmp_tok->col,
								"duplicate obj name.");
				}
				else
					vm_err(	tmp_tok->fn,
							tmp_tok->line,
							tmp_tok->col,
						"only single names identifiers may be used for object name.");
			}
			else
				vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected name for object name.");
			break;
		case P_GET_OBJ_OPEN_BRACE:
			if (		tmp_tok->type == T_SYM &&
						str_cmp_cstr(&tmp_tok->dat_str[0], "{")		)
				md = P_OPEN_PREDEF;
			else
				vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected opening brace for obj.");
			break;
		case P_OPEN_PREDEF:
			if (tmp_tok->type == T_SYM)
			{
				if (str_cmp_cstr(&tmp_tok->dat_str[0], "}"))
					md = P_OPEN; /* close obj */
				else
					vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected closing brace, name for script, OR predef.");
			}
			else if (tmp_tok->type == T_NAME)
			{
				/* look for var name of format '.xxx' */
				if (token_nnames(tmp_tok) == 2)
				{
					/* watch out for duplicate predef names */
					if (var_search_str(used_predef_names, &tmp_tok->dat_str[1]) == 0)
					{
						var_add_str(used_predef_names, &tmp_tok->dat_str[1]);
						
						md = P_GET_PREDEF_EQUALS;
						
						/* used_predef_names->last for current predef name. */
					}
					else
						vm_err(	tmp_tok->fn,
								tmp_tok->line,
								tmp_tok->col,
								"duplicate predef name.");
				}
				else if (token_nnames(tmp_tok) == 1)
				{
					/* if script name */
					if (	str_cmp_cstr(&tmp_tok->dat_str[0], "init")	)
					{
						
						script_type = S_INIT;
						if (didinit)
							vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
								"duplicate object script.");
						didinit = 1;
					}
					else if (	str_cmp_cstr(&tmp_tok->dat_str[0], "body")	)
					{
						script_type = S_BODY;
						if (didbody)
							vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
								"duplicate object script.");
						didbody = 1;
					}
					else if (	str_cmp_cstr(&tmp_tok->dat_str[0], "term")	)
					{
						script_type = S_TERM;
						if (didterm)
							vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
								"duplicate object script.");
						didterm = 1;
					}
					else
						vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
							"only \"init\", \"body\", or \"term\" scripts are supported.");
					
					md = P_GET_SCRIPT_OPEN_BRACE;
				}
				else
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
								"too many names.");
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected name or obj close.");
			break;
		case P_GET_PREDEF_EQUALS:
			if (tmp_tok->type == T_SYM)
			{
				if (str_cmp_cstr(&tmp_tok->dat_str[0], "="))
				{
					
					
					md = P_GET_PREDEF_LITERAL;
				}
			}
			break;
		default:
			break;
		}
		
		tmp_tok = tmp_tok->next;
	}
	
};
