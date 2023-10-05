#pragma once
#ifndef MEM_H__
#define MEM_H__

#ifndef HEAP_SIZE
#define HEAP_SIZE 1024
#endif // !HEAP_SIZE

void* my_mem_alloc(int size);
void my_mem_free(void* ptr);

#endif // !MEM_H__