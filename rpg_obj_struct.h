

#define V_INT	0
#define V_FLOAT	1
#define V_STR	2
#define V_NAME	3
#define V_LIST	4
#define V_NULL	-1

#define IDNT_VAR 		4
#define IDNT_OBJVAR 	3
#define IDNT_OBJ 		2
#define IDNT_NULL 		0
#define IDNT_REG 		1

#define NO_OPP -1

#define F_UNKNOWN -2
#define F_LABEL	-1




#define S_INIT	1
#define S_BODY	2
#define S_TERM	4

/* this needs to be the length of funcnames[] */



struct var
{
	int type;
	struct str *name;
	
	struct str 	*dat_str;
	struct str 	*dat_str_1; /* for names.*/
	struct str 	*dat_str_2;
	
	int			dat_int; /* if var of type name, the will be obj idx of first name */
	float		dat_float;
	
	/* if var itself is a list, this references first item in list. */
	struct var *dat_list;
	
	/* for next item in list (var is in list if it's refereced by var of type V_LIST).  NULL if item is last in list. */
	struct var *list_next; 
	
	/*
	 * next var in var list (within obj data, NOT func args);
	 * any var data in function args is contained within the idnt struct.
	 */
	struct var *next; /* NULL if last. */
	struct var *last;
	
};

/* used for func return and args, thus "next" */
struct idnt
{
	int type;
	/*
	 * IDNT_NULL |	null identifer; nothing happens
	 * IDNT_REG |	register, "idx" becomes register name (0-7)
	 * IDNT_VAR |	identifer contains its own variable data; "use_var"
	 * IDNT_OBJVAR |
	 * IDNT_OBJ
	 */
	 
	/* register number OR obj_idx */
	int idx;
	
	/* object name (in case of run-tine obj creation) */
	struct str *obj_name;
	
	/* var name (goes with obj idx) */
	struct str *var_name;
	
	/* for when argument contains var data */
	struct var *use_var;
	
	struct idnt *next; /* for next arg.  NULL if last */
	struct idnt *last;
};

struct func
{
	int id;
	
	/* for labels (id=-1), OR jmp functions: */
	int label;
	struct str *label_name; /* if id=F_UNKNOWN, this becomes function name string. */
	
	/* idx to object (as opperand).  may be NO_OPP if no opperand obj */
	int obj_idx;
	/* may be needed for object creation. */
	struct str *obj_name; 
	struct obj *ob;
	
	struct idnt *ret; /* NULL if no return. */
	
	struct idnt *args; /* NULL if no arguments are given to function call */
	
	struct func *next; /* NULL if last. */
	struct func *last;
};

struct obj
{
	struct str *type;
	int itype;
	
	struct str *name;
	
	/* vars list */
	struct var *vars;  /* NULL if empty*/
	
	/* obj scripts */
	struct func *init; /* NULL if empty*/
	struct func *body;
	struct func *term;
	
	struct obj *next;
};

struct obj_dat
{
	struct obj *first;
	struct obj *last;
};

void var_init(struct var *in);
struct var *new_var(void);
void free_var(struct var *in);
int var_search_str(struct var *vars, struct str *find);
void var_add_str(struct var *vars, struct str *add);
void add_var(struct var *vars, struct var *addme);
void add_new_var(struct var *vars);
void print_var_val(struct var *in);
void print_var(struct var *in, char *pad);
void idnt_init(struct idnt *in);
void print_idnt(struct idnt *in, char *pad);
struct idnt *new_idnt(void);
struct idnt *create_idnt_two_names(struct str *n0, struct str *n1);
struct idnt *create_idnt_reg(int regn);
void func_init(struct func *in);
struct func *new_func(void);
void print_func(struct func *in, char *pad);
struct func *create_func_label(struct str *labelstr);
struct func *create_func_jmp(int id, struct str *labelstr, int regn);
void obj_init(struct obj *in);
struct obj *new_obj(void);
void obj_dat_init(struct obj_dat *in);
void add_obj(struct obj_dat *in);
void obj_add_var(struct obj *in, struct var *addvar);
void obj_add_new_var(struct obj *in);
void obj_add_var(struct obj *in,  struct var *addvar);
void obj_add_func(struct obj *in, struct func *addme, int stype);
void print_objs(struct obj_dat *in);
int get_funcname_id(struct str *in);
char *get_funcname(int id);
