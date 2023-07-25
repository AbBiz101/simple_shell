#include "allHeader.h"

/**
 * printInputStr_0 - Print an input string to the standard error stream (STDERR).
 * @str: Pointer to the input string to be printed.
 */
void printInputStr_0(char *str)
{
	int i = 0;
	if (!str) return;
	while (str[i] != '\0')
	{
		charCToStderr(str[i]);
		i++;
	}
}

/**
 * isInteractive - Check if the shell is running in interactive mode.
 * @info: Pointer to the structure containing context information.
 * Return: 1 (true) if the shell is running in interactive mode, 0 (false) otherwise.
 */
int isInteractive(infoTable *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * splitString - Split a string into an array of strings based on delimiters.
 * @str: Pointer to the input string to be split.
 * @d: Pointer to the delimiter string. If NULL, the default delimiter is a space " ".
 * Return: Pointer to the array of strings (string array) resulting from the split operation.
 */
char **splitString(char *str, char *d)
{
	int i, j, k, m, numWords = 0;
	char **s;
	if (str == NULL || str[0] == '\0') return (NULL);
	if (!d)
		d = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!isDelimiter(str[i], d) && (isDelimiter(str[i + 1], d) || !str[i + 1]))
			numWords++;
	if (numWords == 0) return (NULL);
	s = malloc((1 + numWords) * sizeof(char *));
	if (!s) return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (isDelimiter(str[i], d))
			i++;
		k = 0;
		while (!isDelimiter(str[i + k], d) && str[i + k])
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = '\0';
	}
	s[j] = NULL;
	return (s);
}

/**
 * charCToStderr - Write characters to standard error (stderr) in a buffered manner.
 * @c: The character to be written to stderr or FLASH_BUFFER (special character).
 * Return: 1 if successful (always), -1 on error (e.g., if write fails).
 */
int charCToStderr(char c)
{
	static int i;
	static char buf[READ_WRITE_BUF_SIZE];
	if (c == FLASH_BUFFER || i >= READ_WRITE_BUF_SIZE)
	{
		ssize_t numWritten = write(2, buf, i);
		if (numWritten == -1)
			return -1;
		i = 0;
	}
	if (c != FLASH_BUFFER) buf[i++] = c;
	return 1;
}

/**
 * isDelimiter - Check if a character is a delimiter.
 * @c: The character to check if it is a delimiter.
 * @delim: The string containing delimiter characters.
 * Return: 1 if 'c' is a delimiter, 0 otherwise.
 */
int isDelimiter(char c, char *delim)
{
	while (*delim)
	{
		if (*delim == c) return 1;
		delim++;
	}
	return 0;
}


