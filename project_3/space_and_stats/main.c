#include "get_stats.h"
#include "get_space.h"
#include <stdio.h>

int main(int argc, char const *argv[]){
	int size = 10000;
	int i;
	unsigned long before_alloc = slob_get_total_alloc_mem_wrapper();
	unsigned long before_free = slob_get_total_free_mem_wrapper();
	unsigned long after_alloc;
	unsigned long after_free;

	printf("Before - Total allocated memory: %lu\n", before_alloc);
	printf("Before - Total free memory: %lu\n", before_free);

	for (i = 0; i < 10000; ++i){
		my_kmalloc_mem_wrapper(size);
	}

	after_alloc = slob_get_total_alloc_mem_wrapper();
	after_free = slob_get_total_free_mem_wrapper();

	printf("After - Total allocated memory: %lu\n",after_alloc);
	printf("After - Total free memory: %lu\n", after_free);

	printf("Diff free: %ld\n", after_free - before_free);
	printf("Diff alloc: %ld\n", after_alloc - before_alloc);
	return 0;
}