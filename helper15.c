#include "allHeader.h"

/**
 * stringReplace - Replaces the content of a string pointer with a new string.
 * @old: Pointer to the old string (will be freed).
 * @new: New string to replace the old one.
 * Return: 1 on success, 0 if new string allocation fails.
 */
int stringReplace(char **old, char *new)
{
	free(*old);
	*old = new;
	return (new != NULL); 
}

