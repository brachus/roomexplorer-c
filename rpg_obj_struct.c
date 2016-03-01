
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"


void var_init(struct var *in)
{
	in->type = V_NULL;
	
	in->name = create_str();
	
	in->dat_str = NULL;
	in->dat_str_1 = NULL;
	in->dat_str_2 = NULL;
	
	in->dat_int = 0;
	in->dat_float = 0;
	
	in->dat_list = NULL;
	in->list_next = NULL;
	in->next  = NULL;
	
}

struct var *new_var(void)
{
	struct var *n;
	
	n = malloc(sizeof(struct var));
	
	var_init(n);
	
	return n;
}

void print_var_val(struct var *in)
{
	switch(in->type)
	{
	case V_INT:
		printf("%d", in->dat_int);
		break;
	case V_FLOAT:
		printf("%f", in->dat_float);
		break;
	case V_STR:
		printf("\"");
		str_print(in->dat_str);
		printf("\"");
		break;
	case V_NAME:
		str_print(in->dat_str);
		if (in->dat_str_1 != NULL)
			str_print(in->dat_str_1);
		if (in->dat_str_2 != NULL)
			str_print(in->dat_str_2);
		break;
	case V_LIST:
		printf("[");
		if (in->dat_list != NULL)
			print_var_val(in->dat_list);
		printf("]");
		break;
	case V_NULL:
		printf("null");
		break;
	default:
		break;
	}
	
	if (in->list_next != NULL)
	{
		printf(",");
		print_var_val(in->list_next);
	}
}

void print_var(struct var *in)
{
	str_print(in->name);
	if (in->name->length == 0)
		printf("<noname>");
		
	printf(":\t");
	
	print_var_val(in);
	
	printf("\n");
}



void idnt_init(struct idnt *in)
{
	in->type = IDNT_NULL;
	in->idx = -1;
	
	in->obj_name = create_str();
	in->var_name = create_str();
	
	in->use_var = NULL;
	in->next = NULL;
}

struct idnt *new_idnt(void)
{
	struct idnt *n;
	
	n = malloc(sizeof(struct idnt));
	
	idnt_init(n);
	
	return n;
}

void func_init(struct func *in)
{
	in->id = -1;
	in->label = -1;
	in->label_name = create_str();
	in->obj_idx = -1;
	in->obj_name = create_str();
	in->next = NULL;
	
}

struct func *new_ifunc(void)
{
	struct func *n;
	
	n = malloc(sizeof(struct func));
	
	func_init(n);
	
	return n;
}

void obj_init(struct obj *in)
{
	in->type = create_str();
	in->name = create_str();
	
	in->vars = NULL;
	
	in->init = NULL;
	in->body = NULL;
	in->term = NULL;
	
}

struct obj *new_obj(void)
{
	struct obj *n;
	
	n = malloc(sizeof(struct obj));
	
	obj_init(n);
	
	return n;
}

void obj_dat_init(struct obj_dat *in)
{
	in->first = NULL;
	in->last = NULL;
}

