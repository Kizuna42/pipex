/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:20:30 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 19:36:42 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Setup for heredoc mode */
int	setup_heredoc(int argc, char **argv, int *i)
{
	int	output_file;
	int	had_error;

	had_error = 0;
	*i = 3;
	output_file = open_file(argv[argc - 1], 0);
	if (output_file == -1)
		had_error = 1;
	here_doc(argv[2], argc);
	dup2(output_file, STDOUT_FILENO);
	return (had_error);
}

/* Setup for regular mode */
int	setup_regular(int argc, char **argv, int *i)
{
	int	output_file;
	int	input_file;
	int	had_error;

	had_error = 0;
	*i = 2;
	output_file = open_file(argv[argc - 1], 1);
	if (output_file == -1)
		had_error = 1;
	input_file = open_file(argv[1], 2);
	if (input_file == -1)
		had_error = 1;
	dup2(input_file, STDIN_FILENO);
	dup2(output_file, STDOUT_FILENO);
	return (had_error);
}

/* Function to setup the files (output & input) and heredoc if necessary */
int	setup_files(int argc, char **argv, int *i)
{
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
		return (setup_heredoc(argc, argv, i));
	else
		return (setup_regular(argc, argv, i));
}
