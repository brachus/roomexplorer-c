

#define NUM_INT 0
#define NUM_FLOAT 1

struct str_l
{
	struct str_l *next;
	char dat;
};

struct str
{
	int length;
	struct str_l *first;
	struct str_l *last;
};

struct num
{
	int num_int;
	float num_float;
	int numtype;
};

struct str new_str(char *in);
struct str *create_str();
struct str *new_str_from_cstr(char *in);
void str_append_char(struct str* in_str, char in);
void str_append_cstr(struct str* in_str, char *in);
void str_append_int(struct str* tstr, int tint);
void str_append_float(struct str* tstr, float tfloat);
void str_append_str(struct str *in, struct str *in2);
void str_print(struct str* in);
char str_idx(struct str* in, int idx);
char *str_to_cstr(struct str* in);
void str_del(struct str *in);
void free_str(struct str *in);
void str_init(struct str *in);
int str_cmp(struct str *a, struct str *b);
int str_cmp_cstr(struct str *a, char *b);
struct str *str_cpy(struct str *in);
int chtoi(char ch);
