
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_parse_main.h"


void var_init(struct var *in)
{
	in->type = V_NULL;
	
	str_init(&in->name);
	
	str_init(&in->dat_str);
	
	in->dat_int = 0;
	in->dat_float = 0;
	
	in->dat_list = NULL;
	in->list_next = NULL;
	in->next  = NULL;
	
}

void idnt_init(struct idnt *in)
{
	in->type = IDNT_NULL;
	in->idx = -1;
	
	str_init(&in->obj_name);
	str_init(&in->var_name);
	
	in->use_var = NULL;
	in->next = NULL;
}

void func_init(struct func *in)
{
	in->id = NULL;
	in->label = NULL;
	str_init(&in->label_name);
	in->obj_idx = NULL;
	str_init(&in->obj_name);
	in->next = NULL;
}

void obj_init(struct obj *in)
{
	str_init(&in->type);
	str_init(&in->name);
	
	in->vars = NULL;
	
	in->init = NULL;
	in->body = NULL;
	in->term = NULL;
}


void obj_dat_init(struct obj_dat *in)
{
	in->first = NULL;
	in->last = NULL;
}


void parse_main(struct obj_dat *in, struct token *tokens)
{
	
	
}
