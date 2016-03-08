
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"
#include "rpg_parse_main.h"


int main(void)
{
	struct str *fn = create_str();
	struct token tokens;
	struct obj_dat odat;
	
	
	str_append_cstr(fn, "test");
	
	token_init(&tokens);
	
	parse_tokenize(fn, &tokens);
	
	printf("tokens:\n");
	print_tokens(&tokens);
	
	
	odat = parse_main(&tokens);
	
	
	return 0;
	
}
