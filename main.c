


#define DEBUG 0


#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_media.h"
#include "rpg_parse_expr.h"
#include "rpg_func_def.h"
#include "rpg_parse_main.h"

#include "rpg_sdl.h"
#include "rpg_input.h"

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
	struct media_lib *md_lib;
	struct sdl_win *window;
	struct input_keys *keys = new_input_keys();
	
	if (argc > 1)
		str_append_cstr(fn, argv[1]);
	else
		str_append_cstr(fn, "test");
	
	
	/* create media lib */
	md_lib = new_media_lib();
	
	
	/* load defaults */
	str_append_cstr(fn_def, "def");
	token_init(&tokens_def);
	parse_tokenize(fn_def, &tokens_def);
	odat_def = parse_main(&tokens_def, 0);
	
	
	/* load main object script */
	token_init(&tokens);
	parse_tokenize(fn, &tokens);
	odat = parse_main(&tokens, md_lib);
	
	
	/* add defaults */
	obj_add_def(&odat, &odat_def);
	
	
	/* load media */
	md_lib_loadall(md_lib);
	
	/* print objs */
	if (DEBUG)
		print_objs(&odat);
	
	regs = init_regs();
	asdat = new_asub_dat();
	add_asub_main(asdat, &odat);


	/* start sdl up */
	sdl_init();
	window = mk_sdl_win("test");
	
	while (1)
	{
		handle_events(keys);
				
		if (!vm_proc_full(asdat, &odat, regs, keys))
			break;
			
		clear_sdl_win(window, 0,0,0);
		
		update_sdl_win(window);
		
		clear_keys(keys);
		
		tick_frame(60);
	}
	
	sdl_term();
	
	
	return 0;
}
