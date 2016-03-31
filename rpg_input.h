

/* create functions for updating input_keys struct
 * after each even poll.
 */

#define KEYS_NULL 0

struct input_keys
{
	unsigned int *hold;
	unsigned int *down; /* just pressed down */
	unsigned int *up;/* just released */
};


struct input_keys *new_input_keys();
void clear_keys(struct input_keys *in);
int check_key(unsigned int *keys, unsigned int key);
void rm_key(unsigned int *keys, unsigned int key);
void add_key_down(struct input_keys *in, unsigned int key);
void add_key_up(struct input_keys *in, unsigned int key);
void handle_events(struct input_keys *in);
void print_keys(struct input_keys *in);
int keys_match_str(unsigned int *keys, struct str *in);
