
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
	struct str *fn_def = create_str();
	
	struct token tokens;
	struct token tokens_def;
	struct obj_dat odat;
	struct obj_dat odat_def;
	struct asub_dat *asdat;
	struct var **regs;
	
	if (argc > 1)
		str_append_cstr(fn, argv[1]);
	else
		str_append_cstr(fn, "test");
	
	/* load defaults */
	str_append_cstr(fn_def, "def");
	token_init(&tokens_def);
	parse_tokenize(fn_def, &tokens_def);
	odat_def = parse_main(&tokens_def);
	
	/* load main object script */
	token_init(&tokens);
	parse_tokenize(fn, &tokens);
	odat = parse_main(&tokens);
	
	/* add defaults */
	obj_add_def(&odat, &odat_def);
	
	regs = init_regs();
	asdat = new_asub_dat();
	add_asub_main(asdat, &odat);

	
	while (vm_proc_full(asdat, &odat, regs));
	
	
	return 0;
}
