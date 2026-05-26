#include <stdlib.h>

#include "huffman.h"

/**
 * symbol_create - create a symbol structure
 * @data: character stored in the symbol
 * @freq: symbol frequency
 *
 * Return: pointer to the created symbol, or NULL on failure
 */
symbol_t *symbol_create(char data, size_t freq)
{
	symbol_t *symbol;

	symbol = malloc(sizeof(symbol_t));
	if (!symbol)
		return (NULL);

	symbol->data = data;
	symbol->freq = freq;
	return (symbol);
}
