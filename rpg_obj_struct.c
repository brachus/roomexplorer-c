
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_func_def.h"
#include "rpg_media.h"




/* TODO:
 * 	 functions for freeing var, obj, func, and
 *   idnt structs. compare to free(p)
 */


void var_init(struct var *in)
{
	in->type = V_NULL;
	
	in->name = 0;
	
	in->dat_str = 0;
	in->dat_str_1 = 0;
	in->dat_str_2 = 0;
	
	in->ob = 0;
	
	in->dat_int = 0;
	in->dat_float = 0;
	
	in->dat_media = 0;
	
	in->dat_list = 0;
	in->list_next = 0;
	in->next  = 0;
	in->last = in;
	
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
	if (!in)
		return;
	
	if (in->name != 0)
		free_str(in->name);
	
	if (in->dat_str != 0)
		free_str(in->dat_str);
	
	if (in->dat_str_1 != 0)
		free_str(in->dat_str_1);
		
	if (in->dat_str_2 != 0)
		free_str(in->dat_str_2);
	
	if (in->dat_list != 0)
		free_var(in->dat_list);
	
	if (in->list_next != 0)
		free_var(in->list_next);
	
	if (in->next != 0)
	{
		free_var(in->next);
		in->last = 0;
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


void var_inplace_cpy(struct var *cpyme, struct var *istay)
{
	/* knock out value, replace it with some other value,
	 * keep links and name.
	 */
	free_str(istay->dat_str);
	free_str(istay->dat_str_1);
	free_str(istay->dat_str_2);
	free_var(istay->dat_list);
	free_var(istay->list_next);
	istay->dat_list = 0;
	istay->list_next = 0;
	
	istay->type = cpyme->type;
	istay->dat_str = str_cpy(cpyme->dat_str);
	istay->dat_str_1 = str_cpy(cpyme->dat_str_1);
	istay->dat_str_2 = str_cpy(cpyme->dat_str_2);
	istay->dat_int = cpyme->dat_int;
	istay->dat_float = cpyme->dat_float;
	if (cpyme->dat_list != 0)
		istay->dat_list = var_cpy(cpyme->dat_list);
	if (cpyme->list_next != 0)
		istay->list_next = var_cpy(cpyme->list_next);
	istay->ob = cpyme->ob;
	istay->dat_media = cpyme->dat_media;
}

struct var *var_cpy(struct var *v)
{
	struct var *cpy = new_var();
	
	cpy->type = v->type;
	cpy->name = str_cpy(v->name);
	
	cpy->dat_str = str_cpy(v->dat_str);
	cpy->dat_str_1 = str_cpy(v->dat_str_1);
	cpy->dat_str_2 = str_cpy(v->dat_str_2);
	
	cpy->ob = v->ob;
	
	cpy->dat_int = v->dat_int;
	cpy->dat_float = v->dat_float;
	
	if (v->dat_list != 0)
		cpy->dat_list = var_cpy(v->dat_list);
	if (v->list_next != 0)
	cpy->list_next = var_cpy(v->list_next);
	
	return cpy;
};

void print_var_val(struct var *in)
{
	if (!in)
	{
		printf("null");
		return;
	}	
		
	switch(in->type)
	{
	case V_INT:
		printf("%d", in->dat_int);
		break;
	case V_FLOAT:
		printf("%f", in->dat_float);
		break;
	case V_STR:
		/*printf("\"");*/
		str_print(in->dat_str);
		/*printf("\"");*/
		break;
	case V_NAME:
		str_print(in->dat_str);
		if (in->dat_str_1 != 0)
		{
			printf(".");
			str_print(in->dat_str_1);
		}
		if (in->dat_str_2 != 0)
		{
			printf(".");
			str_print(in->dat_str_2);
		}
			
		printf("  (%d)", in->ob);
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
	
	printf("%s", pad);
	
	if (!in)
	{
		printf("\n");
		return;
	}
	
		
	if (!in->name || in->name->length == 0)
		printf("<noname>");
	else
		str_print(in->name);
	
	printf(":  ");
	
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
	in->ob = 0;
	
	in->var_name = 0;
	
	in->use_var = 0;
	in->next = 0;
	in->last = 0;
}

void print_idnt(struct idnt *in, char *pad)
{
	printf("%s", pad);
	
	if (!in)
	{
		printf("");
		return;
	}
		
	switch (in->type)
	{
	case IDNT_NULL:
		printf("null");
		break;
	case IDNT_OBJ:
		str_print(in->obj_name);
		break;
	case IDNT_OBJVAR:
		str_print(in->obj_name);
		printf(".");
		str_print(in->var_name);
		
		printf("(%d)", in->idx);
		break;
	case IDNT_REG:
		printf("r%d", in->idx);
		break;
	case IDNT_VAR:
		
		print_var_val(in->use_var);
		break;
	default:break;
	}
	
	if (in->next != 0)
	{
		printf(", ");
		print_idnt(in->next, pad);
	}
		
}

struct idnt *new_idnt()
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
	in->ob = 0;
	in->ret = 0;
	in->args = 0;
	in->next = 0;
	in->last = 0;
	in->lbl = 0;
	
}

struct func *new_func(void)
{
	struct func *n;
	
	n = malloc(sizeof(struct func));
	
	func_init(n);
	
	return n;
}

void print_func(struct func *in, char *pad)
{
	printf("%s", pad);
	
	if (!in)
	{
		printf("\n");
		return;
	}
	
	if (in->id == F_LABEL)
	{
		str_print(in->label_name);
		printf(":\n");
	}
	else
	{
		if (in->ret != 0)
		{
			print_idnt(in->ret, "");
			printf(" = ");
		}
		
		if (in->obj_name != 0)
		{
			str_print(in->obj_name);
			printf(".");
		}
		
		if (in->id >= 0)
			printf("%s", get_funcname(in->id));
		
		printf("(");
		
		if (in->args != 0)
			print_idnt(in->args, "");
		
		printf(")");
		
		if (in->label_name != 0)
		{
			printf(" goto: ");
			str_print(in->label_name);
		}
		
		printf("\n");

	}
	
		
	
	
	if (in->next != 0)
		print_func(in->next, pad);
}

struct func *create_func_label(struct str *labelstr)
{
	struct func *n;
	
	n = new_func();
	n->label_name = str_cpy(labelstr);
	n->id = F_LABEL;
	
	return n;
	
};

struct func *create_func_jmp(int id, struct str *labelstr, int regn)
{
	struct func *n;
	
	n = new_func();
	n->id = id;
	n->label_name = str_cpy(labelstr);
	
	if (id == F_IF_JMP)
	{
		n->args = new_idnt();
		n->args->type = IDNT_REG;
		n->args->idx = regn;
	}
	
	return n;
};

void obj_init(struct obj *in)
{
	in->type = 0;
	in->itype = 0;
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

void print_objs(struct obj_dat *in)
{
	struct obj *otmp;
	struct var *vtmp;
	struct idnt *itmp;
	
	
	otmp = in->first;
	
	while (otmp != 0)
	{
		printf("\n");
		str_print(otmp->type);
		printf(" ");
		str_print(otmp->name);
		printf(":\n");
		
		print_var(otmp->vars, "  ");
		
		printf("  init:\n");
		print_func(otmp->init, "    ");
		
		printf("  body:\n");
		print_func(otmp->body, "    ");
		
		printf("  term:\n");
		print_func(otmp->term, "    ");
		
		otmp = otmp->next;
	}
	
}

static char *funcnames[] = 
{
	"set",
	"println",
	"op_add",
	"op_sub",
	"op_mul",
	"op_div",
	"op_mod",
	"cmp_equal",
	"cmp_nequal",
	"cmp_greater",
	"cmp_lesser",
	"cmp_greatequal",
	"cmp_lessequal",
	"if_jmp",
	"jmp",
	"term",
	"term_vm",
	"next_frame", /* loop */
	"sleep",
	"key",
	"key_down",
	"key_up",
	"key_move",
	"fade_var",
	"start",
	"stop",
	"activate",
	"deactivate",
	"randint",
	"cam_focus",
	"cam_unfocus",
	"clip",
	"aactors_clear",
	"atmaps_clear",
	"keys_disable",
	"keys_enable",
	"sprt_fill_layer",
	"tmap_fill_layer",
	"msg_clear",
	"msg_send",
	"msg_check",
	"open",
	"close",
	"setmode",
	"playsnd",
	"stopsnd",
	"stopall",
	"additem",
	"dropitem",
	"print",
	"type"
};

/* match an input str with an entry in global funcnames */
int get_funcname_id(struct str *in)
{
	
	int i = 0;
	for (i=0;i<NFUNCNAMES;i++)
	{
		if (str_cmp_cstr(in, funcnames[i]))
			return i;
	}
		
	return F_UNKNOWN;
}

char *get_funcname(int id)
{
	if (id >= 0 && id < NFUNCNAMES)
		return funcnames[id];
	return 0;
}

/* supports only int, float, or str types for now. */
void str_append_var(struct str *in, struct var *vin)
{
	if (!in || !vin)
		return;
		
	switch (vin->type)
	{
	case V_INT:
		str_append_int(in, vin->dat_int);
		return;
	case V_FLOAT:
		str_append_float(in, vin->dat_float);
		return;
	case V_STR:
		str_append_str(in, vin->dat_str);
	}
	
	return;
}


struct var *get_var_from_list(struct var *lst, int idx)
{
	struct var *tmp;
	int i=0;
	
	if (!lst)
		return 0;
		
	tmp = lst->dat_list;
	
	while (tmp != 0)
	{
		if (idx == i)
			return tmp;
		
		i++;
		tmp = tmp->list_next;
	}
	
	return 0;
	
};

int get_list_length(struct var *lst)
{
	if (!lst)
		return 0;
		
	int ln = 0;
	struct var *tmp = lst->dat_list;
	
	if (!tmp)
		return 0;
	
	while (tmp != 0)
	{
		ln++;
		tmp = tmp->list_next;
	}
	
	return ln;
}

struct var *get_var_from_cstr(struct var *frst, char *cstr)
{
	if (!frst)
		return 0;
	
	struct var *tmp;
	
	tmp = frst;
	
	while (tmp != 0)
	{
		if (str_cmp_cstr(tmp->name, cstr))
			return tmp;
			
		tmp = tmp->next;
	}
	
	printf("varname: %s\n", cstr);
	vm_err(0,0,0,"couldn't find var.");
};

struct obj *get_obj_from_cstr(struct obj_dat *odat, char *type, char *name)
{
	struct obj *tmp;
	
	if (!odat)
		return 0;
	
	tmp = odat->first;
	
	if (!tmp)
		return 0;
	
	while (tmp != 0)
	{
		if (str_cmp_cstr(tmp->name, name) &&
			str_cmp_cstr(tmp->type, type))
			return tmp;
			
		tmp = tmp->next;
	}
	
	return 0;
};

/* addme doesnt get copied! */
void list_append(struct var *lst, struct var *addme)
{
	if (!lst)
		return;
	
	if (lst->type == V_LIST)
	{
		
	}
}

struct var *lst_get_idx(struct var *lst, int idx)
{
	struct var *tmp;
	int i;
	
	tmp = lst->dat_list;
	
	for (i=0;;i++)
	{
		if (!tmp)
			return 0;
		
		if (i == idx)
			return tmp;
		
		tmp = tmp->list_next;
	}
	
	vm_err(0,0,0,"out of bounds index.");
};

void var_force_coord(struct var *in)
{
	if (!in)
		vm_err(0,0,0,"null var.");
		
	if (in->type == V_LIST &&
		in->dat_list != 0 &&
		in->dat_list->type == V_INT &&
		in->dat_list->list_next != 0 && 
		in->dat_list->list_next->type == V_INT)
		return;
		
	printf("var ");
	str_print(in->name);
	printf(":\n");
	vm_err(0,0,0,"var is not an integer coordinate.");
}

void var_force_fcoord(struct var *in)
{
	if (!in)
		vm_err(0,0,0,"null var.");
		
	if (in->type == V_LIST &&
		in->dat_list != 0 &&
		in->dat_list->type == V_FLOAT &&
		in->dat_list->list_next != 0 && 
		in->dat_list->list_next->type == V_FLOAT)
		return;
		
	printf("var ");
	str_print(in->name);
	printf(":\n");
	vm_err(0,0,0,"var is not an float coordinate.");
}

void var_force_str(struct var *in)
{
	if (!in)
		vm_err(0,0,0,"null var.");
		
	if (in->type == V_STR &&
		in->dat_str != 0)
		return;
		
	printf("var ");
	str_print(in->name);
	printf(":\n");
	vm_err(0,0,0,"var is not a string.");
}

void var_force_int(struct var *in)
{
	if (!in)
		vm_err(0,0,0,"null var.");
		
	if (in->type == V_INT)
		return;
		
	printf("var ");
	str_print(in->name);
	printf(":\n");
	vm_err(0,0,0,"var is not int.");
}

/* works with both int and float */
void var_cpy_coord(struct var *src, struct var *dst)
{
	dst->dat_list->dat_int = src->dat_list->dat_int;
	dst->dat_list->dat_float = src->dat_list->dat_float;
	dst->dat_list->list_next->dat_int =
		src->dat_list->list_next->dat_int;
	dst->dat_list->list_next->dat_float =
		src->dat_list->list_next->dat_float;
}

void var_set_coord(struct var *in, int x, int y)
{
	in->dat_list->dat_int = x;
	in->dat_list->list_next->dat_int = y;
}
