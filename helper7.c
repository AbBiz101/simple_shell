#include "allHeader.h"

/**
 * concatTowStr - Concatenate two strings.
 * @dest: The destination string where 'src' will be appended.
 * @src:  The source string that will be appended to 'dest'.
 * Return: A pointer to the resulting concatenated string (the modified 'dest').
 */
char *concatTowStr(char *dest, const char *src)
{
	char *ret = dest;
	while (*dest) dest++;
	while (*src) *dest++ = *src++;
	*dest = '\0';
	return (ret);
}

/**
 * printStr - Print a string to the standard output (stdout).
 * output (stdout).
 * @str: The input string to be printed.
 */
void printStr(const char *str)
{
 if (!str) return;
    while (*str != '\0')
    {
        write(STDOUT_FILENO, str, 1);
        str++;
    }
}

/**
 * isAlpha - Check if the given character is an alphabetical character (letter).
 * @c: The character to be checked.
 * Return: 1 if 'c' is an alphabetical character, 0 otherwise.
 */
int isAlpha(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    else
        return 0;
}

/**
 * printInputStr - Print the input string to the specified file descriptor.
 * @str: The input string to be printed.
 * @fd: The file descriptor where the string will be printed.
 * Return: The total number of characters written to the file descriptor.
 */
int printInputStr(const char *str, int fd)
{
	int i = 0;
	if (!str) return 0;
	while (*str)
	{
		i += charCToFD(*str++, fd);
	}
	return i;
}

/**
 * concatTwoStr - Concatenate n characters from src to the end of dest.
 * @dest: The destination string where the concatenation will occur.
 * @src: The source string from which characters will be concatenated.
 * @n: The maximum number of characters to be concatenated from src.
 * Return: A pointer to the modified dest string.
 */
char *concatTwoStr(char *dest, const char *src, int n)
{
	int i, j;
	char *s = dest;
	i = 0;
	while (dest[i] != '\0')
		i++;
	for (j = 0; j < n && src[j] != '\0'; j++)
	{
		dest[i] = src[j];
		i++;
	}
	dest[i] = '\0';
	return (s);
}

