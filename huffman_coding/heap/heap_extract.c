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
 * smaller_child - get the child with the smallest data
 * @node: node whose children are inspected
 * @cmp: comparison function
 *
 * Return: pointer to the smallest child, or NULL if no child exists
 */
static binary_tree_node_t *smaller_child(binary_tree_node_t *node,
	int (*cmp)(void *, void *))
{
	if (!node->left && !node->right)
		return (NULL);
	if (!node->right)
		return (node->left);
	if (!node->left)
		return (node->right);
	if (cmp(node->left->data, node->right->data) <= 0)
		return (node->left);
	return (node->right);
}

/**
 * sift_down - restore the heap property downwards
 * @node: node to sift down from
 * @cmp: comparison function
 */
static void sift_down(binary_tree_node_t *node, int (*cmp)(void *, void *))
{
	binary_tree_node_t *child;

	child = smaller_child(node, cmp);
	while (child && cmp(child->data, node->data) <= 0)
	{
		swap_data(node, child);
		node = child;
		child = smaller_child(node, cmp);
	}
}

/**
 * heap_extract - extract the root value of a min heap
 * @heap: heap to extract from
 *
 * Return: extracted data pointer, or NULL on failure
 */
void *heap_extract(heap_t *heap)
{
	binary_tree_node_t *last;
	binary_tree_node_t *parent;
	void *data;
	size_t index;

	if (!heap || !heap->root)
		return (NULL);

	data = heap->root->data;
	if (heap->size == 1)
	{
		free(heap->root);
		heap->root = NULL;
		heap->size = 0;
		return (data);
	}

	index = heap->size;
	last = get_node_by_index(heap->root, index);
	if (!last)
		return (NULL);

	parent = last->parent;
	if (parent->left == last)
		parent->left = NULL;
	else
		parent->right = NULL;

	heap->root->data = last->data;
	free(last);
	heap->size--;
	sift_down(heap->root, heap->data_cmp);
	return (data);
}
