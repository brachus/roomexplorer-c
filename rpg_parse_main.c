
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"
#include "rpg_parse_main.h"
#include "rpg_func_def.h"


struct func *get_label_pntr(struct func *fns, struct str *lname)
{
	while (fns != 0)
	{
		if (fns->id == F_LABEL && fns->label_name != 0)
			if (str_cmp(lname,fns->label_name))
				return fns;
		fns = fns->next;
	}
	return 0;
};

int match_sname(struct str *in)
{
	if 	(	str_cmp_cstr(in, "init")	)
		return S_INIT;
	else if  (	str_cmp_cstr(in, "body")	)
		return S_BODY;
	else if  (	str_cmp_cstr(in, "term")	)
		return S_TERM;
	else
		return 0;
}

struct ifdat *new_ifdat()
{
	struct ifdat *new = malloc(sizeof (struct ifdat));
	
	new->first = 0;
	new->last = 0;
	new->if_cntr = 0;
	new->if_level = -1;
	
	return new;
};

struct ifdat_l *new_ifdat_l()
{
	struct ifdat_l *new = malloc(sizeof(struct ifdat_l));
	
	new->id	  = 0;
	new->mode = 0;
	new->branch_cntr = 0;
	new->next = 0;
	new->prev = 0;
	
	return new;
};

int if_get_last_mode(struct ifdat *in)
{
	if (!in->last)
		return IF_NULL;
	else
		return in->last->mode;
}

void if_set_last_mode(struct ifdat *in, int md)
{
	if (in->last != 0)
		in->last->mode = md;
}

int if_get_last_id(struct ifdat *in)
{
	if (!in->last)
		return -1;
	else
		return in->last->id;
}

int if_get_last_bcntr(struct ifdat *in)
{
	if (!in->last)
		return -1;
	else
		return in->last->branch_cntr;
}

void if_set_last_id(struct ifdat *in, int id)
{
	if (in->last != 0)
		in->last->id = id;
}

void if_set_last_bcntr(struct ifdat *in, int bcntr)
{
	if (in->last != 0)
		in->last->branch_cntr = bcntr;
}

void if_step_last_bcntr(struct ifdat *in)
{
	if (in->last != 0)
		in->last->branch_cntr++;
}

struct str *create_jmp_label(int ifid, int branch_cntr,int doend)
{
	char *s = malloc(64);
	struct str *new = create_str();
	
	if (ifid >= 0 && branch_cntr >= 0)
		sprintf(s, "if%d_%d", ifid, branch_cntr);
	else if (ifid >= 0)
		sprintf(s, "if%d", ifid);
	else
		sprintf(s, "if");
	
	str_append_cstr(new, s);
	
	if (doend)
		str_append_cstr(new, "_end");
	
	free(s);
	
	return new;
};

void ifdat_pop(struct ifdat *dat)
{
	struct ifdat_l *tmp;
	if (dat->last != 0)
	{
		if (dat->last->prev != 0)
		{
			tmp = dat->last;
			
			dat->last = tmp->prev;
			
			dat->last->next = 0;
			
			free(tmp);
		}
		else
		{
			free(dat->last);
			dat->first = 0;
			dat->last = 0;
		}
		
		dat->if_level--;
	}
}

void add_if(struct ifdat *dat, int md)
{
	dat->if_cntr++;
	
	
	if (!dat->first)
	{
		dat->first = new_ifdat_l();
		dat->last = dat->first;
	}
	else
	{
		dat->last->next = new_ifdat_l();
		dat->last->next->prev = dat->last;
		dat->last = dat->last->next;
	}
	
	dat->last->id = dat->if_cntr;
	dat->last->branch_cntr = 0;
	dat->last->mode = md;
	
	dat->if_level++;
	
};

void obj_add_label(struct obj_dat *dat, struct str *labelstr, int stype)
{
	obj_add_func(dat->last, create_func_label(labelstr), stype);
}

void obj_add_ifjmp(struct obj_dat *dat, int regn, struct str *labelstr, int stype)
{
	obj_add_func(dat->last, create_func_jmp(F_IF_JMP, labelstr, regn), stype);
}

