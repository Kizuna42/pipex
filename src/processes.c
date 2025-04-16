/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 17:07:32 by kizuna           ###   ########.fr       */
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

static void	setup_io(t_pipex *pipex, int cmd_index)
{
	if (cmd_index == 0)
	{
		if (dup2(pipex->infile, STDIN_FILENO) == -1)
			error_exit("Dup2 error");
		if (dup2(pipex->pipes[0][1], STDOUT_FILENO) == -1)
			error_exit("Dup2 error");
	}
	else if (cmd_index == pipex->cmd_count - 1)
	{
		if (dup2(pipex->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
			error_exit("Dup2 error");
		if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
			error_exit("Dup2 error");
	}
	else
	{
		if (dup2(pipex->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
			error_exit("Dup2 error");
		if (dup2(pipex->pipes[cmd_index][1], STDOUT_FILENO) == -1)
			error_exit("Dup2 error");
	}
}

static void	prepare_command(t_pipex *pipex, char **cmd_splits)
{
	int	i;

	if (pipex->infile < 0 && pipex->cmd_count == 0)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < pipex->pipe_count)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		i++;
	}
	if (!cmd_splits || !cmd_splits[0])
		exit(EXIT_FAILURE);
}

void	exec_cmd(t_pipex *pipex, char **argv __attribute__((unused)),
	char **envp, int cmd_index)
{
	char	**cmd_splits;

	cmd_splits = ft_split(pipex->cmd_args[cmd_index], ' ');
	setup_io(pipex, cmd_index);
	prepare_command(pipex, cmd_splits);
	execute_cmd(cmd_splits, envp);
}
