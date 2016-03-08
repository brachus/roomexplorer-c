
#include "inc.h"
#include "str_num.h"
#include "rpg_err.h"
#include "rpg_parse_base.h"
#include "rpg_parse_token.h"

void token_init(struct token* tokens)
{
	tokens->first = NULL;
	tokens->last = NULL;
}

void print_tokens(struct token* tokens)
{
	struct token_l *tmp;
	tmp = tokens->first;
	while (tmp != NULL)
	{
		printf("%s:%d:%d: \t", tmp->fn, tmp->line, tmp->col);
		
		switch(tmp->type)
		{
		case T_INT:
			printf("int %d\n",tmp->dat_int);
			break;
		case T_FLOAT:
			printf("float %d\n",tmp->dat_int);
			break;
		case T_STR:
			printf("str ");
			str_print(tmp->dat_str[0]);
			printf("\n");
			break;
		case T_NAME:
			printf("name ");
			str_print(tmp->dat_str[0]);
			printf(" ");
			str_print(tmp->dat_str[1]);
			printf(" ");
			str_print(tmp->dat_str[2]);
			printf("\n");
			break;
		case T_SYM:
			printf("sym ");
			str_print(tmp->dat_str[0]);
			printf("\n");
			break;
		default:
			break;
		}
		
		tmp = tmp->next;
	}
	
}
/*SUSPECT*/
void add_token(struct token* tokens, int type, int line, int col, char* fname)
{
	if (!tokens->first)
	{
		tokens->first = new_token();
		tokens->last = tokens->first;
	}
	else
	{
		tokens->last->next = new_token();
		tokens->last = tokens->last->next;
	}
	
	tokens->last->type = type;
	tokens->last->line = line;
	tokens->last->col = col;
	
	strcpy(tokens->last->fn, fname);
	
}

/*SUSPECT*/
void add_cpy_token(struct token *tokens, struct token_l *tok)
{
	if (!tokens->first)
	{
		tokens->first = cpy_token(tok);
		printf("copied token");
		tokens->last = tokens->first;
	}
	else
	{
		tokens->last->next = cpy_token(tok);
		tokens->last = tokens->last->next;
	}
}

struct token_l *new_token()
{
	struct token_l *new = malloc(sizeof(struct token_l));
	
	new->dat_str = malloc(3 * (sizeof(struct str *)));
	new->dat_str[0] = create_str();
	new->dat_str[1] = create_str();
	new->dat_str[2] = create_str();
	
	new->fn = malloc(65);
	
	new->next = NULL;
	
	return new;
}

struct token_l *cpy_token(struct token_l *in)
{
	struct token_l *cpy = new_token();
	
	cpy->type = in->type;
	
	free_str(cpy->dat_str[0]);
	cpy->dat_str[0] = str_cpy(in->dat_str[0]);
	free_str(cpy->dat_str[1]);
	cpy->dat_str[1] = str_cpy(in->dat_str[1]);
	free_str(cpy->dat_str[2]);
	cpy->dat_str[2] = str_cpy(in->dat_str[2]);
	
	cpy->dat_int = in->dat_int;
	cpy->dat_float = in->dat_float;
	
	cpy->line = in->line;
	cpy->col = in->col;
	strcpy(cpy->fn, in->fn);
	
	return cpy;
	
};

void free_token_l(struct token_l *in)
{
	if (in->next != 0)
		free_token_l(in->next);
	in->next = 0;
	
	free_str(in->dat_str[0]);
	free_str(in->dat_str[1]);
	free_str(in->dat_str[2]);
	
	free(in->dat_str);
	
	free(in->fn);
	
	free(in);
}

void free_tokens(struct token *in)
{
	if (in->first != 0)
		free_token_l(in->first);
	in->first = 0;
	in->last = 0;
}

int token_nnames(struct token_l *in)
{
	int a = 0;
	if (in->type == T_NAME)
	{
		a = 1;
		if (in->dat_str[1]->length > 0 )
		{
			a = 2;
			if (in->dat_str[2]->length > 0 )
				a++;
		}
			
	}
	
	return a;
}

