#include 


#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int picoshell (char **cmds[])
{
    int     i;
    int     prev_fd;
    int     fd[2];
    pid_t   pid;
    int     ret;
    int     status;

    i = 0;
    prev_fd = -1;
    ret = 0;

    while (cmds[i])
    {
        // pipe if theres a second cmd
        if (cmds[i + 1])
        {
            if (pipe(fd) == -1)
                return(1);
        }
        // child current cmd
        pid = fork()
        if (pid == -1)
            return(1);
        if (pid == 0)
        {
            // last cmds input 
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
        else
            prev_fd = -1;
        i++;
        while(wait(&status)> 0)
        {
            if(!WIFEXITED(status) || WEXITSTATUS(status)!= 0)
                ret = 1;
        }
    }
    return(ret);

}