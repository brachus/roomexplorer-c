

struct rpg_render
{
	struct sdl_win *win;
	
	struct r_sprite **sprites; /* 8 layers, each a list of r_sprites */
};

struct r_sprite
{
	int x;
	int y;
	int cx; /* center; from top-left of image to render */
	int cy;
	SDL_Surface *gfx;
	
	struct r_sprite *next;
};

struct rpg_render *new_vm_render(struct sdl_win *in);

struct r_sprite *new_r_sprite(int x, int y, int cx, int cy, struct media_lib_i *gfx);

void free_r_sprites(struct r_sprite *in);

void render_sprite(struct r_sprite *in, SDL_Surface *surf, int camx, int camy);

void vm_render_add_sprt(struct rpg_render *r, int layer, int x, int y, int cx, int cy, struct media_lib_i *gfx);

void vm_render_dorender(struct rpg_render *in, struct obj *omain);
