/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 15:51:19 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	execute_cmd(char **cmd_args, char **envp)
{
	char	*path;

	if (!cmd_args || !cmd_args[0] || !*cmd_args[0])
		exit(EXIT_FAILURE);
	path = find_path(cmd_args[0], envp);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(cmd_args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd_args, envp) == -1)
	{
		free(path);
		error_exit("Execve error");
	}
}

void	child_process1(t_pipex *pipex, char **envp)
{
	if (pipex->infile < 0)
		exit(EXIT_FAILURE);
	close(pipex->pipe_fd[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		error_exit("Dup2 error");
	if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("Dup2 error");
	close(pipex->infile);
	close(pipex->pipe_fd[1]);
	execute_cmd(pipex->cmd1_args, envp);
}

void	child_process2(t_pipex *pipex, char **envp)
{
	close(pipex->pipe_fd[1]);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("Dup2 error");
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		error_exit("Dup2 error");
	close(pipex->pipe_fd[0]);
	close(pipex->outfile);
	execute_cmd(pipex->cmd2_args, envp);
}
