#include "../../include/execution.h"

int pwd_print_working_directory(t_tree *root)
{
    char *pathname;
    int fd = STDOUT_FILENO; 

    // if (root && root->out_fd != -1)
    //     fd = root->out_fd;

    pathname = getcwd(NULL, 0);
    if (!pathname)
    {
        perror("pwd: getcwd");
        return 1;
    }

    write(fd, pathname, ft_strlen(pathname));
	write(fd, "\n", 1);
    free(pathname);
    return 0;
}