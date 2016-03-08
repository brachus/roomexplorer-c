
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
	add_new_var(vars);
	
	vars->last->dat_str = str_cpy(add);
}

/* add existing var to a chain of vars.
 * this function doesnt copy the var;
 * it assumes var comes from parse_literal_expr or cpy_var. */
void add_var(struct var *vars, struct var *addme)
{

	vars->last->next = addme;
	vars->last = vars->last->next;
}

/* add var to a chain of vars */
void add_new_var(struct var *vars)
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
		if (in->dat_str_1 != 0)
			str_print(in->dat_str_1);
		if (in->dat_str_2 != 0)
			str_print(in->dat_str_2);
		break;
	case V_LIST:
		printf("[");
		if (in->dat_list != 0)
			print_var_val(in->dat_list);
		printf("]");
		break;
	case V_NULL:
		printf("null");
		break;
	default:
		break;
	}
	
	if (in->list_next != 0)
	{
		printf(",");
		print_var_val(in->list_next);
	}
}

void print_var(struct var *in, char *pad)
{	
	if (!in)
	{
		printf("<nothing>\n");
		return;
	}
	
	printf("%s", pad);
		
	if (!in->name || in->name->length == 0)
		printf("<noname>");
	else
		str_print(in->name);
	
	printf(":\t");
	
	print_var_val(in);
	
	printf("\n");
	
	if (in->next != 0)
		print_var(in->next, pad);
}

void idnt_init(struct idnt *in)
{
	in->type = IDNT_NULL;
	in->idx = -1;
	
	in->obj_name = 0;
	in->var_name = 0;
	
	in->use_var = 0;
	in->next = 0;
	in->last = 0;
}

struct idnt *new_idnt(void)
{
	struct idnt *n;
	
	n = malloc(sizeof(struct idnt));
	
	idnt_init(n);
	
	return n;
}

struct idnt *create_idnt_two_names(struct str *n0, struct str *n1)
{
	struct idnt *n = new_idnt();
	
	n->type = IDNT_OBJVAR;
	
	n->obj_name = str_cpy(n0);
	n->var_name = str_cpy(n1);
	
	return n;
};

struct idnt *create_idnt_reg(int regn)
{
	struct idnt *n = new_idnt();
	
	n->type = IDNT_REG;
	
	n->idx = regn;
	
	
	return n;
};

void func_init(struct func *in)
{
	in->id = -1;
	in->label = -1;
	in->label_name = 0;
	in->obj_idx = -1;
	in->obj_name = 0;
	in->ret = 0;
	in->args = 0;
	in->next = 0;
	in->last = 0;
	
}

struct func *new_func(void)
{
	struct func *n;
	
	n = malloc(sizeof(struct func));
	
	func_init(n);
	
	return n;
}

struct func *create_func_label(struct str *labelstr)
{
	struct func *n;
	
	n = new_func();
	n->label_name = labelstr;
	n->id = F_LABEL;
	
	return n;
	
};

struct func *create_func_jmp(int id, struct str *labelstr, int regn)
{
	struct func *n;
	
	n = new_func();
	n->id = id;
	n->label_name = labelstr;
	
	if (id == F_IF_JMP)
	{
		n->args = new_idnt();
		n->args->type = IDNT_REG;
		n->idx = regn;
	}
	
	return n;
};

void obj_init(struct obj *in)
{
	in->type = 0;
	in->name = 0;
	
	in->vars = 0;
	
	in->init = 0;
	in->body = 0;
	in->term = 0;
	
	in->next = 0;
	
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
	in->first = 0;
	in->last = 0;
}

void add_obj(struct obj_dat *in)
{
	if (!in->first)
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

void obj_add_var(struct obj *in, struct var *addvar)
{
	if (!in->vars)
	{
		in->vars = addvar;
		in->vars->last = in->vars;
	}
	else
	{
		in->vars->last->next = addvar;
		in->vars->last = in->vars->last->next;
	}
}

void obj_add_new_var(struct obj *in)
{
	obj_add_var(in, new_var());
}

void obj_add_func(struct obj *in, struct func *addme, int stype)
{
	if (stype == S_INIT)
	{
		if (!in->init)
		{
			in->init = addme;
			in->init->last = in->init;
		}
		else
		{
			in->init->last->next = addme;
			in->init->last = in->init->last->next;
		}

	}
	else if (stype == S_BODY)
	{
		if (!in->body)
		{
			in->body = addme;
			in->body->last = in->body;
		}
		else
		{
			in->body->last->next = addme;
			in->body->last = in->body->last->next;
		}

	}
	else if (stype == S_TERM)
	{
		if (!in->term)
		{
			in->term = addme;
			in->term->last = in->term;
		}
		else
		{
			in->term->last->next = addme;
			in->term->last = in->term->last->next;
		}

	}
}

