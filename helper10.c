#include "allHeader.h"

/**
 * getStrEnvVar - Returns an array of strings containing environment variables.
 * @info: Pointer to the info struct.
 * Return: Pointer to the array of strings.
 */
char **getStrEnvVar(infoTable *info)
{
	if (!info->environ || info->envChanged)
	{
		freeString(info->environ);
		info->environ = listStringTypeToArray(info->env);
		info->envChanged = 0;
	}
	return (info->environ);
}

/**
 * findCMDPath - Find and execute the command by searching in PATH variable.
 * @info: Pointer to the info struct.
 * Return: void.
 */
void findCMDPath(infoTable *info)
{
	char *path = NULL;
	info->path = info->argv[0];
	if (info->lineCountFlag == 1)
	{
		info->lineCount++;
		info->lineCountFlag = 0;
	}
	if (info->arg == NULL || *(info->arg) == '\n') return;

	path = pathFind(info, valueOfEnvVar(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forkCMD(info);
	}
	else
	{
		if ((isInteractive(info) || valueOfEnvVar(info, "PATH=") || info->argv[0][0] == '/')
		    && isExecutable(info, info->argv[0]))
		{
			forkCMD(info);
		}
		else
		{
			info->status = 127;
			errorMessage(info, "not found\n");
		}
	}
}

/**
 * charCToStdout - Write the character c to the standard output.
 * @c: The character to be written.
 * Return: 1 on success.
 */
int charCToStdout(char c)
{
	static int i;
	static char buf[READ_WRITE_BUF_SIZE];
	if (c == FLASH_BUFFER || i >= READ_WRITE_BUF_SIZE)
	{
		write(STDOUT_FILENO, buf, i);
		i = 0;
	}
	if (c != FLASH_BUFFER) buf[i++] = c;
	return (1);
}

/**
 * duplicateChar - Create a new string that is a duplicate of a portion of the input string.
 * @pathstr: The input string.
 * @start: The starting index of the portion to duplicate.
 * @stop: The ending index of the portion to duplicate.
 * Return: A new string that is a duplicate of the specified portion.
 */
char *duplicateChar(const char *pathstr, int start, int stop)
{
	int i, k = 0;
	char *buf;
	if (start >= stop) return NULL;
	buf = malloc((stop - start + 1) * sizeof(char));
	if (!buf) return NULL;
	for (i = start; i < stop; i++)
	{
		if (pathstr[i] != ':') buf[k++] = pathstr[i];
	}
	buf[k] = '\0';
	return (buf);
}

/**
 * findBuiltinCMD - Find and execute a built-in command.
 * @info: The infoTable structure containing command and arguments.
 * Return: The return value of the executed built-in command.
 */
int findBuiltinCMD(infoTable *info)
{
	int i, built_in_ret = -1;
	typedef struct {
		char *type;
		int (*func)(infoTable *info);
	} builtinTable;
	builtinTable builtintbl[] = {
		{"exit", shellExit},
		{"env", currentEnvVar},
		{"help", shellHelp},
		{"history", historyList},
		{"setenv", newEnvVer},
		{"unsetenv", deleteEnvVar},
		{"cd", changeCD},
		{"alias", mimicAliasBuiltin},
		{NULL, NULL}
	};
	for (i = 0; builtintbl[i].type; i++)
	{
		if (strCompar(info->argv[0], builtintbl[i].type) == 0)
		{
			info->lineCount++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	}
	return (built_in_ret);
}

