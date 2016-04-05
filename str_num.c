
#include "inc.h"
#include "str_num.h"


/* create struct str from c string. */
struct str new_str(char *in)
{
	int i;
	
	struct str a;
	a.length = 0;

	for (i=0;in[i] != '\0';i++)
	{
		if (i==0)
		{
			a.first = malloc( sizeof(struct str_l) );
			a.last = a.first;
		}
		else
		{
			a.last->next = malloc( sizeof(struct str_l) );
			a.last = a.last->next;
		}
		
		a.last->dat = in[i];
		a.length++;
	}
	
	a.last->next = NULL;
			
	return a;
};

/* this should eventually replace new_str() */
struct str *create_str()
{
	struct str *n;
	
	n = malloc(sizeof(struct str));
	
	str_init(n);
	
	return n;
};

struct str *new_str_from_cstr(char *in)
{
	struct str *n = create_str();
	
	str_append_cstr(n, in);
	
	return n;
};

void str_append_char(struct str* in_str, char in)
{
	if (!in_str->first || in_str->length == 0)
	{
		in_str->first = malloc( sizeof(struct str_l) );
		in_str->last = in_str->first;
		
		in_str->length = 0;
	}
	else
	{
		in_str->last->next = malloc( sizeof(struct str_l) );
		in_str->last = in_str->last->next;
	}
		
	in_str->last->dat = in;
	in_str->last->next = 0;
	
	in_str->length++;
}

void str_append_cstr(struct str* in_str, char *in)
{
	int i;
	for (i=0; in[i] != '\0';i++)
		str_append_char(in_str, in[i]);
}

void str_append_int(struct str* tstr, int tint)
{
	char *c = malloc(32);
	
	sprintf(c, "%d", tint);
	
	str_append_cstr(tstr, c);
	
	free(c);
}

void str_append_float(struct str* tstr, float tfloat)
{
	char *c = malloc(32);
	
	sprintf(c, "%f", tfloat);
	
	str_append_cstr(tstr, c);
	
	free(c);
}

void str_append_str(struct str *in, struct str *in2)
{
	if (!in || !in2)
		return;
		
	struct str_l *tmp = in2->first;
	
	while (tmp != 0)
	{
		str_append_char(in, tmp->dat);
		tmp = tmp->next;
	}
}

void str_print(struct str* in)
{
	if (!in)
		return;
	
	struct str_l *c = in->first;
	
	while (c != 0)
	{
		printf("%c", c->dat);
		c = c->next;
	}
		
}

char str_idx(struct str* in, int idx)
{
	int i=0;
	struct str_l* tmp = in->first;
	
	while (tmp != NULL)
	{
		if (idx == i)
			return tmp->dat;
		tmp = tmp->next;
		i++;
	}
	
	return '\0';
}

char *str_to_cstr(struct str* in)
{
	char *out = malloc(in->length);
	int i = 0;
	struct str_l *tmp = in->first;
	
	while (i < in->length && tmp != 0)
	{
		out[i] = tmp->dat;
		tmp = tmp->next;
		i++;
	}
	
	out[i] = '\0';
	
	return out;
}

void str_del(struct str *in)
{
	struct str_l *tmp;
	struct str_l *n;
	
	tmp = in->first;
	
	while (tmp != 0)
	{
		n = tmp->next;
		free(tmp);
		tmp = n;
	}
	
	str_init(in);
	
}

void free_str(struct str *in)
{
	if (!in)
		return;
	str_del(in);
	free(in);
}

void str_init(struct str *in)
{
	in->length = 0;
	in->first = NULL;
	in->last = NULL;
}

int str_cmp(struct str *a, struct str *b)
{
	struct str_l *tmpa;
	struct str_l *tmpb;
	
	tmpa = a->first;
	tmpb = b->first;
	
	while (tmpa != NULL && tmpb != NULL)
	{
		if (tmpa->dat != tmpb->dat)
			return 0;
		
		if (tmpa == a->last && tmpb == b->last)
			return 1;
		
		tmpa = tmpa->next;
		tmpb = tmpb->next;
	}
	
	return 0;
}

/* watch this ... */
int str_cmp_cstr(struct str *a, char *b)
{
	struct str_l *tmpa;
	int tmpb = 0;
	
	tmpa = a->first;
	
	while (b[tmpb] != '\0' && tmpa != 0)
	{
		if (tmpa->dat != b[tmpb])
			return 0;
			
		if (b[tmpb+1] == '\0' && !tmpa->next )
			return 1;
		
		tmpa = tmpa->next;
		tmpb++;
	}
	
	return 0;
}

struct str *str_cpy(struct str *in)
{
	if (!in) /* ? */
		return 0;
		
	struct str *cpy = create_str();
	struct str_l *tmp = in->first;
	
	while (tmp != 0)
	{
		str_append_char(cpy, tmp->dat);
		tmp = tmp->next;
	}
	
	return cpy;
}

int chtoi(char ch)
{
	switch(ch)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return -1;
	}
}

