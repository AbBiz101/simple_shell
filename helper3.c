#include "allHeader.h"

/**
 * pathFind - Find the full path of an executable command within the specified path.
 * @info: Pointer to the structure containing context information (e.g., environment variables).
 * @pathstr: String containing a list of directories separated by colons (':').
 * @cmd: The command to be found in the directories.
 * Return: Pointer to the full path of the executable command if found, or NULL if not found.
 */
char *pathFind(infoTable *info, char *pathstr, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;
    if (!pathstr) return NULL;
    if (strLength(cmd) > 2 && needleStartWithHaystack(cmd, "./"))
    {
        if (isExecutable(info)) return cmd;
    }
    while (1)
    {
        if (!pathstr[i] || pathstr[i] == ':')
        {
            path = duplicateChar(pathstr, curr_pos, i);
            if (!*path)
                concatTowStr(path, cmd);
            else
            {
                concatTowStr(path, "/");
                concatTowStr(path, cmd);
            }
            if (isExecutable(info)) return path; 
            if (!pathstr[i]) break;
            curr_pos = i;
        }
        i++;
    }
    return NULL;
}

/**
 * shellLoop - Main loop of the shell program.
 * @info: Pointer to the structure containing context information (e.g., environment variables).
 * @av:   Array of strings containing command-line arguments (usually from 'main' function).
 * Return: -2: Exit from an internal function or an error condition.
 *         -1: Exit from an internal 'exit' command with a specific error number.
 *          0: Exit from an internal 'exit' command without an error number.
 *          1: Exit from an internal 'cd' command.
 */
int shellLoop(infoTable *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;
    while (r != -1 && builtin_ret != -2)
    {
        infoStructClear(info);
        if (isInteractive(info)) printStr("$ ");
        charCToStderr(FLASH_BUFFER);
        r = lineMinusNewline(info);
        if (r != -1)
        {
            infoStructInitialize(info, av);
            builtin_ret = findBuiltinCMD(info);
            if (builtin_ret == -1) findCMDPath(info);
        }
        else if (isInteractive(info))
            charCToStdout('\n');
        freeInfoStructFields(info, 0);
    }
    historyWriter(info);
    freeInfoStructFields(info, 1);
    if (!isInteractive(info) && info->status) exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->errNum == -1)
            exit(info->status);
        exit(info->errNum);
    }
    return builtin_ret;
}

/**
 * strCompar - Compare two null-terminated strings lexicographically.
 * @s1: Pointer to the first null-terminated string for comparison.
 * @s2: Pointer to the second null-terminated string for comparison.
 * Return: 0: If both strings are equal up to their null terminators.
 *        -1: If 's1' is lexicographically less than 's2'.
 *         1: If 's1' is lexicographically greater than 's2'.
 */
int strCompar(char *s1, char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return (*s1 - *s2);
        s1++;
        s2++;
    }
    if (*s1 == *s2)
        return 0;
    else
        return (*s1 < *s2 ? -1 : 1); 
}

/**
 * strToInt - Convert a null-terminated string to an integer value.
 * @s: Pointer to the null-terminated string to be converted to an integer.
 * Return: The integer value of the string if it represents a valid integer within
 *         the range of 'int' data type. If the string is not a valid integer or
 *         exceeds the range of 'int' data type, it returns -1.
 */
int strToInt(char *s)
{
    int i = 0;
    unsigned long int result = 0;
    if (*s == '+') s++;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX) return -1; 
        }
        else return -1;
    }
    return result;
}

/**
 * shellExit - Handle the "exit" command in the shell program.
 * @info: Pointer to the structure containing context information.
 * Return: -2 to indicate an exit condition from an internal function.
 */
int shellExit(infoTable *info)
{
    int checkExit;
    if (info->argv[1])
    {
        checkExit = strToInt(info->argv[1]);
        if (checkExit == -1)
        {
            info->status = 2;
            errorMessage(info, "Illegal number: ");
            printInputStr_0(info->argv[1]);
            charCToStderr('\n');
            return 1;
        }
        info->errNum = checkExit;
        return -2;
    }
    info->errNum = -1;
    return -2;
}

