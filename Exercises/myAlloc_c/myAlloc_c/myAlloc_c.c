/*
	myMalloc.c

	Created on: 27.09.2023
*/

#include <stdio.h>
#include <inttypes.h>
#include "myMem.h"

int main(int argc, char* argv[]) {

	char* x = my_mem_alloc(1);
	*x = 'X';

	char* y = my_mem_alloc(1);
	*y = 'Y';

	return 0;
}
