/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 09:54:02 by gcollet           #+#    #+#             */
/*   Updated: 2025/04/16 19:07:42 by kizuna           ###   ########.fr       */
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
		waitpid(pid, NULL, 0);
	}
}

/* Setup input and output file descriptors */
void	setup_io_files(char **argv, int argc, int *filein, int *fileout)
{
	*fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (*fileout == -1)
		error();
	*filein = open(argv[1], O_RDONLY, 0777);
	if (*filein == -1)
		error();
	dup2(*filein, STDIN_FILENO);
	close(*filein);
}

/* Main function that run the childs process with the right file descriptor
 or display an error message if arguments are wrong. */
int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	filein;
	int	fileout;

	if (argc >= 5)
	{
		i = 2;
		setup_io_files(argv, argc, &filein, &fileout);
		while (i < argc - 2)
			child_process(argv[i++], envp);
		dup2(fileout, STDOUT_FILENO);
		close(fileout);
		execute(argv[argc - 2], envp);
	}
	else
	{
		ft_putstr_fd("\033[31mError: Bad arguments\n\e[0m", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
	}
	return (0);
}
