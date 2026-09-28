#include 



int picoshell (char **cmds[])
{
    int     i;
    int     prev_fd;
    int     fd[2];
    pid_t   pid;

    i = 0;
    prev_fd = -1; 

    while (cmds[i])
    {
        if (cmds[i + 1])
        {
            if (pipe(fd) == -1)
                return(1);
        }
        pid = fork()
        if (pid == -1)
            return(1);
        if (pid == 0)
        {
            if(prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);
            if (cmds[i + 1])
                dup2(fd[1], STDOUT_FILENO);
            if (prev_fd != -1)
                close(prev_fd);
            if (cmds[i + 1])
            {
                close(fd[0]);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        if(prev_fd != -1)
            close(prev_fd);
        if(cmds[i + 1])
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        i++;
    }
    return(1);

}