void obj_add_jmp(struct obj_dat *dat, struct str *labelstr, int stype)
{
	obj_add_func(dat->last, create_func_jmp(F_JMP, labelstr, -1), stype);
}



struct func *obj_get_last_func(struct obj *in, int stype)
{
	if (stype == S_INIT && in->init != 0)
		return in->init->last;
	else if (stype == S_BODY && in->body != 0)
		return in->body->last;
	else if (stype == S_TERM && in->term != 0)
		return in->term->last;
	
	return 0;
}

void func_add_arg(struct func *in, struct idnt *addme)
{
	if (!in->args)
	{
		in->args = addme;
		in->args->last = in->args;
	}
	else
	{
		in->args->last->next = addme;
		in->args->last = in->args->last->next;
	}
}

int obj_find_label(struct obj *in, struct str *findme, int stype)
{
	struct func *tmp;
	int tidx = 0;
	
	if (stype == S_INIT)
		tmp = in->init;
	else if (stype == S_BODY)
		tmp = in->body;
	else if (stype == S_TERM)
		tmp = in->term;
	
	
	
	while (tmp != 0)
	{
		if (tmp->id == F_LABEL)
			if (  str_cmp(tmp->label_name, findme)  )
				return tidx;
		
		tmp = tmp->next;
		tidx++;
	}
	
	return -1;
}

/* go through each jmp/ifjmp function and take label_name
 * and find label idx; idx of where to jump to within
 * a script.
 */
void obj_jmp_add_label_idx(struct obj *in, int stype)
{
	struct func *tmp;
	struct func *strt;
	
	if (stype == S_INIT)
		strt = in->init;
	else if (stype == S_BODY)
		strt = in->body;
	else if (stype == S_TERM)
		strt = in->term;
		
	tmp = strt;
	
	while (tmp != 0)
	{
		if (tmp->id == F_JMP || tmp->id == F_IF_JMP)
		{
			if (tmp->label_name != 0)
			{
				tmp->label = obj_find_label(in, tmp->label_name, stype);
				tmp->lbl = get_label_pntr(strt, tmp->label_name);
			}
			else if (	tmp->id == F_JMP && 
						tmp->args != 0 && 
						tmp->args->type == IDNT_OBJ &&
						tmp->args->obj_name != 0)
			{
				
				tmp->lbl = get_label_pntr(strt, tmp->args->obj_name);
			}
			else if (	tmp->id == F_IF_JMP && 
						tmp->args != 0)
				{
					if (
							tmp->args->next != 0 && 
							tmp->args->next->type == IDNT_OBJ &&
							tmp->args->next->obj_name != 0)
						tmp->lbl = get_label_pntr(strt, tmp->args->next->obj_name);
				}
		else
			vm_err(0,0,0, "invalid arguments for jmp/if_jmp.");
		}
		
			
		
		tmp = tmp->next;
	}
}

void idnt_fill_idxs(struct obj_dat *odat, struct idnt *in)
{
	if (in->type == IDNT_OBJVAR || in->type == IDNT_OBJ)
	{
		in->idx = get_obj_idx(odat, in->obj_name);
		in->ob =  get_obj_pntr(odat, in->obj_name);
	}
	
	
	var_fill_idxs(odat, in->use_var);
				
	if (in->next != 0)
		idnt_fill_idxs(odat, in->next);
}



void func_fill_idxs(struct obj_dat *odat, struct func *in)
{
	if (in->obj_name != 0)
	{
		in->obj_idx = get_obj_idx(odat, in->obj_name);
		in->ob = get_obj_pntr(odat, in->obj_name);
	}
	
	
	if (in->ret != 0)
		idnt_fill_idxs(odat, in->ret);
	
	if (in->args != 0)
		idnt_fill_idxs(odat, in->args);
	
	/* move on to next */
	if (in->next != 0)
		func_fill_idxs(odat, in->next);
}

/* go through each variable.
 * if its of type name
 * or a list of names,
 * use get obj idx and 
 * set dat_int.
 */
