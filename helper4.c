#include "allHeader.h"

/**
 * infoStructClear - Clear certain fields of the 'info' structure.
 * @info: Pointer to the structure containing context information.
 */
void infoStructClear(infoTable *info)
{
    info->arg = NULL;
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0; 
}

/**
 * changeCD - Handle the "cd" (change directory) command in the shell program.
 * @info: Pointer to the structure containing context information.
 * Return: Always 0 to indicate successful execution of the "cd" command.
 */
int changeCD(infoTable *info)
{
    char *s, *dir, buffer[1024];
    int chdirRet;
    s = getcwd(buffer, 1024);
    if (!s) printStr(">>failure<<\n");
    if (!info->argv[1])
    {
        dir = valueOfEnvVar(info, "HOME=");
        if (!dir)
            chdirRet = chdir((dir = valueOfEnvVar(info, "PWD=")) ? dir : "/");
        else chdirRet = chdir(dir);
    }
    else if (strCompar(info->argv[1], "-") == 0)
    {
        if (!valueOfEnvVar(info, "OLDPWD="))
        {
            printStr(s);
            charCToStdout('\n');
            return 1;
        }
        printStr(valueOfEnvVar(info, "OLDPWD="));
        charCToStdout('\n');
        chdirRet = chdir((dir = valueOfEnvVar(info, "OLDPWD=")) ? dir : "/");
    }
    else  chdirRet = chdir(info->argv[1]);
    if (chdirRet == -1)
    {
        errorMessage(info, "can't cd to ");
        printInputStr_0(info->argv[1]);
        charCToStderr('\n');
    }
    else
    {
        envVarInit(info, "OLDPWD", valueOfEnvVar(info, "PWD="));
        envVarInit(info, "PWD", getcwd(buffer, 1024));
    }
    return 0; 
}

/**
 * infoStructInitialize - Initialize the 'info' structure with command-line arguments.
 * @info: Pointer to the structure containing context information.
 * @av: Array of strings containing command-line arguments passed to the shell program.
 */
void infoStructInitialize(infoTable *info, char **av)
{
    int i = 0;
    info->fname = av[0]; 
    if (info->arg)
    {
        info->argv = splitString(info->arg, " \t");
        if (!info->argv)
        {
            info->argv = malloc(sizeof(char *) * 2);
            if (info->argv)
            {
                info->argv[0] = strDuplicate(info->arg);
                info->argv[1] = NULL;
            }
        }
        for (i = 0; info->argv && info->argv[i]; i++)
        info->argc = i;
        aliasReplace(info);
        varsReplace(info);
    }
}

/**
 * errorMessage - Print an error message to the standard error stream (STDERR).
 * @info: Pointer to the structure containing context information.
 * @estr: String containing the specified error type to be printed.
 */
void errorMessage(infoTable *info, char *estr)
{
    printInputStr_0(info->fname);
    charCToFD(':', STDERR_FILENO);
    intPrinter(info->lineCount, STDERR_FILENO);
    charCToFD(':', STDERR_FILENO);
    printInputStr_0(info->argv[0]);
    charCToFD(':', STDERR_FILENO);
    printInputStr_0(estr);
}

/**
 * **splitStringToWords 
 * @str: 
 * @d: 
 * Return: 
 */
char **splitStringToWords(char *str, char d)
{
	int i, j, k, m, numWords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
		    (str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numWords++;
	if (numWords == 0)
		return (NULL);
	s = malloc((1 + numWords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (str[i] == d && str[i] != d)
			i++;
		k = 0;
		while (str[i + k] != d && str[i + k] && str[i + k] != d)
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
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}


/**
 * splitStringToWords - Split a string into an array of words based on a delimiter character.
 * @str: Pointer to the input string to be split.
 * @d: The delimiter character used to split the string into words.
 * Return: The last element of the array is set to NULL.
 *         If the input string is empty or contains no words, the function returns NULL.
 *         If memory allocation fails, the function returns NULL.
 */
char **splitStringToWords(char *str, char d)
{
	int i, j, k, m, numWords = 0;
	char **s;
	if (str == NULL || str[0] == '\0') return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
		    (str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numWords++;
	if (numWords == 0) return (NULL);
	s = malloc((numWords + 1) * sizeof(char *));
	if (!s) return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (str[i] == d && str[i] != '\0')
			i++;
		k = 0;
		while (str[i + k] != d && str[i + k] && str[i + k] != d)
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

