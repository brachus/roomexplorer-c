


#define M_GFX 1
#define M_SND 2

struct media_l
{
	int type;
	int loaded;
	char *name;
	SDL_Surface *gfx_dat;
	struct media_l *next;
};

struct media
{
	struct media_l *first;
	struct media_l *last;
	int length;
};


void init_media(struct media *in);
struct media_l *new_media_l();
void add_media(struct media *in, int type, char *name);
void load_media_l(struct media_l *in);
void load_media(struct media *in);