void var_fill_idxs(struct obj_dat *odat, struct var *in)
{
	if (!in)
		return;
	
	if (in->type == V_NAME)
	{
		in->dat_int = get_obj_idx(odat, in->dat_str);
		in->ob = get_obj_pntr(odat, in->dat_str);
	}
	else if (in->type == V_LIST && in->dat_list != 0)
		var_fill_idxs(odat, in->dat_list);
	
	if (in->list_next != 0)
		var_fill_idxs(odat, in->list_next);
	
	/* move on to next */
	if (in->next != 0)
		var_fill_idxs(odat, in->next);
}

/* do something within each object */
void obj_do_each(struct obj_dat *in)
{
	struct obj *otmp = in->first;
	struct func *ftmp;
	
	while (otmp != 0)
	{
		/* go through each variable in vars */		
		if (otmp->vars != 0)
			var_fill_idxs(in, otmp->vars);
		
		/* FIXME add default vars */
		
		/* take type str and determine type enum */
		obj_do_itype(otmp);
		
		/* go through each function in each script */
		if (otmp->init != 0)
			func_fill_idxs(in, otmp->init);
		if (otmp->body != 0)
			func_fill_idxs(in, otmp->body);
		if (otmp->term != 0)
			func_fill_idxs(in, otmp->term);
		
		otmp = otmp->next;
	}
	
}

int get_obj_idx(struct obj_dat *in, struct str *obj_str)
{
	int tidx = 0;
	struct obj *tmp = in->first;
	
	while (tmp != 0)
	{
		if (  str_cmp(tmp->name, obj_str)  )
			return tidx;
		tmp = tmp->next;
		tidx++;
	}
	
	return -1;
}

struct obj *get_obj_pntr(struct obj_dat *in, struct str *obj_str)
{
	struct obj *tmp = in->first;
	
	
	if (!obj_str)
		return 0;
	
	while (tmp != 0)
	{
		if (  str_cmp(tmp->name, obj_str)  )
			return tmp;
		tmp = tmp->next;
	}
	
	return 0;
}

void obj_do_itype(struct obj *in)
{
	if (  str_cmp_cstr(in->type, "game")  )
		in->itype = O_GAME;
	else
		in->itype = O_NONE;
}

struct obj_dat parse_main(struct token *tokens)
{
	
	int md, hold, regn,
		stype, didsnames,
		dostmnt, blvl;
	
	md = P_OPEN;
	
	struct token l_expr;
	token_init(&l_expr);
	
	struct obj_dat dat;
	obj_dat_init(&dat);
	
	struct obj *cur_obj;
	struct var *cur_var;
	struct func *cur_func;
	
	
	
	add_token(tokens, T_EOF, 0, 0, "");
	
	struct var *used_obj_names = new_var();
	struct var *used_predef_names = new_var();
	struct var *used_script_names = new_var();
	
	struct token_l *tmp_tok;
	struct token_l *tmp_name = 0;
	
	struct ifdat *ifstate = new_ifdat();
	
	struct func *tmpfunc;
	
	hold = 0;
	
	tmp_tok = tokens->first;
	
