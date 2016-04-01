#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SWIDTH 300
#define SHEIGHT 200


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



int main(void)
{
	/* initialize things: */

	SDL_Event e;
	SDL_Surface *img, *main_display, *tmpd;
	SDL_Window  *win;
	SDL_Rect 	pos;
	SDL_Rect 	pos0;
	
	pos0.x = 0;
	pos0.y = 0;
	
	int ani0 = 0;
	int ani1 = -160;
	
	
	printf("testing ...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL init failed...\n");
		return 1;
	}
	
	IMG_Init(IMG_INIT_PNG);
	
	/* create things: */


	win = SDL_CreateWindow
		(
			"win",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SWIDTH, SHEIGHT,
			SDL_WINDOW_OPENGL
		);
	
	main_display = SDL_GetWindowSurface(win);
	tmpd = SDL_CreateRGBSurface(0, SWIDTH, SHEIGHT, 32, 0, 0, 0, 0);
	
	img = IMG_Load("test.png");
	
	if (!img)
	{
		printf("image load failed...\n");
		return 1;
	}
	
	pos.x = SWIDTH / 2;
	pos.y = SHEIGHT / 2;
	

	/* run things: */

	while (1)
	{
		
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT)
			break;
        
        switch (ani0)
        {
		case 0:
			pos.x = (SWIDTH/2) + 4;
			pos.y = (SHEIGHT/2);
			ani0=1;
			break;
		case 1:
			pos.x = (SWIDTH/2) + 3;
			pos.y = (SHEIGHT/2) + 3;
			ani0=2;
			break;
		case 2:
			pos.x = (SWIDTH/2);
			pos.y = (SHEIGHT/2)+4;
			ani0=3;
			break;
		case 3:
			pos.x = (SWIDTH/2) -3;
			pos.y = (SHEIGHT/2)+3;
			ani0=4;
			break;
		case 4:
			pos.x = (SWIDTH/2) - 4;
			pos.y = (SHEIGHT/2);
			ani0=5;
			break;
		case 5:
			pos.x = (SWIDTH/2) - 3;
			pos.y = (SHEIGHT/2) - 3;
			ani0=6;
			break;
		case 6:
			pos.x = (SWIDTH/2);
			pos.y = (SHEIGHT/2) - 4;
			ani0=7;
			break;
		case 7:
			pos.x = (SWIDTH/2) + 3;
			pos.y = (SHEIGHT/2) - 3;
			ani0=0;
			break;
		}
		
		pos.x += ani1;
		ani1 = 0;
		
		SDL_FillRect( tmpd, 0, SDL_MapRGBA( tmpd->format, 0xFF, 0xFF, 0xFF, 255 ) );
        SDL_FillRect( main_display, 0, SDL_MapRGB( main_display->format, 0xFF, 0xFF, 0xFF ) );
		
		SDL_BlitSurface(img,0 , tmpd, &pos);
		
		SDL_BlitSurface(tmpd,0 , main_display, &pos0);
		
		SDL_UpdateWindowSurface(win);
		
		tick_frame(60);
	}
	
	/* terminate things: */
	
	SDL_FreeSurface(img);
	
	SDL_DestroyWindow(win);

	IMG_Quit();

	SDL_Quit();
	
	return 0;
}
