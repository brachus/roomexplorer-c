

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"
#include "rpg_func_def.h"
#include "rpg_parse_main.h"
#include "rpg_vm_proc.h"


int var_get_int(struct var *in)
{
	if (!in)
		return 0;
	
	if (in->type == V_INT)
		return in->dat_int;
	else
		vm_err(	0,0,0,
			"expected type int from argument.");
}

void set_var_to_idnt(struct var *vtmp, struct idnt *itmp, struct var **regs)
{
	struct var *tmp;
		
	switch(itmp->type)
	{
	case IDNT_VAR:
		return;
	case IDNT_OBJVAR:
		if (!itmp->ob || !itmp->var_name)
			return;
		else
			tmp = get_var_from_varstr_obj(itmp->ob, itmp->var_name);
		
		/* add new var if non-existent */
		if (!tmp && itmp->ob != 0)
		{
			obj_add_new_var(itmp->ob);
			tmp = itmp->ob->vars->last;
			tmp->name = str_cpy(itmp->var_name);
		}
			
		
		if (tmp != 0)
			var_inplace_cpy(vtmp, tmp);
		
		return;
	case IDNT_OBJ:
		
		return;
	case IDNT_REG:
		if (regs[itmp->idx] != 0 && regs[itmp->idx]->type == V_INT &&
				vtmp->type == V_INT)
		{
			regs[itmp->idx]->dat_int = vtmp->dat_int;
		}
			
		else
		{
			free_var(regs[itmp->idx]);
			regs[itmp->idx] = var_cpy(vtmp); /* lets hope reg[x] is modified. */
		}
			
			
		return;
	default:
		return;
	}
}

struct var *get_var_from_varstr_obj(struct obj *otmp, struct str *vname)
{
	struct var *vtmp;
	
	vtmp = otmp->vars;
	
	
	while (vtmp != 0)
	{
		if (str_cmp(vtmp->name, vname))
			return vtmp;
			
		vtmp = vtmp->next;
	}
	
	return 0;
};

struct str *get_str_from_arg(struct idnt *arg, struct var **regs)
{
	struct var *vtmp = get_var_from_arg(arg, regs);
	
	if (!vtmp)
		vm_err(	0,0,0,
			"bad var.");
	
	if (vtmp->type != V_STR)
		vm_err(	0,0,0,
			"expected type str from argument.");
	
	return vtmp->dat_str;
};

int get_int_from_arg(struct idnt *arg, struct var **regs)
{
	struct var *vtmp = get_var_from_arg(arg, regs);
	
	if (!vtmp)
		vm_err(	0,0,0,
			"bad var.");
		
	if (vtmp->type != V_INT)
		vm_err(	0,0,0,
			"expected type int from argument.");
	
	return vtmp->dat_int;
};

float get_float_from_arg(struct idnt *arg, struct var **regs)
{
	struct var *vtmp = get_var_from_arg(arg, regs);
	
	if (!vtmp)
		vm_err(	0,0,0,
			"bad var.");
		
	if (vtmp->type != V_FLOAT)
		vm_err(	0,0,0,
			"expected type float from argument.");
	
	return vtmp->dat_int;
};

struct obj *get_objpntr_from_arg(struct idnt *arg)
{
	if (!arg)
		vm_err(	0,0,0,
			"bad argument.");
	
	if (arg->type != IDNT_OBJ)
		vm_err(	0,0,0,
			"expected obj name from argument.");
	else
		return arg->ob;
};

struct var *get_var_from_arg(struct idnt *arg, struct var **regs)
{
	if (!arg)
		vm_err(	0,0,0, "bad argument.");
	
	switch(arg->type)
	{
	case IDNT_VAR:
		return arg->use_var;
	case IDNT_OBJVAR:
		if (!arg->ob || !arg->var_name)
			return 0;
		else
			return get_var_from_varstr_obj(arg->ob, arg->var_name);
	case IDNT_OBJ:
		return arg->use_var;
	case IDNT_REG:
		return regs[arg->idx];
	default:
		return 0;
	}
};

