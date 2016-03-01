

#define V_INT	0
#define V_FLOAT	1
#define V_STR	2
#define V_NAME	3
#define V_LIST	4
#define V_NULL	-1

#define IDNT_VAR -2
#define IDNT_OBJVAR -1
#define IDNT_OBJ -2
#define IDNT_NULL -2
#define IDNT_REG -1

#define NO_OPP -1


#define P_GET_OBJ_NAME					14
#define P_GET_OBJ_OPEN_BRACE			15
#define p_OPEN_PREDEF					16
#define P_GET_PREDEF_EQUALS				17
#define P_GET_PREDEF_LITERAL			18
#define P_GET_SCRIPT_OPEN_BRACE			19
#define P_SCRIPT_OPEN					20
#define P_SCRIPT_GET_IF_REG				21
#define P_SCRIPT_GET_ELSE_IF			22
#define P_SCRIPT_GET_IF_BRANCH_OPEN		23
#define P_SCRIPT_GET_PARA_COLON			24
#define P_SCRIPT_GET_PARA_EQUALS		25
#define P_SCRIPT_GET_EQUALS				26
#define P_SCRIPT_GET_FNAME				27
#define P_SCRIPT_GET_OPEN_PARA			28
#define P_SCRIPT_GET_ARG				29
#define P_SCRIPT_GET_SEMICOLON			30
#define P_ERR							31



struct var
{
	int type;
	struct str *name;
	
	struct str 	*dat_str;
	struct str 	*dat_str_1; /* for names.*/
	struct str 	*dat_str_2;
	
	int			dat_int;
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
	 * */
	 
	/* register number OR obj_idx */
	int idx;
	
	/* object name (in case of run-tine obj creation) */
	struct str obj_name;
	
	/* var name (goes with obj idx) */
	struct str var_name;
	
	/* for when argument contains var data */
	struct var *use_var;
	
	struct idnt *next; /* for next arg.  NULL if last */
};

struct func
{
	int id;
	
	int label;
	struct str label_name;
	
	/* idx to object (as opperand).  may be NO_OPP if no opperand obj */
	int obj_idx;
	/* may be needed for object creation. */
	struct str obj_name; 
	
	struct func *next; /* NULL if last. */
};

struct obj
{
	struct str type;
	struct str name;
	
	/* vars list */
	struct var *vars;  /* NULL if empty*/
	
	/* obj scripts */
	struct func *init; /* NULL if empty*/
	struct func *body;
	struct func *term;
};

struct obj_dat
{
	struct obj *first;
	struct obj *last;
};

void var_init(struct var *in);
struct var *new_var(void);
void print_var_val(struct var *in);
void print_var(struct var *in);

void idnt_init(struct idnt *in);
struct idnt *new_idnt(void);

void func_init(struct func *in);
struct func *new_func(void);

void obj_init(struct obj *in);
struct obj *new_obj(void);

void obj_dat_init(struct obj_dat *in);
