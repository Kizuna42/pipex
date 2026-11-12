/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:05:47 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 12:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	usage(void)
{
	ft_putstr_fd("\033[31mError: Bad argument\n\e[0m", 2);
	ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <...> <file2>\n", 1);
	ft_putstr_fd("    ./pipex \"here_doc\" <LIMITER> <cmd> <cmd1> <...> ", 1);
	ft_putstr_fd("<file>\n", 1);
	exit(EXIT_FAILURE);
}

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		file = open(argv, O_RDONLY);
	if (file == -1)
		error();
	return (file);
}

static char	*hd_append(char *buf, size_t *len, size_t *cap, char *line)
{
	size_t	n;
	char	*tmp;

	n = ft_strlen(line);
	if (*len + n + 1 > *cap)
	{
		while (*len + n + 1 > *cap)
			*cap *= 2;
		tmp = malloc(*cap);
		if (tmp)
			ft_memcpy(tmp, buf, *len);
		free(buf);
		buf = tmp;
	}
	if (buf)
	{
		ft_memcpy(buf + *len, line, n);
		*len += n;
		buf[*len] = '\0';
	}
	free(line);
	return (buf);
}

char	*read_heredoc(char *limiter, size_t *len)
{
	char	*line;
	char	*buf;
	size_t	cap;
	size_t	lim;

	lim = ft_strlen(limiter);
	cap = 4096;
	*len = 0;
	buf = malloc(cap);
	if (!buf)
		error();
	buf[0] = '\0';
	line = get_next_line(0);
	while (buf && line && !(ft_strncmp(line, limiter, lim) == 0
			&& (line[lim] == '\n' || line[lim] == '\0')))
	{
		buf = hd_append(buf, len, &cap, line);
		line = get_next_line(0);
	}
	free(line);
	if (!buf)
		error();
	return (buf);
}
