/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:20:59 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	is_limiter(char *line, char *limiter)
{
	size_t	len;

	len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		return (1);
	return (0);
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
