#include "allHeader.h"

/**
 * inputBuffers - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 * Return: bytes read
 */
ssize_t inputBuffers(infoTable *info, char **buf, size_t *len)
{
    ssize_t r = 0;
    size_t len_p = 0;
    if (!*len)
    {
        free(*buf);
        *buf = NULL;
        signal(SIGINT, blockCtrlC);
#if USE_GETLINE_STRTOK
        r = getline(buf, &len_p, stdin);
#else
        r = nextInputLine(info, buf, &len_p);
#endif
        if (r > 0)
        {
            if ((*buf)[r - 1] == '\n')
            {
                (*buf)[r - 1] = '\0';
                r--;
            }
            info->lineCountFlag = 1;
            commentToNull(*buf);
            historyListBuilder(info, *buf, info->histCount++);
            *len = r;
            info->cmdBuffer = buf;
        }
    }
    return r;
}

/**
 * lineMinusNewline - gets a line minus the newline
 * @info: parameter struct
 * Return: bytes read
 */
ssize_t lineMinusNewline(infoTable *info)
{
    static char *buf;
    static size_t i = 0, j = 0, len = 0;
    ssize_t r = 0;
    char **buf_p = &(info->arg), *p;
    charCToStdout(FLASH_BUFFER);
    r = inputBuffers(info, &buf, &len);
    if (r == -1)
        return (-1);
    if (len)
    {
        j = i;
        p = buf + i;
        chainCheck(info, buf, &j, i, len);
        while (j < len)
        {
            if (isChain(info, buf, &j))
                break;
            j++;
        }
        i = j + 1;
        if (i >= len)
        {
            i = len = 0;
            info->cmdBufferType = NORM_CMD;
        }
        *buf_p = p;
        return (strLength(p));
    }
    *buf_p = buf;
    return (r); 
}

/**
 * readBuffers - reads data into the buffer
 * @info: parameter struct
 * @buf: buffer to store the read data
 * @i: pointer to the size of the buffer
 * Return: Number of bytes read (or error code if negative)
 */
ssize_t readBuffers(infoTable *info, char *buf, size_t *i)
{
    ssize_t r = 0;
    if (*i) return 0;
    r = read(info->readfd, buf, READ_READ_WRITE_BUF_SIZE);
    if (r >= 0) *i = (size_t)r;
    return r; 
}

/**
 * nextInputLine - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: address of the variable to store the length of the buffer
 * Return: The length of the input line read (or -1 on failure)
 */
int nextInputLine(infoTable *info, char **ptr, size_t *length)
{
    static char buf[READ_READ_WRITE_BUF_SIZE];
    static size_t i = 0, len = 0;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;
    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;
    r = readBuffers(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return -1;
    c = charInStr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = reallocMemory(p, s, s ? s + k : k + 1);
    if (!new_p) 
    {
        if (p)
            free(p);
        return -1;
    }
    if (s)
        concatTwoStr(new_p, buf + i, k - i);
    else
        strCopy(new_p, buf + i, k - i + 1);
    s += k - i;
    i = k;
    p = new_p;
    if (length)
        *length = s;
    *ptr = p;
    return s;

/**
 * blockCtrlC - Signal handler to block Ctrl-C (SIGINT)
 * @sig_num: the signal number (unused)
 * Return: void
 */
void blockCtrlC(__attribute__((unused)) int sig_num)
{
    printStr("\n$ ");
    charCToStdout(FLASH_BUFFER);
}

