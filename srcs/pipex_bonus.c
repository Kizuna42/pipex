/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 10:33:51 by gcollet           #+#    #+#             */
/*   Updated: 2025/04/16 19:16:21 by kizuna           ###   ########.fr       */
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

/* Child process for here_doc that reads from stdin until it finds limiter */
void	here_doc_child(int *fd, char *limiter)
{
	char	*line;

	close(fd[0]);
	ft_putstr_fd("heredoc> ", 1);
	while (pipex_get_next_line(&line))
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			exit(EXIT_SUCCESS);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
		ft_putstr_fd("heredoc> ", 1);
	}
	free(line);
	exit(EXIT_SUCCESS);
}

/* Function for here_doc processing with child process and pipe */
void	here_doc(char *limiter, int argc)
{
	pid_t	reader;
	int		fd[2];
	int		status;

	if (argc < 6)
		usage();
	if (pipe(fd) == -1)
		error();
	reader = fork();
	if (reader == 0)
		here_doc_child(fd, limiter);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(reader, &status, 0);
	}
}

/* Process input arguments and setup file descriptors */
void	setup_files(int argc, char **argv, int *fileout, int *i)
{
	int	filein;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		*i = 3;
		*fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (*fileout == -1)
		{
			perror("\033[31mError");
			*fileout = open("/dev/null", O_WRONLY);
		}
		here_doc(argv[2], argc);
	}
	else
	{
		*i = 2;
		*fileout = open_file(argv[argc - 1], 1);
		filein = open_file(argv[1], 2);
		dup2(filein, STDIN_FILENO);
		close(filein);
	}
}

/* Main function to process commands with pipes */
int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	fileout;

	if (argc >= 5)
	{
		setup_files(argc, argv, &fileout, &i);
		while (i < argc - 2)
			child_process(argv[i++], envp);
		dup2(fileout, STDOUT_FILENO);
		close(fileout);
		execute(argv[argc - 2], envp);
	}
	else
		usage();
	return (0);
}
