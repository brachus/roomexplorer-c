

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

#define S_INIT	0
#define S_BODY	1
#define S_TERM	2

struct obj_dat *parse_main(struct token *tokens);
