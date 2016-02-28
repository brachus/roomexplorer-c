
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"


int main(void)
{
	struct str fn;
	
	fn = new_str("test");
	
	printf("opening ");
	str_print(&fn);
	printf(" ...\n");
	
	struct token tokens;
	
	  parse_tokenize(&fn, &tokens);
	
	struct token_l *tmp;
	tmp = tokens.tok_first;
	while (tmp != NULL)
	{
		printf("%d: ",tmp->line);
		str_print(&(tmp->dat_str[0]));
		printf(" ");
		str_print(&(tmp->dat_str[1]));
		printf(" ");
		str_print(&(tmp->dat_str[2]));
		printf("\n");
		
		tmp = tmp->next;
	}
	
	return 0;
	
}
