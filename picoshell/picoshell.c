#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int	picoshell(char **cmds[])
{
	int	    i;
	int	    in_fd;
	int	    fd[2];
	pid_t	pid;
	int	    status;
	int	    ret;

	i = 0;
	in_fd = STDIN_FILENO;
	ret = 0;

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fd) == -1)
			return (1);

		pid = fork();
		if (pid == -1)
			return (1);

		if (pid == 0)
		{
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}

			if (cmds[i + 1])
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}

			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}

		if (in_fd != STDIN_FILENO)
			close(in_fd);

		if (cmds[i + 1])
		{
			close(fd[1]);
			in_fd = fd[0];
		}

		i++;
	}

	while (wait(&status) > 0)
	{
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			ret = 1;
	}

	return (ret);
}
