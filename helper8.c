#include "allHeader.h"

/**
 * ifStrToInt - Convert a string to an integer value.
 * @s: The input string to be converted to an integer.
 * Return: The converted integer value.
 */
int ifStrToInt(const char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;
	if (s == NULL)
		return (0);
	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;
		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}
	if (sign == -1)
		output = -result;
	else
		output = result;
	return (output);
}


/**
 * isExecutable - Check if the file at the given path is executable.
 * @path: The path to the file to check.
 * Return: 1 if the file is executable, 0 otherwise.
 */
int isExecutable(const char *path)
{
	struct stat st;
	if (!path) return 0;
	if (stat(path, &st) == -1) return 0;
	if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH))
		return 1;
	return 0;
}

/**
 * strCopy - Copy at most n characters from src to dest.
 * @dest: The destination buffer to copy to.
 * @src: The source buffer to copy from.
 * @n: The maximum number of characters to copy.
 * Return: A pointer to the destination string dest.
 */
char *strCopy(char *dest, const char *src, int n)
{
	int i;
	for (i = 0; i < n - 1 && src[i]; i++)
		dest[i] = src[i];
	while (i < n)
		dest[i++] = '\0';
	return dest;
}

/**
 * setMemory - Set the first n bytes of the memory block pointed to by s to the specified value b.
 * @s: Pointer to the memory block to set.
 * @b: Value to be set.
 * @n: Number of bytes to set.
 * Return: A pointer to the memory block s.
 */
char *setMemory(char *s, char b, unsigned int n)
{
    unsigned int i;
    for (i = 0; i < n; i++) s[i] = b;
    return s;
}

/**
 * freeString - Free a dynamically allocated array of strings.
 * This function frees a dynamically allocated array of strings, along with the array itself.
 * @pp: Pointer to the array of strings to be freed.
 */
void freeString(char **pp)
{
    char **a = pp;
    if (!pp) return;
    while (*pp)
    {
        free(*pp); 
        pp++;
    }
    free(a); 
}

