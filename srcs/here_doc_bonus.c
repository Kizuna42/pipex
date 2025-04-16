/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:15:30 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 19:45:49 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Child process for here_doc that reads from stdin until it finds limiter */
void	here_doc_child(int *fd, char *limiter)
{
	char	*line;

	close(fd[0]);
	ft_putstr_fd("heredoc> ", 1);
	while (pipex_get_next_line(&line))
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
		ft_putstr_fd("heredoc> ", 1);
	}
	free(line);
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

/* Function for here_doc processing with child process and pipe */
void	here_doc(char *limiter, int argc)
{
	pid_t	reader;
	int		fd[2];
	int		status;

	if (argc < 6)
		usage();
	if (pipe(fd) == -1)
		error();
	reader = fork();
	if (reader == 0)
		here_doc_child(fd, limiter);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(reader, &status, 0);
	}
}
