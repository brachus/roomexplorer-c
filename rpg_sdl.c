

#include "inc.h"
#include "rpg_sdl.h"



void tick_frame(int fps)
{
	static unsigned int prevtime;
	static int first = 1;
	if (!first)
		SDL_Delay( (1000/fps) - (SDL_GetTicks() - prevtime) );
	else
		first = 0;
	prevtime = SDL_GetTicks();
}

void sdl_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		vm_err(0,0,0,"sdl init failed.");
	
	if (IMG_Init(IMG_INIT_PNG) < 0)
		vm_err(0,0,0,"sdl image init failed.");
}

void sdl_term()
{
	
	IMG_Quit();

	SDL_Quit();
}

SDL_Window *sdl_mkwin(char *title, int w, int h )
{
	return  SDL_CreateWindow ( title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w, h,
			SDL_WINDOW_OPENGL);
}

SDL_Surface *get_win_surface(SDL_Window *win)
{
	return SDL_GetWindowSurface(win);
}

void fill_surf(SDL_Surface *surf, int r, int g, int b)
{
	SDL_FillRect( surf, 0, SDL_MapRGB( surf->format, r, g, b ) );
}

void blit_surf(SDL_Surface *src, SDL_Surface  *dst, int x, int y )
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	
	SDL_BlitSurface(src, 0 , dst, &pos);
}

void win_update(SDL_Window *win)
{
	SDL_UpdateWindowSurface(win);
}

SDL_Surface *sdl_mksurf(int w, int h)
{
	return SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

struct sdl_win *mk_sdl_win(char *title)
{
	struct sdl_win *n = malloc(sizeof(struct sdl_win));
	
	n->win = sdl_mkwin(title, SCREENWIDTH * 2, SCREENHEIGHT * 2);
	n->w_surf = get_win_surface(n->win);
	n->m_surf = sdl_mksurf(SCREENWIDTH, SCREENHEIGHT);
	
	return n;
};

void blitto_sdl_win(SDL_Surface *src, struct sdl_win *dst, int x, int y)
{
	blit_surf(src, dst->m_surf, x, y);
}

void update_sdl_win(struct sdl_win *in)
{
	SDL_BlitScaled(in->m_surf, 0, in->w_surf, 0);
	win_update(in->win);
}

void clear_sdl_win(struct sdl_win *in ,int r,int g,int b)
{
	fill_surf(in->m_surf, r, g, b);
}
