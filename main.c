
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"


int main(void)
{
	struct str fn;
	
	fn = new_str("test");
	
	struct token tokens;
	
	token_init(&tokens);
	
	parse_tokenize(&fn, &tokens);
	
	print_tokens(&tokens);
	
	
	struct var *testvar;
	
	testvar = parse_literal_expr(&tokens);
	
	
	print_var(testvar);
	
	
	return 0;
	
}
