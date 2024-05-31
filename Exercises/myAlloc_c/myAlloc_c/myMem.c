/*
	myMem.c

	Created on: 27.09.2023
*/

#include "myMem.h"
#include <inttypes.h>
#include <stdio.h>

#define FALSE 'F' // 'F' for false -> allocated

static uint8_t heap[HEAP_SIZE] = { 0 };
typedef struct {
	uint8_t size;
	char isFree;
} memHeader_t;

void* my_mem_alloc(int size) {
	memHeader_t* pHeader = (memHeader_t*)&heap[0];

	if (pHeader->isFree != NULL && pHeader->size != NULL)
	{
		while (pHeader->isFree != NULL && pHeader->isFree == FALSE)
		{
			pHeader = (uint8_t*)pHeader + pHeader->size;
		}
	}

	pHeader->size = (uint8_t)size + sizeof(memHeader_t);
	pHeader->isFree = FALSE;

	return (uint8_t*)pHeader + sizeof(memHeader_t);
}

void my_mem_free(void* ptr) {
	// TODO
}