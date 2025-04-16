/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:10:44 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	execute_commands(t_pipex *pipex, char **argv, char **envp)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] < 0)
			error_exit("Fork error");
		if (pipex->pids[i] == 0)
			exec_cmd(pipex, argv, envp, i);
		i++;
	}
}

static int	check_args(int argc, char **argv)
{
	int	is_here_doc;
	int	min_args;

	is_here_doc = is_heredoc(argv[1]);
	min_args = 5;
	if (is_here_doc)
		min_args = 6;
	if (argc < min_args)
	{
		if (is_here_doc)
			ft_putstr_fd("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file\n", 2);
		else
			ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", 2);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (!check_args(argc, argv))
		return (1);
	init_pipex(&pipex, argc, argv);
	if (pipex.is_heredoc)
		handle_heredoc(&pipex);
	create_pipes(&pipex);
	execute_commands(&pipex, argv, envp);
	close_all_pipes(&pipex);
	wait_all_children(&pipex);
	free_pipex(&pipex);
	return (0);
}