	while (tmp_tok != 0)
	{		
		switch (md)
		{
		case P_OPEN:
			if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				add_obj(&dat);
				dat.last->type = str_cpy(tmp_tok->dat_str[0]);
				md = P_GET_OBJ_NAME;
				free_var(used_predef_names);
				free_var(used_script_names);
				used_predef_names = new_var();
				used_script_names = new_var();
				didsnames = 0;
			}
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) > 1)
				vm_err(	tmp_tok->fn, tmp_tok->line, tmp_tok->col,
					"only single names identifiers may be used for object type.");
			else if (tmp_tok->type == T_EOF)
				break;
			else
				vm_err(	tmp_tok->fn, tmp_tok->line, tmp_tok->col,
						"expected name for object type.");
			break;
		case P_GET_OBJ_NAME:
			if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				/* check for duplicate obj name. */
				if (var_search_str(used_obj_names, tmp_tok->dat_str[0]) == 0)
				{
					var_add_str(used_obj_names, tmp_tok->dat_str[0]);
					dat.last->name = str_cpy(tmp_tok->dat_str[0]);
					md = P_GET_OBJ_OPEN_BRACE;
				}
				else
					vm_err(	tmp_tok->fn, tmp_tok->line, tmp_tok->col,
						"duplicate obj name.");
				
			}
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) > 1)
				vm_err(	tmp_tok->fn, tmp_tok->line, tmp_tok->col,
					"only single names identifiers may be used for object name.");
			else
				vm_err(	tmp_tok->fn, tmp_tok->line, tmp_tok->col,
					"expected name for object name.");
			break;
		case P_GET_OBJ_OPEN_BRACE:
			if ( token_if_sym(tmp_tok, "{")	)
				md = P_OPEN_PREDEF;
			else
				vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected opening brace for obj.");
			break;
		case P_OPEN_PREDEF:
			if (token_if_sym(tmp_tok, "}"))
				md = P_OPEN; /* close obj */
				
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 2)
			{
				
				/* watch out for duplicate predef names */
				if (var_search_str(used_predef_names, tmp_tok->dat_str[1]) == 0)
				{
					var_add_str(used_predef_names, tmp_tok->dat_str[1]);
					
					md = P_GET_PREDEF_EQUALS;
					
					/* used_predef_names->last for current predef name. */
				}
				else
					vm_err(	tmp_tok->fn,
							tmp_tok->line,
							tmp_tok->col,
							"duplicate predef name.");
			}
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				stype = match_sname(tmp_tok->dat_str[0]);
				
				if (stype == 0)
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
						"only \"init\", \"body\", or \"term\" scripts are supported.");
				
				if (didsnames & stype)
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
							"duplicate object script.");
				
				didsnames |= stype;
					
				
				md = P_GET_SCRIPT_OPEN_BRACE;
			}
			else if  (tmp_tok->type == T_NAME && token_nnames(tmp_tok) > 2)
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
						"too many names.");
			else
				vm_err(	tmp_tok->fn,
						tmp_tok->line,
						tmp_tok->col,
						"expected closing brace, name for script, OR predef.");
			break;
		case P_GET_PREDEF_EQUALS:
			if (	token_if_sym(tmp_tok, "=")	)
			{
				free_tokens(&l_expr);
				
				md = P_GET_PREDEF_LITERAL;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
								"expected \"=\".");
			
			break;
			
		case P_GET_PREDEF_LITERAL:
			if (	token_if_sym(tmp_tok, ";")	)
			{
				obj_add_var(dat.last, parse_literal_expr(&l_expr));
				dat.last->vars->last->name = str_cpy(used_predef_names->last->dat_str);
				md = P_OPEN_PREDEF;
			}
			else if ( tmp_tok->type == T_EOF )
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected continued expression or \";\".");
			else
				add_cpy_token(&l_expr, tmp_tok);
			
			break;
			
		case P_GET_SCRIPT_OPEN_BRACE:
			if (	token_if_sym(tmp_tok, "{") )
				md =  P_SCRIPT_OPEN;
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
							"expected opening brace.");
			break;
			
		case P_SCRIPT_OPEN:
			dostmnt = 0;
			
			
						
			if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 2)
			{
				free_token_l(tmp_name);
				tmp_name = cpy_token(tmp_tok);
				
				/* if .xxx */
				if (tmp_name->dat_str[0]->length == 0)
				{
					free_str(tmp_name->dat_str[0]);
					tmp_name->dat_str[0] = str_cpy(dat.last->name);
				}
				
				md = P_SCRIPT_GET_PARA_EQUALS;
				dostmnt = 1;
			}
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				free_token_l(tmp_name);
				tmp_name = cpy_token(tmp_tok);
				
				md = P_SCRIPT_GET_PARA_COLON;
				
				dostmnt = 1;
				
				if (parse_ret_reg(tmp_tok->dat_str[0]) != -1)
					md = P_SCRIPT_GET_EQUALS;
				else if (str_cmp_cstr(tmp_name -> dat_str[0],"if"))
				{
					if (	ifstate->if_level == -1 ||
							if_get_last_mode(ifstate) == IF_IN_BRANCH ||
							if_get_last_mode(ifstate) == IF_IN_ELSEBRANCH)
					{
						add_if(ifstate, IF_GET_REG);
						
						
						md = P_SCRIPT_GET_IF_REG;
						dostmnt = 0;
					}
					else if (if_get_last_mode(ifstate) == IF_OPEN)
					{
						obj_add_label(&dat,
							create_jmp_label(
								if_get_last_id(ifstate),
								-1,
								1 ),
							stype);
							
						ifstate->if_cntr++;
						
						if_set_last_mode(ifstate, IF_GET_REG);
						if_set_last_bcntr(ifstate, 0);
						if_set_last_id(ifstate, ifstate->if_cntr);
						
						md = P_SCRIPT_GET_IF_REG;
						dostmnt = 0;
					}
				}
				else if (str_cmp_cstr(tmp_name -> dat_str[0],"else"))
				{
					if (	ifstate->if_level == -1 ||
							if_get_last_mode(ifstate) == IF_IN_BRANCH ||
							if_get_last_mode(ifstate) == IF_IN_ELSEBRANCH  )
						vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
							"unexpected else branch.");
					else if (if_get_last_mode(ifstate) == IF_OPEN)
					{
						if_set_last_mode(ifstate, IF_GET_ELSE_IF);
						if_step_last_bcntr(ifstate);
						md = P_SCRIPT_GET_ELSE_IF;
					}
				}
			}
			else if ( token_if_sym(tmp_tok, "}") )
			{
				/* FIXME*/
				if (ifstate->if_level == -1)
				{
					obj_jmp_add_label_idx(dat.last, stype);
					
					/* close the script block */
					md = P_OPEN_PREDEF;
				}
					
				else if (	if_get_last_mode(ifstate) == IF_IN_BRANCH ||
							if_get_last_mode(ifstate) == IF_IN_ELSEBRANCH )
				{
					/* jump out of branch to end of conditional block. */
					obj_add_jmp(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							-1,
							1 ),
						stype);
					
					/* mark end of branch */
					obj_add_label(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							if_get_last_bcntr(ifstate),
							1 ),
						stype);
					
					if_set_last_mode(ifstate, IF_OPEN);
					
					/* INVESTIGATE ln 402 in "rpg_parse_main_with_branching.js" */
					
					md = P_SCRIPT_OPEN;
				}
				else if (if_get_last_mode(ifstate) == IF_OPEN)
				{
					/* close conditional block, and a branch underneath if it
					 * exists.
					 */
					obj_add_label(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							-1,
							1 ),
						stype);
						
					ifdat_pop(ifstate);
										
					if (ifstate->if_level >= 0)
					{
						
						
						/* jump out of branch to end of conditional block. */
						obj_add_jmp(&dat,
							create_jmp_label(
								if_get_last_id(ifstate),
								-1,
								1 ),
							stype);
							
						/* mark end of branch */
						obj_add_label(&dat,
							create_jmp_label(
								if_get_last_id(ifstate),
								if_get_last_bcntr(ifstate),
								1 ),
							stype);
						
						if_set_last_mode(ifstate, IF_OPEN);
						md = P_SCRIPT_OPEN;
						
					}
					else
					{
						obj_jmp_add_label_idx(dat.last, stype);
						
						md = P_OPEN_PREDEF;
					}
						
				}
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected script content OR ending brace.");
			
			if (	ifstate->if_level >= 0 &&
					if_get_last_mode(ifstate) == IF_OPEN &&
					dostmnt  )
			{
				obj_add_label(&dat,
					create_jmp_label(
						if_get_last_id(ifstate),
						-1,
						1 ),
					stype);
					
				ifdat_pop(ifstate);
			}
			
			break;
		case P_SCRIPT_GET_IF_REG:
			if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				regn = parse_ret_reg(tmp_tok->dat_str[0]);
				if (regn >= 0)
				{
					/* setup ifjmp func */
					obj_add_ifjmp(&dat,
						regn,
						create_jmp_label(
							if_get_last_id(ifstate),
							if_get_last_bcntr(ifstate),
							0 ),
						stype);
						
					/* setup jmp for else case */
					obj_add_jmp(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							if_get_last_bcntr(ifstate),
							1 ),
						stype);
					
					/* open branch */
					obj_add_label(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							if_get_last_bcntr(ifstate),
							0 ),
						stype);
					
					if_set_last_mode(ifstate, IF_IN_BRANCH);
					
					/*printf("get_if: %d = %d ?\n", IF_IN_BRANCH, if_get_last_mode(ifstate));*/
					
					md = P_SCRIPT_GET_IF_BRANCH_OPEN;
					
				}
				else
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
						"expected valid register.");
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected register.");
			break;
			
		case P_SCRIPT_GET_ELSE_IF:
			if (tmp_tok->type == T_NAME &&
					token_nnames(tmp_tok) == 1 &&
					str_cmp_cstr(tmp_name -> dat_str[0],"if") )
				md = P_SCRIPT_GET_IF_REG;
			else if ( token_if_sym(tmp_tok, "{") )
			{
				if_set_last_mode(ifstate, IF_IN_ELSEBRANCH);
				md=P_SCRIPT_OPEN;
			}
			else
			{
				hold = 1;
				if_set_last_mode(ifstate, IF_IN_ELSEBRANCH_ONELINE);
				md=P_SCRIPT_OPEN;
			}
			
			break;
		
		case P_SCRIPT_GET_IF_BRANCH_OPEN:
			if (token_if_sym(tmp_tok, "{"))
				md = P_SCRIPT_OPEN;
			else
			{
				hold = 1;
				if_set_last_mode(ifstate, IF_IN_BRANCH_ONELINE);
				md = P_SCRIPT_OPEN;
			}
			break;
		
		case P_SCRIPT_GET_PARA_COLON:
			/*tmp_name will only have 1 name */
			
			if (token_if_sym(tmp_tok, "("))
			{
				
				obj_add_func(dat.last, new_func(), stype);
				tmpfunc = obj_get_last_func(dat.last, stype);
				tmpfunc->id = get_funcname_id(tmp_name->dat_str[0]);
				
				if (tmpfunc->id == F_UNKNOWN)
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
						"unknown function.");
				
				free_tokens(&l_expr);
				
				blvl = 0;
				
				md = P_SCRIPT_GET_ARG;
			}
			else if (token_if_sym(tmp_tok, ":"))
			{
				obj_add_label(&dat,
					tmp_name->dat_str[0],
					stype);
					
				md = P_SCRIPT_OPEN;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected \":\" or \"(\".");
			
			break;
		
		case P_SCRIPT_GET_PARA_EQUALS:
			/* assume tmp_name has two names */
			if (token_if_sym(tmp_tok, "("))
			{
				
				obj_add_func(dat.last, new_func(), stype);
				tmpfunc = obj_get_last_func(dat.last, stype);
				tmpfunc->id = get_funcname_id(tmp_name->dat_str[1]);
				
				if (tmpfunc->id == F_UNKNOWN)
					vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
						"unknown function.");
				
				tmpfunc->obj_name = str_cpy(tmp_name->dat_str[0]);
				
				free_tokens(&l_expr);
				
				blvl = 0;
				
				md = P_SCRIPT_GET_ARG;
			}
			else if (token_if_sym(tmp_tok, "="))
			{
				obj_add_func(dat.last, new_func(), stype);
				tmpfunc = obj_get_last_func(dat.last, stype);
				
				tmpfunc->ret = create_idnt_two_names(tmp_name->dat_str[0], tmp_name->dat_str[1]);
				
				md = P_SCRIPT_GET_FNAME;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected \"(\" or \"=\".");
			break;
		
		case P_SCRIPT_GET_EQUALS:
			/* assume tmp_name is register identifier */
			if (token_if_sym(tmp_tok, "="))
			{
				obj_add_func(dat.last, new_func(), stype);
				tmpfunc = obj_get_last_func(dat.last, stype);
				
				tmpfunc->ret = create_idnt_reg(
					parse_ret_reg(tmp_name->dat_str[0])
						);
				
				md = P_SCRIPT_GET_FNAME;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected \"=\".");
			
			break;
		
		case P_SCRIPT_GET_FNAME:
			if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 1)
			{
				tmpfunc->id = get_funcname_id(tmp_tok->dat_str[0]);
				md = P_SCRIPT_GET_OPEN_PARA;
			}
			else if (tmp_tok->type == T_NAME && token_nnames(tmp_tok) == 2)
			{
				tmpfunc->id = get_funcname_id(tmp_tok->dat_str[1]);
				
				if (tmp_tok->dat_str[0]->length == 0)
					tmpfunc->obj_name = str_cpy(dat.last->name);
				else
					tmpfunc->obj_name = str_cpy(tmp_tok->dat_str[0]);
					
				md = P_SCRIPT_GET_OPEN_PARA;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected single or double name.");
			
			break;
		
		case P_SCRIPT_GET_OPEN_PARA:
			if (token_if_sym(tmp_tok, "("))
			{
				free_tokens(&l_expr);
				
				blvl = 0;
				
				md = P_SCRIPT_GET_ARG;
			}
				
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected \"(\"."  );
			
			break;
		
		case P_SCRIPT_GET_ARG:
					
			if (tmp_tok->type == T_EOF)
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"unexpected EOF."  );
			else if (blvl == 0)
			{
				if ( token_if_sym(tmp_tok, ",") || token_if_sym(tmp_tok, ")") )
				{
					if (l_expr.first != 0)
					{
						func_add_arg(tmpfunc, parse_lexpr_idnt(&l_expr));
						
						/* ".xxx" --> "curobj.xxx" */
						if (tmpfunc->args->last->obj_name != 0 &&
							tmpfunc->args->last->obj_name->length == 0)
						{
							free_str(tmpfunc->args->last->obj_name);
							tmpfunc->args->last->obj_name =
								str_cpy(dat.last->name);
						}
							
					}
					
					free_tokens(&l_expr);
					
					if (token_if_sym(tmp_tok, ")"))
						md = P_SCRIPT_GET_SEMICOLON;
				}
				else  
				{
					if (token_if_sym(tmp_tok, "["))
						blvl++;
					add_cpy_token(&l_expr, tmp_tok);
				}
				
			}
			else
			{
				if (token_if_sym(tmp_tok, "]"))
					blvl--;
				add_cpy_token(&l_expr, tmp_tok);
			}
			
			break;
		
		case P_SCRIPT_GET_SEMICOLON:
			if (token_if_sym(tmp_tok, ";"))
			{
				if (ifstate->if_level >= 0 &&
						if_get_last_mode(ifstate) == IF_IN_BRANCH_ONELINE)
				{
					/* close branch */
					/* jump out of branch to end of conditional block. */
					obj_add_jmp(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							-1,
							1 ),
						stype);
					
					/* mark end of branch */
					obj_add_label(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							if_get_last_bcntr(ifstate),
							1 ),
						stype);
					
					if_set_last_mode(ifstate, IF_OPEN);
					
				}
				else if (ifstate->if_level >= 0 &&
					if_get_last_mode(ifstate) == IF_IN_ELSEBRANCH_ONELINE)
				{
					/* close cond. block */
					obj_add_label(&dat,
						create_jmp_label(
							if_get_last_id(ifstate),
							-1,
							1 ),
						stype);
					
					ifdat_pop(ifstate);
				}
				
				md = P_SCRIPT_OPEN;
			}
			else
				vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"expected \";\"."  );
					
			break;
		
		case P_ERR:
			vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
					"unspecified error."  );
			break;
			
		default:
			break;
		}
		
		if (hold == 0)
			tmp_tok = tmp_tok->next;
		else
			hold = 0;
	}
	
	if (md != P_OPEN)
		vm_err(	tmp_tok->fn,tmp_tok->line,tmp_tok->col,
			"unexpected EOF.");
	
	/* FIXME: make sure jmp/ifjmp references idx to label AND pointer to label.
	 * make sure functions all reference the next in a chain.
	 */
	/* FIXME: 
	 * 
	 */
	obj_do_each(&dat);
	
	
	return dat;
	
}
