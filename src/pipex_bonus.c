/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:25:40 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 12:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
		error();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute(argv, envp);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

void	here_doc(char *limiter, int argc)
{
	pid_t	writer;
	int		fd[2];
	char	*buf;
	size_t	len;

	if (argc < 6)
		usage();
	buf = read_heredoc(limiter, &len);
	if (pipe(fd) == -1)
		error();
	writer = fork();
	if (writer == -1)
		error();
	if (writer == 0)
	{
		close(fd[0]);
		write(fd[1], buf, len);
		close(fd[1]);
		free(buf);
		exit(EXIT_SUCCESS);
	}
	free(buf);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

static int	exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

static int	run_last(char *argv, char **envp, int fileout)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = -1;
	if (fileout != -1)
		pid = fork();
	if (fileout != -1 && pid == -1)
		error();
	if (pid == 0)
	{
		dup2(fileout, STDOUT_FILENO);
		close(fileout);
		execute(argv, envp);
	}
	if (fileout != -1)
		close(fileout);
	close(STDIN_FILENO);
	if (pid > 0)
		waitpid(pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (pid < 0)
		return (EXIT_FAILURE);
	return (exit_code(status));
}

int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	fileout;

	if (argc >= 5)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0)
		{
			i = 3;
			fileout = open_file(argv[argc - 1], 0);
			here_doc(argv[2], argc);
		}
		else
		{
			fileout = open_file(argv[argc - 1], 1);
			i = setup_infile(argv[1]);
		}
		while (i < argc - 2)
			child_process(argv[i++], envp);
		return (run_last(argv[argc - 2], envp, fileout));
	}
	usage();
}
