/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 17:12:46 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

extern void	create_output_dir(char *outfile_path);

static void	open_outfile(t_pipex *pipex, char *outfile_path, int is_append)
{
	int	flags;

	if (is_append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	create_output_dir(outfile_path);
	pipex->outfile = open(outfile_path, flags, 0644);
	if (pipex->outfile < 0)
	{
		perror(outfile_path);
		if (errno == ENOENT)
			ft_putstr_fd("Error: Output directory does not exist\n", 2);
		if (pipex->is_heredoc)
			unlink(".heredoc_tmp");
		exit(EXIT_FAILURE);
	}
}

static void	open_heredoc_files(t_pipex *pipex, char **argv, int argc)
{
	char	*outfile_path;

	outfile_path = argv[argc - 1];
	pipex->infile = open(".heredoc_tmp",
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->infile < 0)
	{
		perror(".heredoc_tmp");
		exit(EXIT_FAILURE);
	}
	pipex->heredoc_limiter = argv[2];
	open_outfile(pipex, outfile_path, 1);
}

static void	open_regular_files(t_pipex *pipex, char **argv, int argc)
{
	char	*outfile_path;

	outfile_path = argv[argc - 1];
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		perror(argv[1]);
		ft_putstr_fd("Warning: Input file error.\n", 2);
		ft_putstr_fd("Command will not receive input.\n", 2);
	}
	open_outfile(pipex, outfile_path, 0);
}

void	open_files(t_pipex *pipex, char **argv, int argc)
{
	if (pipex->is_heredoc)
		open_heredoc_files(pipex, argv, argc);
	else
		open_regular_files(pipex, argv, argc);
}
