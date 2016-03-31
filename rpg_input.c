

#include "inc.h"
#include "str_num.h"

#include "rpg_sdl.h"
#include "rpg_input.h"


struct input_keys *new_input_keys()
{
	struct input_keys *n = malloc(sizeof(struct input_keys));
	int i;
	
	n->hold = malloc(sizeof(unsigned int) * 8);
	n->down = malloc(sizeof(unsigned int) * 8);
	n->up =   malloc(sizeof(unsigned int) * 8);
	
	
	for (i=0;i<8;i++)
		n->down[i] = KEYS_NULL;
		
	for (i=0;i<8;i++)
		n->up[i] = KEYS_NULL;
		
	for (i=0;i<8;i++)
		n->hold[i] = KEYS_NULL;
		
	
	return n;
};

void clear_keys(struct input_keys *in)
{
	int i = 0;
	
	for (i=0;i<8;i++)
		in->down[i] = KEYS_NULL;
	
	for (i=0;i<8;i++)
		in->up[i] = KEYS_NULL;
}

int check_key(unsigned int *keys, unsigned int key)
{
	int i;
	for (i=0;i<8;i++)
		if (keys[i] == key)
			return 1;
	return 0;
}

void rm_key(unsigned int *keys, unsigned int key)
{
	int i;
	for (i=0;i<8;i++)
	{
		if (keys[i] == key)
		{
			/* shift all keys to right to remove matching key */
			for (;i<8;i++)
			{
				if (i<7)
					keys[i] = keys[i+1];
				else
					keys[i] = KEYS_NULL;
			}
			
			break;
		}
	}
}

void add_key_down(struct input_keys *in, unsigned int key)
{
	int i;
	
	for(i=0;i<8;i++)
		if (in->down[i] == KEYS_NULL)
		{
			in->down[i] = key;
			break;
		}
}

void add_key_up(struct input_keys *in, unsigned int key)
{
	int i;
	
	for(i=0;i<8;i++)
		if (in->up[i] == KEYS_NULL)
		{
			in->up[i] = key;
			break;
		}
}

void add_key_hold(struct input_keys *in, unsigned int key)
{
	int i;
	
	for(i=0;i<8;i++)
		if (in->hold[i] == KEYS_NULL)
		{
			in->hold[i] = key;
			break;
		}
}

void handle_events(struct input_keys *in)
{
	SDL_Event *e = malloc(sizeof(SDL_Event));
	
	while (SDL_PollEvent(e))
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (!check_key(in->down, e->key.keysym.sym) && 
					!check_key(in->hold, e->key.keysym.sym))
				add_key_down(in, e->key.keysym.sym);
			
			if (!check_key(in->hold, e->key.keysym.sym))
				add_key_hold(in, e->key.keysym.sym);
				
		}
		else if (e->type == SDL_KEYUP)
		{
			if (!check_key(in->up, e->key.keysym.sym))
				add_key_up(in, e->key.keysym.sym);
			rm_key(in->hold, e->key.keysym.sym);
		}
		/* this will change */
		else if (e->type == SDL_QUIT )
		{
			printf(" bye bye\n");
			exit(0);
		}
	}
}

void print_keys(struct input_keys *in)
{
	printf("print_keys: ");
	int i;
	for (i=0;i<8;i++)
		printf("%u, ", in->hold[i]);
	printf("\n");
}



int keys_match_str(unsigned int *keys, struct str *in) 
{
	static char *keystrs[] =
	{
		"left",
		"right",
		"up",
		"down",
		"space",
		"esc"
	};
	
	static int keycodes[] = 
	{
		SDLK_LEFT,
		SDLK_RIGHT,
		SDLK_UP, 
		SDLK_DOWN,
		SDLK_SPACE,
		SDLK_ESCAPE 
	};
	
	int i,j;
	
	for (i=0;i<8;i++)
	{
		j=0;
		while(1)
		{
			if (keycodes[j] == keys[i] && 
				str_cmp_cstr(in, keystrs[j]))
				return 1;
			
			if (keycodes[j] == SDLK_ESCAPE)
				break;
			j++;
		}

	}
	
	return 0;
}
