
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
	in_str->last->next = NULL;
	
	in_str->length++;
}

void str_append_cstr(struct str* in_str, char *in)
{
	int i;
	for (i=0; in[i] != '\0';i++)
		str_append_char(in_str, in[i]);
}

void str_print(struct str* in)
{
	struct str_l *tmp = in->first;
	
	while (tmp != NULL)
	{
		printf("%c",tmp->dat);
		tmp = tmp->next;
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

int str_cmp_cstr(struct str *a, char *b)
{
	struct str_l *tmpa;
	int tmpb = 0;
	
	tmpa = a->first;
	
	while (b[tmpb] != '\0' && tmpa != NULL)
	{
		if (tmpa->dat != b[tmpb])
			return 0;
		
		tmpa = tmpa->next;
		tmpb++;
	}
	
	
	return 1;
}

struct str *str_cpy(struct str *in)
{
	struct str *cpy = create_str();
	struct str_l *tmp = in->first;
	
	while (tmp != NULL)
	{
		str_append_char(cpy, tmp->dat);
		tmp = tmp->next;
	}
	
	return cpy;
}

int chtoi(char ch)
{
	char tmp[2];
	tmp[0] = ch;
	return atoi(tmp);
}

