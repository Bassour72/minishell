
#include "../../include/execution.h"

#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>

void	close_pipe_fds(int pipefd[2])
{
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
}


int is_fd_open(int fd)
{
    struct winsize ws;
    if (ioctl(fd, TIOCGWINSZ, &ws) == -1)
    {
        if (errno == EBADF)
            return 0; // FD is not open
    }
    return 1; // FD is open (or at least valid for ioctl)
}

 void	close_all_fds_(void)
{
	int fd;

	fd = 0;
	while (fd <= 40)
    {
		close(fd);
		 ++fd;
	}
}

 int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(-1);
		return (-1);
	}
	return (0);
}