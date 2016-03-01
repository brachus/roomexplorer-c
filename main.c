
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"


int main(void)
{
	struct str fn;
	
	fn = new_str("test");
	
	struct token tokens;
	token_init(&tokens);
	
	  parse_tokenize(&fn, &tokens);
	
	struct token_l *tmp;
	tmp = tokens.tok_first;
	while (tmp != NULL)
	{
		
		printf("ln %d: ",tmp->line);
		
		switch(tmp->type)
		{
		case T_INT:
			printf("int %d\n",tmp->dat_int);
			break;
		case T_FLOAT:
			printf("float %d\n",tmp->dat_int);
			break;
		case T_STR:
			printf("str ");
			str_print(&(tmp->dat_str[0]));
			printf("\n");
			break;
		case T_NAME:
			printf("name ");
			str_print(&(tmp->dat_str[0]));
			printf(" ");
			str_print(&(tmp->dat_str[1]));
			printf(" ");
			str_print(&(tmp->dat_str[2]));
			printf("\n");
			break;
		case T_SYM:
			printf("sym ");
			str_print(&(tmp->dat_str[0]));
			printf("\n");
			break;
		default:
			break;
		}
		
		tmp = tmp->next;
	}
	
	return 0;
	
}
