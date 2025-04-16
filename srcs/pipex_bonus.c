/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 15:04:26 by gcollet           #+#    #+#             */
/*   Updated: 2025/04/16 19:29:36 by kizuna           ###   ########.fr       */
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

/* Function to setup the files (output & input) and heredoc if necessary */
int	setup_files(int argc, char **argv, int *i)
{
	int	had_error;
	int	output_file;

	had_error = 0;
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
	{
		*i = 3;
		output_file = open_file(argv[argc - 1], 0);
		if (output_file == -1)
			had_error = 1;
		here_doc(argv[2], argc);
	}
	else
	{
		*i = 2;
		output_file = open_file(argv[argc - 1], 1);
		if (output_file == -1)
			had_error = 1;
		dup2(open_file(argv[1], 2), STDIN_FILENO);
	}
	dup2(output_file, STDOUT_FILENO);
	return (had_error);
}

/* Main Function */
int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	file_result;
	int	status;
	int	last_pid;

	if (argc >= 5)
	{
		file_result = setup_files(argc, argv, &i);
		while (i < argc - 2)
			child_process(argv[i++], envp);
		last_pid = fork();
		if (last_pid == 0)
			execute(argv[i], envp);
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (file_result)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	usage();
	return (EXIT_FAILURE);
}
