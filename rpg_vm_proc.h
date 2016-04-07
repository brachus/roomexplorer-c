

/* return vals for vm_proc_step. */
#define PR_STEP 0
#define PR_TERM 1
#define PR_TERMVM 2
#define PR_NEXTSUB 3

struct asub_i
{
	int mode; /* script mode.  uses S_INIT, S_BODY, S_TERM predefs */
	struct obj *ob; /* obj of focus. */
	struct func *pc; /* pointer to next function to execute. */
	struct asub_i *next;
	int timer;
};

struct asub_dat
{
	struct asub_i *first;
	struct asub_i *last;
};

struct fad_dat
{
	struct fad_i *first;
	struct fad_i *last;
};

struct fad_i
{
	int type; /* uses variable type defs */
	
	struct var *target;
	
	struct var *incr;
	
	struct var *goal;
	
	int frames;
	
	struct fad_i *next;
};

int var_get_int(struct var *in);
struct str *var_get_str(struct var *in);
void set_var_to_idnt(struct var *vtmp, struct idnt *itmp, struct var **regs);
struct var *get_var_from_varstr_obj(struct obj *otmp, struct str *vname);
struct str *get_str_from_arg(struct idnt *arg, struct var **regs);
int get_int_from_arg(struct idnt *arg, struct var **regs);
float get_float_from_arg(struct idnt *arg, struct var **regs);
struct obj *get_objpntr_from_arg(struct idnt *arg);
struct var *get_var_from_arg(struct idnt *arg, struct var **regs);
int get_nargs(struct idnt *args);
struct var **init_regs();
struct asub_dat *new_asub_dat();
void add_asub_i(struct asub_dat *in, struct obj *obj_in);
void rm_asub_i(struct asub_dat *in, struct asub_i *s);
void add_asub_main(struct asub_dat *in, struct obj_dat *objd_in);
int do_int_cmp(int cmp_op, int a, int b);
int do_str_cmp(int cmp_op, struct str*a, struct str *b);
void obj_add_def(struct obj_dat *main, struct obj_dat *defs);
int vm_proc_step(struct asub_i *in, struct obj_dat *odat, struct var **regs, struct input_keys *keys, struct fad_dat *fdat);
int vm_proc_full(struct asub_dat *in, struct obj_dat *odat, struct var **regs, struct input_keys *keys, struct fad_dat *fdat);

int update_actors(struct obj *omain, struct rpg_render *rndr);
void do_mod(struct obj *omain);

struct fad_i *new_fad_i();
struct fad_dat *new_fad_dat();
void add_fad_i(struct fad_dat *in, struct var *target, struct var *goal, int frames);
void proc_fad_dat(struct fad_dat *in);
