

struct rpg_render
{
	struct sdl_win *win;
	
	struct r_sprite **sprites; /* 8 layers, each a list of r_sprites */
	struct r_tmap **tmaps;
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

struct r_tmap
{
	int x;
	int y;
	struct var *gfx;
	struct var *arr;
	
	struct r_tmap *next;
};

struct rpg_render *new_vm_render(struct sdl_win *in);

struct r_sprite *new_r_sprite(int x, int y, int cx, int cy, struct media_lib_i *gfx);

struct r_tmap *new_r_tmap(int x, int y, struct var *arr, struct var *gfx);

void free_r_sprites(struct r_sprite *in);

void free_r_tmaps(struct r_tmap *in);

void render_sprite(struct r_sprite *in, SDL_Surface *surf, int camx, int camy);
void render_tmap(struct r_tmap *in, SDL_Surface *surf, int camx, int camy);

void vm_render_add_sprt(struct rpg_render *r, int layer, int x, int y, int cx, int cy, struct media_lib_i *gfx);

void vm_render_add_tmap( struct rpg_render *r,int layer,int x, int y, struct var *array, struct var *gfx);

void vm_render_dorender(struct rpg_render *in, struct obj *omain);

void render_set_cam(struct  obj *omain);
