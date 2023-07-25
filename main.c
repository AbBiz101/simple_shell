#include "allHeader.h"

int openInputFile(const char *filename)
{
int fd = open(filename, O_RDONLY);
if (fd == -1)
{
if (errno == EACCES)
{
perror("Error: Access denied");
exit(ERROR_FILE_OPEN);
}
else if (errno == ENOENT)
{
fprintf(stderr, "Error: File '%s' not found\n", filename);
exit(ERROR_FILE_NOT_FOUND);
}
else
{
perror("Error while opening file");
exit(EXIT_FAILURE);
}
}
return (fd);
}
int main(int argc, char **argv)
{
infoTable info;
int fd = STDIN_FILENO;
if (argc == 2)
{
fd = openInputFile(argv[1]);
}
envListPopulate(&info);
historyReader(&info);
shellLoop(&info, argv);
close(fd);
freeListNodes(&(info.env));
return (EXIT_SUCCESS);
}
