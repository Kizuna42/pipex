/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 17:08:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

extern void	open_files(t_pipex *pipex, char **argv, int argc);

static void	allocate_memory(t_pipex *pipex)
{
	pipex->cmd_args = (char **)malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->cmd_args)
		error_exit("Malloc error");
	pipex->cmd_paths = (char **)malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->cmd_paths)
		error_exit("Malloc error");
	pipex->pids = (pid_t *)malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
		error_exit("Malloc error");
}

void	init_pipex(t_pipex *pipex, int argc, char **argv)
{
	int	i;
	int	cmd_start;

	ft_memset(pipex, 0, sizeof(t_pipex));
	pipex->is_heredoc = is_heredoc(argv[1]);
	if (pipex->is_heredoc)
		cmd_start = 3;
	else
		cmd_start = 2;
	pipex->cmd_count = argc - cmd_start - 1;
	pipex->pipe_count = pipex->cmd_count - 1;
	allocate_memory(pipex);
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->cmd_args[i] = argv[i + cmd_start];
		pipex->cmd_paths[i] = NULL;
		i++;
	}
	pipex->cmd_args[i] = NULL;
	pipex->cmd_paths[i] = NULL;
	open_files(pipex, argv, argc);
}
