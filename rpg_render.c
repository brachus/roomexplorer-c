

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

#include "rpg_render.h"


struct rpg_render *new_vm_render(struct sdl_win *in)
{
	struct rpg_render *n = malloc(sizeof(struct rpg_render));
	int i;
	
	n->win = in;
	
	n->sprites = malloc(sizeof(struct r_sprite *) * 8);
	
	for (i=0;i<8;i++)
		n->sprites[i] = 0;
		
	n->tmaps = malloc(sizeof(struct r_tmap *) * 8);
	
	for (i=0;i<8;i++)
		n->tmaps[i] = 0;
	
	return n;
};

void clear_vm_render(struct rpg_render *in)
{
	int i;
	for (i=0;i<8;i++)
	{
		free_r_sprites(in->sprites[i]);
		in->sprites[i] = 0;
	}
		
}

struct r_sprite *new_r_sprite(int x, int y, int cx, int cy, struct media_lib_i *gfx)
{
	struct r_sprite *n = malloc(sizeof(struct r_sprite));
	
	if (!gfx)
		vm_err(0,0,0,"invalid media_lib_i.");
	
	n->x = x;
	n->y = y;
	n->cx = cx;
	n->cy = cy;
	n->gfx = gfx->img_dat;
	
	n->next = 0;
	
	return n;
};

struct r_tmap *new_r_tmap(int x, int y, struct var *arr, struct var *gfx)
{
	struct r_tmap *n = malloc(sizeof(struct r_tmap));
	
	n->x = x;
	n->y = y;
	n->arr = arr;
	n->gfx = gfx;
	
	n->next = 0;
	
	return n;
};

void free_r_sprites(struct r_sprite *in)
{
	if (!in)
		return;
		
	free_r_sprites(in->next);
	
	free(in);
}

void free_r_tmaps(struct r_tmap *in)
{
	if (!in)
		return;
		
	free_r_tmaps(in->next);
	
	free(in);
}

void render_sprite(struct r_sprite *in, SDL_Surface *surf, int camx, int camy)
{
	int x, y;
	
	x = in->x - camx;
	y = in->y - camy;
	
	y = -y;
	
	x += SCREENWIDTH / 2;
	y += SCREENHEIGHT / 2;

	x -= in->cx;
	y -= in->cy;
		
	blit_surf(in->gfx, surf, x, y);
}


void render_tmap(struct r_tmap *in, SDL_Surface *surf, int camx, int camy)
{
	int x, y, gfx_idx, irow=0, icol=0;
	
	struct var *row, *col;
	
	row = in->arr->dat_list;
	
	
	x = in->x - camx;
	y = in->y - camy;
	
	y = -y;
	
	x += SCREENWIDTH / 2;
	y += SCREENHEIGHT / 2;
	
	
	
	while (row != 0)
	{
		col = row->dat_list;
		icol=0;
		while (col != 0)
		{
			if (!col->dat_media)
			{
				gfx_idx = col->dat_int;
				col->dat_media = lst_get_idx(in->gfx, gfx_idx)->dat_media;
				/* fill pointer for array cell. */
			}
			
			blit_surf(col->dat_media->img_dat, surf, x + (icol*16), y + (irow*16));
			
			col = col->list_next;
			icol++;
		}
		row = row->list_next;
		irow++;
	}
	
}

