

#define M_IMG 0
#define M_SND 0

struct media_lib_i
{
	int type;
	int loaded;
	struct str *fn;
	SDL_Surface *img_dat;
	struct media_lib_i *next;
};

struct media_lib
{
	struct media_lib_i *first;
	struct media_lib_i *last;
	int loaded;
};


SDL_Surface *media_lib_get_img(struct media_lib *in, int idx);
struct media_lib_i *mli_new_img(struct str *fn);
struct media_lib *new_media_lib();
void ml_add_img(struct media_lib *in, struct str *fn);