int get_nargs(struct idnt *args)
{
	register int n = 0;
		
	while (args != 0)
	{
		args = args->next;
		n++;
	}
	
	return n;
}

struct var **init_regs()
{
	struct var **n = malloc(sizeof(struct var *) * 8);
	int i;
	
	for (i=0;i<8;i++)
		n[i] = 0;
		
	return n;
}

struct asub_dat *new_asub_dat()
{
	struct asub_dat *n;
	n = malloc(sizeof(struct asub_dat));
	
	n->first=0;
	n->last=0;
	
	return n;
}

void add_asub_i(struct asub_dat *in, struct obj *obj_in)
{
	if (!in->first)
	{
		in->first = malloc(sizeof (struct asub_i));
		in->last = in->first;
	}
	else
	{
		in->last->next = malloc(sizeof (struct asub_i));
		in->last = in->last->next;
	}
	
	in->last->mode = S_INIT;
	in->last->ob = obj_in;
	in->last->pc = obj_in->init;
	in->last->next = 0;
	in->last->timer = 0;
	
};

void rm_asub_i(struct asub_dat *in, struct asub_i *s)
{
	struct asub_i *tmp = 0;
	struct asub_i *prev = 0;
	
	tmp = in->first;
	
	while (tmp!=0)
	{
		if (tmp == s)
		{
			if (!prev)
				in->first = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp);
			break;
		}
			
		prev = tmp;
		tmp = tmp->next;
	}
	
	tmp = in->first;
	while (tmp!=0)
	{
		if (!tmp->next)
			in->last;
		tmp = tmp->next;
	}
	
	return;
}


void add_asub_main(struct asub_dat *in, struct obj_dat *objd_in)
{
	struct obj *otmp;
	struct obj *ofnd;
	
	otmp = objd_in->first;
	
	while (otmp != 0)
	{
		if (str_cmp_cstr(otmp->name, "main") && str_cmp_cstr(otmp->type, "game"))
		{
			ofnd = otmp;
			break;
		}
		otmp = otmp->next;
	}
	
	if (!ofnd)
		vm_err(0,0,0, "no main object.");
	
	
	add_asub_i(in, ofnd);	

}

void force_no_type(struct var *a, int notype)
{
	if (a->type == notype)
		vm_err(0,0,0, "invalid type for arg.");
}

int var_is_type(struct var *a, int type)
{
	if (!a)
		return 0;
	if (a->type == type)
		return 1;
	else
		return 0;
}

void do_op(char op, struct var *a, struct var *b, struct var *c)
{
	if (a->type == V_STR || b->type == V_STR)
	{
		if (op == '+')
		{
			if (c->dat_str != 0)
				free_str(c->dat_str);
			c->dat_str = create_str();
			str_append_var(c->dat_str, a);
			str_append_var(c->dat_str, b);
			
			c->type = V_STR;
		}
		else
			vm_err(0,0,0, "strings only support addition.");
		
	}
	else if (a->type == V_FLOAT || b->type == V_FLOAT)
	{
		if (a->type != V_FLOAT)
			a->dat_float = a->dat_int;
		if (b->type != V_FLOAT)
			b->dat_float = b->dat_int;
		switch (op)
		{
		case '+':c->dat_float = a->dat_float + b->dat_float;break;
		case '-':c->dat_float = a->dat_float - b->dat_float;break;
		case '*':c->dat_float = a->dat_float * b->dat_float;break;
		case '/':
			if (b->dat_float == 0.0)
				vm_err(0,0,0, "division by zero.");
			c->dat_float = a->dat_float / b->dat_float;
			break;
		case '%':
			vm_err(0,0,0, "float can't do modulus.");
		default:vm_err(0,0,0, "invalid op.");
		}
		c->type = V_FLOAT;
	}
	else
	{
		switch (op)
		{
		case '+':c->dat_int = a->dat_int + b->dat_int;break;
		case '-':c->dat_int = a->dat_int - b->dat_int;break;
		case '*':c->dat_int = a->dat_int * b->dat_int;break;
		case '/':
			if (b->dat_int == 0)
				vm_err(0,0,0, "division by zero.");
			c->dat_int = a->dat_int / b->dat_int;
			break;
		case '%':
			if (b->dat_int == 0)
				vm_err(0,0,0, "division by zero.");
			c->dat_int = a->dat_int % b->dat_int;
			break;
		default:vm_err(0,0,0, "invalid op.");
		}
		c->type = V_INT;
	}
}

