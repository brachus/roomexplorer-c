

#define SCREENWIDTH 320
#define SCREENHEIGHT 240

#define SSCALE 1


struct sdl_win
{
	SDL_Window *win; /* x SSCALE */
	SDL_Surface *t_surf; /* 1x */
	SDL_Surface *m_surf; /* 1x */
	SDL_Surface *w_surf; /* x SSCALE */
};


void tick_frame(int fps);
void sdl_init();
void sdl_term();
SDL_Window *sdl_mkwin(char *title, int w, int h );
SDL_Surface *get_win_surface(SDL_Window *win);

void fill_surf(SDL_Surface *surf, int r, int g, int b);
void fill_surf_rgba(SDL_Surface *surf, int r, int g, int b, int a);
void blit_surf(SDL_Surface *src, SDL_Surface  *dst, int x, int y );

void win_update(SDL_Window *win);

SDL_Surface *sdl_mksurf(int w, int h);

struct sdl_win *mk_sdl_win(char *title);
void blitto_sdl_win(SDL_Surface *src, struct sdl_win *dst, int x, int y);
void update_sdl_win(struct sdl_win *in);
void clear_sdl_win(struct sdl_win *in ,int r,int g,int b,int a);
void clear_tmp(struct sdl_win *in);
