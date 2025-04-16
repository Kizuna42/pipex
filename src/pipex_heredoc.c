/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:03:15 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	is_heredoc(char *arg)
{
	return (arg && ft_strncmp(arg, HEREDOC, ft_strlen(HEREDOC) + 1) == 0);
}

int	is_limiter(char *line, char *limiter)
{
	size_t	len;

	len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		return (1);
	return (0);
}

void	handle_heredoc(t_pipex *pipex)
{
	char	*line;
	int		fd;

	fd = pipex->infile;
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (is_limiter(line, pipex->heredoc_limiter))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
	close(fd);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
		error_exit(".heredoc_tmp");
}
