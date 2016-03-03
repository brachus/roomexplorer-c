
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"

/* TODO:
 * 	 functions for freeing var, obj, func, and
 *   idnt structs. compare to free(p)
 */


void var_init(struct var *in)
{
	in->type = V_NULL;
	
	in->name = NULL;
	
	in->dat_str = NULL;
	in->dat_str_1 = NULL;
	in->dat_str_2 = NULL;
	
	in->dat_int = 0;
	in->dat_float = 0;
	
	in->dat_list = NULL;
	in->list_next = NULL;
	in->next  = NULL;
	in->last = NULL;
	
}

struct var *new_var(void)
{
	struct var *n;
	
	n = malloc(sizeof(struct var));
	
	var_init(n);
	
	return n;
}

void free_var(struct var *in)
{
	if (in->name != NULL)
		free_str(in->name);
	
	if (in->dat_str != NULL)
		free_str(in->dat_str);
	
	if (in->dat_str_1 != NULL)
		free_str(in->dat_str_1);
		
	if (in->dat_str_2 != NULL)
		free_str(in->dat_str_2);
	
	if (in->dat_list != NULL)
		free_var(in->dat_list);
	
	if (in->list_next != NULL)
		free_var(in->list_next);
	
	if (in->next != NULL)
	{
		free_var(in->next);
		in->last = NULL;
	}
	
	free(in);
}

/* search through dat_str values among linked vars to find match */
int var_search_str(struct var *vars, struct str *find)
{
	struct var *tmp;
	tmp = vars;
	
	while (tmp != NULL)
	{
		if (tmp->dat_str != NULL)
			if (str_cmp(tmp->dat_str,find))
				return 1;
		tmp = tmp->next;
	}
	
	return 0;
}

void var_add_str(struct var *vars, struct str *add)
{
	add_var(vars);
	
	vars->last->dat_str = str_cpy(add);
}

/* add var to a chain of vars */
void add_var(struct var *vars)
{

	vars->last->next = new_var();
	vars->last = vars->last->next;
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
	if (!in->name || in->name->length == 0)
		printf("<noname>");
	else
		str_print(in->name);
	
	printf(":\t");
	
	print_var_val(in);
	
	printf("\n");
}



void idnt_init(struct idnt *in)
{
	in->type = IDNT_NULL;
	in->idx = -1;
	
	in->obj_name = NULL;
	in->var_name = NULL;
	
	in->use_var = NULL;
	in->next = NULL;
	in->last = in;
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
	in->label_name = NULL;
	in->obj_idx = -1;
	in->obj_name = NULL;
	in->next = NULL;
	in->last = in;
	
}

struct func *new_func(void)
{
	struct func *n;
	
	n = malloc(sizeof(struct func));
	
	func_init(n);
	
	return n;
}

void obj_init(struct obj *in)
{
	in->type = NULL;
	in->name = NULL;
	
	in->vars = NULL;
	
	in->init = NULL;
	in->body = NULL;
	in->term = NULL;
	
	in->next = NULL;
	
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

void add_obj(struct obj_dat *in)
{
	if (in->first == NULL)
	{
		in->first = new_obj();
		in->last = in->first;
	}
	else
	{
		in->last->next = new_obj();
		in->last = in->last->next;
	}
}

void obj_add_var(struct obj *in)
{
	if (in->vars == NULL)
	{
		in->vars = new_var();
	}
	else
	{
		in->vars->last->next = new_var();
		in->vars->last = in->vars->last->next;
	}
}
