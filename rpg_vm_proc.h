

/* return vals for vm_proc_step. */
#define PR_STEP 0
#define PR_TERM 1


struct asub_i
{
	int mode; /* script mode.  uses S_INIT, S_BODY, S_TERM predefs */
	struct obj *ob; /* obj of focus. */
	struct func *pc; /* pointer to next function to execute. */
	struct asub_i *next;
};

struct asub_dat
{
	struct asub_i *first;
	struct asub_i *last;
};


struct asub_dat *new_asub_dat();
void add_asub_i(struct asub_dat *in, struct obj *obj_in);
void add_asub_main(struct asub_dat *in, struct obj_dat *objd_in);
