
#include "inc.h"
#include "str_num.h"
#include "rpg_parse_base.h"




int is_numeral(char ch)
{
	static char * NUMERALS = "0123456789";
	int i;
	for (i=0;NUMERALS[i]!='\0';i++)
		if (ch == NUMERALS[i])
			return 1;
	return 0;
}

int is_symbol(char ch)
{
	static char * SYMBOLS = "{}[]<>().,=;:-#/*\\";
	int i;
	for (i=0;SYMBOLS[i]!='\0';i++)
		if (ch == SYMBOLS[i])
			return 1;
	return 0;
}

int is_quote(char ch)
{
	static char * QUOTES = "\'\"";
	int i;
	for (i=0;QUOTES[i]!='\0';i++)
		if (ch == QUOTES[i])
			return 1;
	return 0;
}

int is_letter(char ch)
{
	static char * LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
	int i;
	for (i=0;LETTERS[i]!='\0';i++)
		if (ch == LETTERS[i])
			return 1;
	return 0;
}

int is_whitespace(char ch)
{
	static char * WHITESPACES = " \t\n";
	int i;
	for (i=0;WHITESPACES[i]!='\0';i++)
		if (ch == WHITESPACES[i])
			return 1;
	return 0;
}

int char_identify(char ch)
{
    if (is_numeral(ch))
        return NUMERAL;
    if (is_symbol(ch))
        return SYMBOL;
    if (is_quote(ch))
        return QUOTE;
    if (is_letter(ch))
        return LETTER;
    if (is_whitespace(ch))
        return WHITESPACE;
    return -1;
};


int parse_ret_reg( struct str *in )
{
	if (in->length == 2 )
		if ( str_idx(in,0) == 'r' && is_numeral(str_idx(in,1)) )
			return chtoi(str_idx(in,1));
	return -1;
};


