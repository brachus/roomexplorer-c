
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"


int main(void)
{
	struct str *fn = create_str();
	
	str_append_cstr(fn, "test");
	
	struct token tokens;
	
	token_init(&tokens);
	
	parse_tokenize(fn, &tokens);
	
	print_tokens(&tokens);
	
	
	struct var *testvar;
	testvar = parse_literal_expr(&tokens);
	
	printf("\n");
	
	print_var(testvar,"\t");
	
	
	return 0;
	
}
