/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:10:26 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 19:35:52 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Child process that create a fork and a pipe, put the output inside a pipe
 and then close with the exec function. The main process will change his stdin
 for the pipe file descriptor. */
void	child_process(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];
	int		status;

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
		error();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute(argv, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, &status, 0);
	}
}

/* Process the commands in the pipeline */
int	process_commands(char **argv, char **envp, int i, int argc)
{
	int	status;
	int	last_pid;

	while (i < argc - 2)
		child_process(argv[i++], envp);
	last_pid = fork();
	if (last_pid == 0)
		execute(argv[i], envp);
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_SUCCESS);
}
