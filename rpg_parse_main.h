

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


/* function id's */

#define F_JMP		0
#define F_IF_JMP	1


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

int match_sname(struct str *in);
struct ifdat *new_ifdat();
struct ifdat_l *new_ifdat_l();
int if_get_last_mode(struct ifdat *in);
void if_set_last_mode(struct ifdat *in, int md);
void if_step_last_bcntr(struct ifdat *in);
struct create_jmp_label(int ifid, int branch_cntr,int doend);
void ifdat_pop(struct ifdat *dat);
void add_if(struct ifdat *dat, int md);
void obj_add_label(struct obj_dat *dat, struct str *labelstr, int stype);
struct obj_dat *parse_main(struct token *tokens);
