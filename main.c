
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_parse_expr.h"
#include "rpg_func_def.h"
#include "rpg_parse_main.h"
#include "rpg_vm_proc.h"


int main(int argc, char *argv[])
{
	struct str *fn = create_str();
	struct token tokens;
	struct obj_dat odat;
	struct asub_dat *asdat;
	struct var **regs;
	
	if (argc > 1)
		str_append_cstr(fn, argv[1]);
	else
		str_append_cstr(fn, "test");
	
	token_init(&tokens);
	
	parse_tokenize(fn, &tokens);
	
	/*printf("tokens:\n");
	print_tokens(&tokens);*/
	
	
	odat = parse_main(&tokens);
	
	/*print_objs(&odat);
	printf("_________\n\n");*/
	
	asdat = new_asub_dat();
	
	add_asub_main(asdat, &odat);
	
	regs = init_regs();
	
	vm_proc_full(asdat, &odat, regs);
	
	return 0;
	
}
