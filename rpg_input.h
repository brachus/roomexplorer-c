

/* create functions for updating input_keys struct
 * after each even poll.
 */

#define KEYS_NULL -1

struct input_keys
{
	int *hold;
	int *down; /* just pressed down */
	int *up;/* just released */
};


struct input_keys *new_input_keys()
{
	struct input_keys *n = malloc(sizeof(struct input_keys));
	
	n->hold = malloc(sizeof(int) * 8);
	n->down = malloc(sizeof(int) * 8);
	n->up = malloc(sizeof(int) * 8);
};

void clear_keys(struct input_keys *in)
{
	int i = 0;
	
	while (i<8)
		in->down[i] = KEYS_NULL;
	
	while (i<8)
		in->up[i] = KEYS_NULL;
}