int do_int_cmp(int cmp_op, int a, int b)
{
	switch (cmp_op)
	{
	case F_CMP_EQUAL:
		if (a == b)
			return 1;
		break;
	case F_CMP_NEQUAL:
		if (a != b)
			return 1;
		break;
	case F_CMP_GREATER:
		if (a > b)
			return 1;
		break;
	case F_CMP_LESSER:
		if (a < b)
			return 1;
		break;
	case F_CMP_LESSEQUAL:
		if (a <= b)
			return 1;
		break;
	case F_CMP_GREATEQUAL:
		if (a >= b)
			return 1;
		break;
	}
	return 0;
}




int vm_proc_step(struct asub_i *in, struct obj_dat *odat, struct var **regs)
{
	
	int ret_val = PR_STEP;
	int nostep = 0;
	int ptype = O_NONE;
	
	
	
	/* make local copies of asub_i vars. */
	int 		 tmd = in->mode;
	struct func *tpc = in->pc;
	struct obj  *tob = in->ob;
	
	static struct var *tvars[16];
	static struct idnt *targs[16];
	int nargs, i;
	
	struct idnt *tidnt;
	
	
	struct var *retdat = 0;
	static struct var *retmydat;
	
	if (!retmydat)
		retmydat = new_var();
		
	int doret = 0;
	
	struct str *tstr;
	
	
	
	
	if (!tpc)
		switch(tmd)
		{
		case S_INIT:
			in->mode = S_BODY;
			in->pc = tob->body;
			return PR_STEP;
		case S_BODY:
			in->mode = S_TERM;
			in->pc = tob->term;
			return PR_STEP;
		case S_TERM: default:
			return PR_TERM;
		}
		
	
	if (tpc->id == F_LABEL)
	{
		in->pc = in->pc->next;
		return ret_val;
	}
	
	/* if current call has an opperand. */
	if (in->pc->ob != 0)
		ptype = in->pc->ob->itype;
	
	nargs = get_nargs(in->pc->args);
	
	tidnt = in->pc->args;
	
	for (i=0;i<nargs;i++)
	{
		targs[i] = tidnt;
		tidnt = tidnt->next;
	}
	
	
	
	switch(ptype)
	{
	case O_NONE:
		switch (in->pc->id)
		{
		case F_CMP_LESSEQUAL:
		case F_CMP_GREATEQUAL:
		case F_CMP_LESSER:
		case F_CMP_GREATER:
		case F_CMP_NEQUAL:
		case F_CMP_EQUAL:
			doret = 1;
			if (nargs > 0)
			{
				tvars[0] = get_var_from_arg(targs[0], regs);
				tvars[1] = get_var_from_arg(targs[1], regs);
			}
			
			
			retmydat->type = V_INT;
			
			
			/* ONLY INTS FOR NOW: */
			if (tvars[0]->type == V_INT && tvars[1]->type == V_INT)
			{
				
				retmydat->dat_int = do_int_cmp(
					in->pc->id,
					tvars[0]->dat_int,
					tvars[1]->dat_int);
				

			}
				
			break;
		case F_JMP:
			in->pc = in->pc->lbl;
			nostep = 1;
			break;
		case F_IF_JMP:
			if (nargs > 0)
				tvars[0] = get_var_from_arg(targs[0], regs);
			
			
			if (var_get_int(tvars[0]))
				in->pc = in->pc->lbl;
			
			break;
		case F_SET:
			doret = 1;
			retdat = get_var_from_arg(targs[0], regs);
			break;
		case F_PRINT:
		case F_PRINTLN:
			if (nargs > 0)
			{
				tvars[0] = get_var_from_arg(targs[0], regs);
				print_var_val(tvars[0]);
			}
			
			if (in->pc->id == F_PRINTLN)
				printf("\n");
			
			break;
		case F_OP_ADD:
		case F_OP_SUB:
		case F_OP_MUL:
		case F_OP_DIV:
		case F_OP_MOD:
			doret = 1;
			if (nargs == 2)
			{
				tvars[0] = get_var_from_arg(targs[0], regs);
				tvars[1] = get_var_from_arg(targs[1], regs);
			}
			else
				vm_err(0,0,0, "wrong number of args.");
			
			
			force_no_type(tvars[0], V_LIST);
			force_no_type(tvars[0], V_NULL);
			force_no_type(tvars[0], V_NAME);
			force_no_type(tvars[1], V_LIST);
			force_no_type(tvars[1], V_NULL);
			force_no_type(tvars[1], V_NAME);
			
			
			do_op(
				( (in->pc->id == F_OP_ADD) ? '+' :
				  (in->pc->id == F_OP_SUB) ? '-' :
				  (in->pc->id == F_OP_MUL) ? '*' :
				  (in->pc->id == F_OP_DIV) ? '/' :
					'%'
				  ),
				tvars[0], 
				tvars[1], 
				retmydat);
			
			
			break;
		case F_TYPE:
			doret = 1;
			if (nargs == 1)
			{
				tvars[0] = get_var_from_arg(targs[0], regs);
				
				if (tvars[0] != 0)
				{
					retmydat->type = V_STR;
					free_str(retmydat->dat_str);
					retmydat->dat_str = create_str();
					
					switch(tvars[0]->type)
					{
					case V_INT:
						str_append_cstr(retmydat->dat_str, "int");
						break;
					case V_FLOAT:
						str_append_cstr(retmydat->dat_str, "float");
						break;
					case V_STR:
						str_append_cstr(retmydat->dat_str, "string");
						break;
					case V_LIST:
						str_append_cstr(retmydat->dat_str, "list");
						break;
					default:
						str_append_cstr(retmydat->dat_str, "null");
						break;
					}
				}
			}
			break;
		
		case F_TERM_VM:
			return PR_TERMVM;
		
		case F_TERM:
			return PR_TERM;
		
		
		default:
			vm_err(0,0,0, "unrecognized function.");
		}
		break;
	}
	
	/* copy retdat to return identifier. */
	if (in->pc->ret != 0 && doret)
	{
		if (retdat)
			set_var_to_idnt(retdat, in->pc->ret, regs);
		else
			set_var_to_idnt(retmydat, in->pc->ret, regs);
			
	}
		
	
	if (!nostep)
		in->pc = in->pc->next;
	
	free_var(retmydat->dat_list);
	
	if (!in->pc)
	{
		free_var(retmydat);
		retmydat = 0;
	}
		
	
	return ret_val;
}



void vm_proc_full(struct asub_dat *in, struct obj_dat *odat, struct var **regs)
{
	struct asub_i *stmp, *prev;
	int step, rval;
	
	stmp = in->first;
	
	while (stmp != 0)
	{
		step = 1;
		while (step)
		{
			if (stmp->timer > 0)
			{
				stmp->timer--;
				step = 0;
			}
			else
			{
				rval = vm_proc_step(stmp, odat, regs);
				
				switch(rval)
				{
				case PR_TERM:
					rm_asub_i(in, stmp);
					stmp = prev;
					step = 0;
					break;
				case PR_TERMVM:
					return;
				case PR_NEXTSUB:
					step = 0;
					break;
				}
			}
		}
		
		if (!stmp)
			break;
			
		prev = stmp;
		stmp = stmp->next;
		
	}
}
