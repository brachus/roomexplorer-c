

#define V_INT	0
#define V_FLOAT	1
#define V_STR	2
#define V_NAME	3
#define V_LIST	4
#define V_NULL	-1




#define P_GET_OBJ_NAME					14
#define P_GET_OBJ_OPEN_BRACE			15
#define P_OPEN_PREDEF					16
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

#define IF_NULL				-1
#define IF_GET_REG			0
#define IF_IN_BRANCH		1
#define IF_IN_ELSEBRANCH	2
#define IF_OPEN				3
#define IF_GET_ELSE_IF		4
#define IF_IN_BRANCH_ONELINE 5
#define IF_IN_ELSEBRANCH_ONELINE 6





struct ifdat_l
{
	int id;
	int mode;
	int branch_cntr;
	struct ifdat_l *next;
	struct ifdat_l *prev;
};

struct ifdat
{
	struct ifdat_l *first;
	struct ifdat_l *last;
	int if_cntr;
	int if_level;
};

struct func *get_label_pntr(struct func *fns, struct str *lname);
int match_sname(struct str *in);
struct ifdat *new_ifdat();
struct ifdat_l *new_ifdat_l();
int if_get_last_mode(struct ifdat *in);
void if_set_last_mode(struct ifdat *in, int md);
int if_get_last_id(struct ifdat *in);
int if_get_last_bcntr(struct ifdat *in);
void if_set_last_id(struct ifdat *in, int id);
void if_set_last_bcntr(struct ifdat *in, int bcntr);
void if_step_last_bcntr(struct ifdat *in);
struct str *create_jmp_label(int ifid, int branch_cntr,int doend);
void ifdat_pop(struct ifdat *dat);
void add_if(struct ifdat *dat, int md);
void obj_add_label(struct obj_dat *dat, struct str *labelstr, int stype);
void obj_add_ifjmp(struct obj_dat *dat, int regn, struct str *labelstr, int stype);
void obj_add_jmp(struct obj_dat *dat, struct str *labelstr, int stype);
struct func *obj_get_last_func(struct obj *in, int stype);
void func_add_arg(struct func *in, struct idnt *addme);
int obj_find_label(struct obj *in, struct str *findme, int stype);
void obj_jmp_add_label_idx(struct obj *in, int stype);
void idnt_fill_idxs(struct obj_dat *odat, struct idnt *in);
void func_fill_idxs(struct obj_dat *odat, struct func *in);
void var_fill_idxs(struct obj_dat *odat, struct var *in);
void obj_do_each(struct obj_dat *in);
int get_obj_idx(struct obj_dat *in, struct str *obj_str);
struct obj *get_obj_pntr(struct obj_dat *in, struct str *obj_str);
void obj_do_itype(struct obj *in);
struct obj_dat parse_main(struct token *tokens);