void vm_render_add_sprt(struct rpg_render *r, int layer, int x, int y, int cx, int cy, struct media_lib_i *gfx)
{
	struct r_sprite *tmp, *nxt, *prev;
	
	if (r->sprites[layer] == 0)
		r->sprites[layer] = new_r_sprite(x,y,cx,cy,gfx);
		
	else
	{
		tmp = r->sprites[layer];
		prev=0;
		/* list runs from highest to lowest y value. */
		while (tmp != 0)
		{
			/* insert item *before* first one with lesser y value. */
			if (y > tmp->y)
			{
				if (!prev)
				{
					r->sprites[layer] = new_r_sprite(x,y,cx,cy,gfx);
					r->sprites[layer]->next = tmp;
				}
				else
				{
					prev->next = new_r_sprite(x,y,cx,cy,gfx);
					prev->next->next = tmp;
				}
				break;
			}
			else if (tmp->next == 0)
			{
				tmp->next = new_r_sprite(x,y,cx,cy,gfx);
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}
}

void vm_render_add_tmap( struct rpg_render *r,int layer,int x, int y, struct var *array, struct var *gfx)
{
	struct r_tmap *tmp, *nxt;
	
	if (r->tmaps[layer] == 0)
		r->tmaps[layer] = new_r_tmap(x,y,array, gfx);
	else
	{
		tmp = r->tmaps[layer];
		
		while (tmp!=0)
		{
			if (!tmp->next)
			{
				tmp->next = new_r_tmap(x,y,array, gfx);
				break;
			}
			
			tmp = tmp->next;
		}
	}
		
}

void vm_render_dorender(struct rpg_render *in, struct obj *omain)
{
	struct var *cam_pos, *layer_opacity, *opacity, *f_rgb, *f_o, *tmp;
	struct r_sprite *rtmp;
	struct r_tmap *ttmp;
	int i, j, camx, camy;
	
	
	
	cam_pos = get_var_from_cstr(omain->vars, "cam_pos");
	
	var_force_coord(cam_pos);
	
	camx = cam_pos->dat_list->dat_int;
	camy = cam_pos->dat_list->list_next->dat_int;
	
	
	for (i=0;i<8;i++)
	{
		
		layer_opacity = get_var_from_cstr(omain->vars, "tmap_layer_opacity");
		opacity = lst_get_idx(layer_opacity, i);
		
		/* render tilemap layer if visible */
		if (opacity->dat_int > 0)
		{
			clear_tmp(in->win);
			
			/*prefill*/
			
			/*draw tilemaps to tmp*/
			ttmp = in->tmaps[i];
			while (ttmp != 0)
			{
				render_tmap(ttmp, in->win->t_surf, camx, camy);
				ttmp = ttmp->next;
			}
			
			/* blit t_surf to m_surf */
			blit_surf(in->win->t_surf, in->win->m_surf, 0,0);
		}
		
		layer_opacity = get_var_from_cstr(omain->vars, "sprt_layer_opacity");
		opacity = lst_get_idx(layer_opacity, i);
		/* render sprite layer if visible */
		if (opacity->dat_int > 0)
		{
			clear_tmp(in->win);
			
			/*prefill*/
			/* draw sprites of current layer to tmp*/
			rtmp = in->sprites[i];
			while (rtmp != 0)
			{
				render_sprite(rtmp, in->win->t_surf, camx, camy);
				rtmp = rtmp->next;
			}
			blit_surf(in->win->t_surf, in->win->m_surf,0,0);
			
		}
		
	}
	
	/* render fade overlay */
	f_rgb = get_var_from_cstr(omain->vars, "fad_color");
	f_o = get_var_from_cstr(omain->vars, "fad_opacity");
		
	fill_surf_rgba(
		in->win->t_surf,
		lst_get_idx(f_rgb, 0)->dat_int,
		lst_get_idx(f_rgb, 1)->dat_int,
		lst_get_idx(f_rgb, 2)->dat_int,
		f_o->dat_int
		);
		
	blit_surf(in->win->t_surf, in->win->m_surf,0,0);
}

void render_set_cam(struct  obj *omain)
{
	struct var *tmp, *cam_pos, *cam_scale, *cam_actor,
		*apos, *ascale;
	
	cam_actor = get_var_from_cstr(omain->vars, "cam_actor");
	cam_pos = get_var_from_cstr(omain->vars, "cam_pos");
	cam_scale = get_var_from_cstr(omain->vars, "cam_scale");
	
	var_force_coord(cam_pos);
	var_force_fcoord(cam_scale);
	
	if (cam_actor->type == V_NAME && cam_actor->ob != 0)
	{
		apos = get_var_from_cstr(cam_actor->ob->vars, "pos");
		ascale = get_var_from_cstr(cam_actor->ob->vars, "scale");
		
		var_cpy_coord(apos, cam_pos);
	}
}
