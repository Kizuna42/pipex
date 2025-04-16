/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:02:53 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	error_exit(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		i++;
	}
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	free_cmds_and_pipes(t_pipex *pipex)
{
	int	i;

	if (pipex->cmd_args)
		free(pipex->cmd_args);
	if (pipex->cmd_paths)
	{
		i = 0;
		while (i < pipex->cmd_count && pipex->cmd_paths[i])
		{
			free(pipex->cmd_paths[i]);
			i++;
		}
		free(pipex->cmd_paths);
	}
	if (pipex->pipes)
	{
		i = 0;
		while (i < pipex->pipe_count)
		{
			free(pipex->pipes[i]);
			i++;
		}
		free(pipex->pipes);
	}
}

void	free_pipex(t_pipex *pipex)
{
	free_cmds_and_pipes(pipex);
	if (pipex->pids)
		free(pipex->pids);
	if (pipex->infile > 0)
		close(pipex->infile);
	if (pipex->outfile > 0)
		close(pipex->outfile);
}
