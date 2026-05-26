#include <stdlib.h>

#include "heap.h"

/**
 * get_node_by_index - get the node at a 1-based heap index
 * @root: root node of the heap
 * @index: 1-based node index
 *
 * Return: pointer to the indexed node, or NULL on failure
 */
static binary_tree_node_t *get_node_by_index(binary_tree_node_t *root,
	size_t index)
{
	size_t mask;

	if (!root || index < 1)
		return (NULL);

	mask = 1;
	while (mask <= index)
		mask <<= 1;
	mask >>= 2;
	while (mask > 0)
	{
		if (index & mask)
			root = root->right;
		else
			root = root->left;
		if (!root)
			return (NULL);
		mask >>= 1;
	}
	return (root);
}

/**
 * swap_data - swap the data pointers of two tree nodes
 * @first: first node
 * @second: second node
 */
static void swap_data(binary_tree_node_t *first, binary_tree_node_t *second)
{
	void *tmp;

	tmp = first->data;
	first->data = second->data;
	second->data = tmp;
}

/**
 * sift_up - restore the heap property upwards
 * @node: node to sift up from
 * @cmp: comparison function
 */
static void sift_up(binary_tree_node_t *node, int (*cmp)(void *, void *))
{
	while (node->parent && cmp(node->data, node->parent->data) < 0)
	{
		swap_data(node, node->parent);
		node = node->parent;
	}
}

/**
 * heap_insert - insert data in a min heap
 * @heap: heap to insert into
 * @data: data to insert
 *
 * Return: pointer to the created node, or NULL on failure
 */
binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
	binary_tree_node_t *node;
	binary_tree_node_t *parent;
	size_t index;

	if (!heap || !data)
		return (NULL);

	node = binary_tree_node(NULL, data);
	if (!node)
		return (NULL);

	if (!heap->root)
	{
		heap->root = node;
		heap->size++;
		return (node);
	}

	index = heap->size + 1;
	parent = get_node_by_index(heap->root, index / 2);
	if (!parent)
	{
		free(node);
		return (NULL);
	}

	node->parent = parent;
	if (index % 2 == 0)
		parent->left = node;
	else
		parent->right = node;

	sift_up(node, heap->data_cmp);
	heap->size++;
	return (node);
}
