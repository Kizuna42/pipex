/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:19:24 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	open_files(t_pipex *pipex, char **argv, int argc)
{
	if (pipex->is_heredoc)
	{
		pipex->infile = open(".heredoc_tmp",
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (pipex->infile < 0)
			error_exit(".heredoc_tmp");
		pipex->heredoc_limiter = argv[2];
		pipex->outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
		{
			perror(argv[1]);
			ft_putstr_fd("Warning: Input file error.\n", 2);
			ft_putstr_fd("Command will not receive input.\n", 2);
		}
		pipex->outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (pipex->outfile < 0)
		error_exit(argv[argc - 1]);
}
