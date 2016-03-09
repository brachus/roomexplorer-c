
#include "inc.h"
#include "str_num.h"


int vm_err_do_warn = 1;

void vm_err_prefix(char* fn, int line, int col)
{
	if (fn != NULL)
		printf("%s:", fn);
	if (line >= 1)
		printf("%d:", line);
	if (col >= 0)
		printf("%d:", col);
}

void vm_warn(char* fn, int line, int col, char* msg)
{
	if (vm_err_do_warn)
	{
		if (line != 0)
			vm_err_prefix(fn, line, col);
		printf(" warning: %s\n", msg);
	}
}

void vm_err(char* fn, int line, int col, char* msg)
{
	if (line != 0)
		vm_err_prefix(fn, line, col);
	printf(" error: %s\n", msg);
	
	exit(1);
}