int token_if_sym(struct token_l *tok, char *sym)
{
		
	if (	tok->type == T_SYM &&
			str_cmp_cstr(tok->dat_str[0], sym)	)
		return 1;
	else
		return 0;
}

void parse_tokenize(struct str *fn, struct token* tokens)
{
	
	char *fname = str_to_cstr(fn);
		
	FILE *fp = fopen(fname,"r");
	
	if (!fp)
		vm_err(fname,-1,-1, "non-existent file.");
	
	struct str *macro_dat = malloc(2 * sizeof(struct str));
	int macro_idx = 0;
	
	char ch, md, ch_type;
	
	int hold, set_hold, tok_line, tok_col, tok_item_idx;
	
	tok_line = 1;
	tok_col = 0;
	
	hold = 0;
	set_hold = 0;
	
	tok_item_idx = 0;
	
	md = P_OPEN;
		
	while (ch != EOF)
	{
		if (hold == 0)
		{
			ch = getc(fp);
			ch_type = char_identify(ch);
		}
		
		if (ch == EOF)
			break;
		
				
		switch (md)
		{
		case P_OPEN:
			switch (ch_type)
			{
			case NUMERAL:
				add_token(
					tokens,
					T_INT,
					tok_line,
					tok_col,
					fname );
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
					/* append char to string data in newly created token. */
				
				md = P_INT;
				break;
			case SYMBOL:
				if (ch == '#')
					md = P_MACRO;
				else if (ch == '/')
					md = P_COM_GET_STAR;
				else
				{
					add_token(
						tokens,
						T_SYM,
						tok_line,
						tok_col,
						fname );
					str_append_char( tokens->last->dat_str[tok_item_idx], ch);
					
					if (ch == '.')
						md = P_DOT;
				}
				break;
			case LETTER:
				
				add_token(
					tokens,
					T_NAME,
					tok_line,
					tok_col,
					fname );
				str_append_char( tokens->last->dat_str[0], ch);
				md = P_NAME;
				
				
				break;
			case QUOTE:
				add_token(
					tokens,
					T_STR,
					tok_line,
					tok_col,
					fname );
				
				if (ch == '\'')
					md = P_SINGLEQUOTE;
				else
					md = P_DOUBLEQUOTE;
				
				break;
			default:
				break;
			}
			break;
			
		case P_INT:
			if (ch_type == NUMERAL)
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			else if (ch_type == SYMBOL)
			{
				if (ch == '.')
				{
					str_append_char( tokens->last->dat_str[tok_item_idx], ch);
					tokens->last->type = T_FLOAT;
					md = P_FLOAT;
				}
				else
				{
					tokens->last->dat_int = atoi(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
						/* convert string data in token to integer data */
					tok_item_idx = 0;
					
					set_hold = 1; /* reread char to get symbol token. */
					
					md = P_OPEN;
				}
			}
			else if (ch_type == LETTER)
				vm_err(fname, tok_line, tok_col, "expected numeral, got letter.");
			else
			{
				tokens->last->dat_int =atoi(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
				tok_item_idx = 0;
				set_hold = 1;	
				md = P_OPEN;
			}
			break;
		case P_NAME:
			if (ch_type == LETTER || ch_type == NUMERAL)
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			else if (ch_type == SYMBOL)
			{
				if (ch == '.')
				{
					if (tok_item_idx == 2)
						vm_err(fname, tok_line, tok_col, "too many sub names.");
					
					tok_item_idx++; /* move on to next str dat in token*/
					
					md = P_NAME_DOT;
				}
				else
				{
					tok_item_idx = 0;
					set_hold = 1;
					md = P_OPEN;
				}
			}
			else
			{
				tok_item_idx = 0;
				set_hold = 1;
				md = P_OPEN;
			}
			break;
		case P_NAME_DOT:
			if (ch_type == LETTER)
			{
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
				md = P_NAME;
			}
			else 
				vm_err(fname, tok_line, tok_col, "unexpected name after \".\"");
			break;
		case P_DOT:
			if (ch_type == LETTER)
			{
				tokens->last->type = T_NAME;
				
				 /* for ".xx", have first string be only a null character*/
				tokens->last->dat_str[0]->first->dat = '\0';
				
				tok_item_idx++;
				
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
				
				md = P_NAME;
			}
			else if (ch_type == NUMERAL)
			{
				tokens->last->type = T_FLOAT;
				
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
				
				md = P_FLOAT;
			}
			else
			{
				set_hold = 1;
				md = P_OPEN;
			}
			break;
		case P_DOUBLEQUOTE:
			if (ch == '\"')
				md = P_OPEN;
			else if (ch == '\\')
				md = P_DOUBLEQUOTE_ESC;
			else
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			break;
		case P_SINGLEQUOTE:
			if (ch == '\'')
				md = P_OPEN;
			else if (ch == '\\')
				md = P_SINGLEQUOTE_ESC;
			else
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			break;
		case P_DOUBLEQUOTE_ESC:
			if (ch == '\"' || ch == '\'')
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			md = P_DOUBLEQUOTE;
			break;
		case P_SINGLEQUOTE_ESC:
			if (ch == '\"' || ch == '\'')
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			md = P_SINGLEQUOTE;
			break;
		case P_FLOAT:
			if (ch_type == NUMERAL)
				str_append_char( tokens->last->dat_str[tok_item_idx], ch);
			else if (ch_type == SYMBOL)
			{
				if (ch == '.')
					vm_err(fname, tok_line, tok_col, "too many dots in float.");
				else
				{
					tokens->last->dat_float = atof(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
						/* convert string data in token to float data */
					tok_item_idx = 0;
					
					set_hold = 1; /* reread char to get symbol token. */
					
					md = P_OPEN;
				}
			}
			else if (ch_type == LETTER)
				vm_err(fname, tok_line, tok_col, "expected numeral, got letter.");
			else
			{
				tokens->last->dat_float = atof(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
				tok_item_idx = 0;
				set_hold = 1;	
				md = P_OPEN;
			}
			break;
		case P_MACRO:
			if (ch == ' ')
			{
				macro_idx++;
				if (macro_idx > 1)
					vm_err(fname, tok_line, tok_col, "too many names for macro");
			}
			else if (ch == '\n')
			{
								
				if (str_cmp_cstr(&macro_dat[0], "include") && macro_idx == 1)
					parse_tokenize(&macro_dat[1], tokens);
				else
					vm_err(fname, tok_line, tok_col, "bad macro identifier.");
								
				macro_idx = 0;
				
				md = P_OPEN;
				
				str_del(&macro_dat[0]);
				str_del(&macro_dat[1]);
				
			}
			else
				str_append_char( &(macro_dat[macro_idx]), ch);
			
			break;
		case P_COM_GET_STAR:
			
			if (ch == '*')
				md = P_COMMENT;
			else
			{
				vm_warn(fname, tok_line, tok_col, "ignoring stray \"/\".");
				md = P_OPEN;
			}
			
			break;
		case P_COMMENT:
		
			if (ch == '*')
				md = P_COM_GET_SLASH;
			
			break;
		case P_COM_GET_SLASH:
			
			if (ch == '/')
				md = P_OPEN;
			else if (ch == '*')
				md = P_COM_GET_SLASH;
				/*no change*/
			else
				md = P_COMMENT;
			
			break;
		default:
			break;
		}
		
		if (hold == 0)
		{
			tok_col++;
			
			if (ch == '\n')
			{
				tok_col = 0;
				tok_line++;
			}
		}
		else
			hold = 0;
		if (set_hold)
		{
			hold = 1;
			set_hold = 0;
		}
		
	}
	
	/* if failure hasn't happened, address state of parse_mode at EOF */
    switch (md)
    {
	case P_INT:
		tokens->last->dat_int = atoi(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
		break;
		
	case P_FLOAT:
		tokens->last->dat_float = atof(str_to_cstr(tokens->last->dat_str[tok_item_idx]));
		break;
	
	case P_NAME_DOT:
	case P_DOT:
	case P_DOUBLEQUOTE:
	case P_SINGLEQUOTE:
		vm_err(fname, tok_line, tok_col, "unexpected EOF.");
		break;
		
	default:
		break;
    }
    
    fclose(fp);

}
