
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"
#include "rpg_obj_struct.h"
#include "rpg_media.h"


SDL_Surface *media_lib_get_img(struct media_lib *in, int idx)
{
	struct media_lib_i *tmp;
	int i = 0;
	
	if (!in) return 0;
	
	tmp = in->first;
	
	
	while (tmp != 0)
	{
		if (i == idx)
			return tmp->img_dat;
		
		i++;
		tmp = tmp->next;
	}
	
	return 0;
}

struct media_lib_i *mli_new_img(struct str *fn)
{
	struct media_lib_i *n = malloc(sizeof(struct media_lib_i));
	
	n->type = M_IMG;
	n->loaded = 0;
	n->fn = str_cpy(fn);
	n->img_dat = 0;
	n->next = 0;
	
	return n;
};

struct media_lib *new_media_lib()
{
	struct media_lib *n = malloc(sizeof(struct media_lib));
	
	n->first = 0;
	n->last = 0;
	n->loaded = 0;
	
	return n;
};

void ml_add_img(struct media_lib *in, struct str *fn)
{
	if (!in->first)
	{
		in->first = mli_new_img(fn);
		in->last = in->first;
	}
	else
	{
		in->last->next = mli_new_img(fn);
		in->last = in->last->next;
	}
}

void md_lib_loadall(struct media_lib *in)
{
	struct media_lib_i *tmp = in->first;
	
	SDL_BlendMode bmd = SDL_BLENDMODE_BLEND;
	
	char *fn;
	
	while (tmp != 0)
	{
		if (!tmp->loaded && tmp->type == M_IMG)
		{
			fn = str_to_cstr(tmp->fn);
						
			tmp->img_dat = IMG_Load(fn);
			
			if (!tmp->img_dat)
			{
				printf(" img \"%s\":\n",fn);
				free(fn);
				vm_err(0,0,0, "img doesn't exist.");
			}
			
			SDL_SetSurfaceBlendMode(tmp->img_dat, bmd);
			
				
			free(fn);
		}
		
		tmp = tmp->next;
	}
}



