#include "allHeader.h"

/**
 * charCToFD - Write characters to a given file descriptor (fd) in a buffered manner.
 * @c: The character to be written to the file descriptor or FLASH_BUFFER.
 * @fd: The file descriptor to which the characters are written.
 * Return: 1 if successful (always).
 */
int charCToFD(char c, int fd)
{
	static int i;
	static char buf[READ_WRITE_BUF_SIZE];
	if (c == FLASH_BUFFER || i >= READ_WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != FLASH_BUFFER) buf[i++] = c;
	return (1);
}

/**
 * charInStr - Find the first occurrence of a character in a string.
 * @s: The input string to search.
 * @c: The character to find in the string.
 * Return: Pointer to the first occurrence of 'c' in 's', or NULL if 'c' is not found.
 */
char *charInStr(char *s, char c)
{
	do {
		if (*s == c) return s;
	} while (*s++ != '\0');
	return (NULL);
}

/**
 * forkCMD - Fork a new process and execute a command.
 * @info: The 'infoTable' struct containing command information.
 * Return: None.
 */
void forkCMD(infoTable *info)
{
	pid_t childPid;
	childPid = fork();
	if (childPid == -1)
	{
		perror("Error:");
		return;
	}
	if (childPid == 0)
	{
		if (execve(info->path, info->argv, getStrEnvVar(info)) == -1)
		{
			freeInfoStructFields(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				errorMessage(info, "Permission denied\n");
		}
	}
}

/**
 * strDuplicate - Create a duplicate of a given string.
 * @str: The input string to be duplicated.
 * Return: A pointer to the newly allocated string, or NULL if memory allocation fails.
 */
char *strDuplicate(const char *str)
{
	int len = 0;
	char *ret;
	if (str == NULL) return (NULL);
	while (*str++)
		len++;
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret) return (NULL);
	for (len++; len--;) ret[len] = *--str;
	return (ret);
}

/**
 * needleStartWithHaystack - Check if the needle starts with the characters of the haystack.
 * @haystack: The string to search in (haystack).
 * @needle:   The string to search for (needle).
 * Return: A pointer to the first character of 'haystack' where 'needle' starts,
 */
char *needleStartWithHaystack(const char *haystack, const char *needle)
{
	while (*needle)
	{
		if (*needle++ != *haystack++) return (NULL);
	}
	return ((char *)haystack);
}

