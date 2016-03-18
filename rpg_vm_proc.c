

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

struct obj *match_obj_name();


struct asub_dat *new_asub_dat()
{
	struct asub_dat *n;
	n = malloc(sizeof(struct asub_dat));
	
	n->first=0;
	n->last=0;
	
	return n;
};

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
	
};


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


void vm_proc_step(struct asub_i *in, struct obj_dat *odat)
{
	
	int ret_val = PR_STEP;
	int no_step = 0;
	int ptype = O_NONE;
	
	/* make local copies of asub_i vars. */
	int 		 tmd = in->mode;
	struct func *tpc = in->pc;
	struct obj  *tob = in->ob;
	
	struct idnt *targs;
	
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
	
	targs = in->pc->args;
	
	switch(ptype)
	{
	case O_NONE:
		break;
	}
	
	return ret_val;
}
