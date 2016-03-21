

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
		if (tmp != 0)
			var_inplace_cpy(vtmp, tmp);
		
		return;
	case IDNT_OBJ:
		return;
	case IDNT_REG:
		free_var(regs[itmp->idx]);
		regs[itmp->idx] = var_cpy(vtmp); /* lets hope reg[x] is modified. */
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
		vm_err(	0,0,0,
			"bad argument.");
	
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
		return 0;
	case IDNT_REG:
		return regs[arg->idx];
	default:
		return 0;
	}
};

int get_nargs(struct idnt *args)
{
	int n = 0;
	struct idnt *tmp = args;
	
	while (tmp != 0)
	{
		tmp = tmp->next;
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
	
	struct var *retdat;
	
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
	
	/* FIXME: start coding other elementary 
	 * functions, like set, op_add, etc.
	 */
	
	switch(ptype)
	{
	case O_NONE:
		switch (in->pc->id)
		{
		case F_SET:
			retdat = get_var_from_arg(targs[0], regs);
			break;
		case F_PRINTLN:
			if (nargs > 0)
			{
				tvars[0] = get_var_from_arg(targs[0], regs);
				print_var_val(tvars[0]);
			}
			
			printf("\n");
			
			break;
		default:
			goto fail;
		}
		break;
	}
	/* copy retdat to return identifier. */
	if (in->pc->ret != 0 && retdat != 0)
		set_var_to_idnt(retdat, in->pc->ret, regs);
	
	if (!nostep)
		in->pc = in->pc->next;
	
	
	return ret_val;
	
	
  fail:
	vm_err(0,0,0, "unrecognized function.");
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
		
		prev = stmp;
		stmp = stmp->next;
	}
}
