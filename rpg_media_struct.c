

#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_media_struct.h"


void init_media(struct media *in)
{
	in->first = 0;
	in->last = 0;
	in->length = 0;
}

struct media_l *new_media_l()
{
	struct media_l *n = malloc(sizeof(struct media_l));
	
	in->last->type = 0;
	in->last->loaded = 0;
	in->last->name = 0;
	in->last->gfx_dat = 0;
	in->last->next = 0;
	
	return n;
}

void add_media(struct media *in, int type, char *name)
{
	if (!in->first)
	{
		in->first = new_media_l();
		in->last = in->first;
	}
	else
	{
		in->last->next = new_media_l();
		in->last = in->last->next;
	}
	
	in->last->type = type;
	in->last->name = strcpy(name);
	
	in->length++;
}

void load_media_l(struct media_l *in)
{
	if (!in)
		return;
	
	if (!in->name)
		vm_err(	0, 0,0,"cannot load media with no filename.");
	
	if (in->type == M_GFX)
	{
		in->gfx_dat = IMG_Load(in->name);
		if (in->gfx_dat != 0)
			in->loaded = 1;
		else
			vm_err(	0, 0,0,"file doesn't exist.");
	}
	else
		vm_err(	0, 0,0,"unknown media type.");
	
	if (in->next != 0)
		load_media_l(in->next);
}

void load_media(struct media *in)
{
	load_media_l(in->first);
}
