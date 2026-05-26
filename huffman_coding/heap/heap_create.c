#include <stdlib.h>

#include "heap.h"

/**
 * heap_create - create a new heap
 * @data_cmp: comparison function for node data
 *
 * Return: pointer to the created heap, or NULL on failure
 */
heap_t *heap_create(int (*data_cmp)(void *, void *))
{
	heap_t *heap;

	if (!data_cmp)
		return (NULL);

	heap = malloc(sizeof(heap_t));
	if (!heap)
		return (NULL);

	heap->size = 0;
	heap->data_cmp = data_cmp;
	heap->root = NULL;
	return (heap);
}
