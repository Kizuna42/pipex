/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:01:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	create_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipes = (int **)malloc(sizeof(int *) * pipex->pipe_count);
	if (!pipex->pipes)
		error_exit("Malloc error");
	i = 0;
	while (i < pipex->pipe_count)
	{
		pipex->pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
			error_exit("Malloc error");
		if (pipe(pipex->pipes[i]) < 0)
			error_exit("Pipe error");
		i++;
	}
}

void	close_all_pipes(t_pipex *pipex)
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

void	wait_all_children(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pids[i], NULL, 0);
		i++;
	}
	if (pipex->is_heredoc)
		unlink(".heredoc_tmp");
}
