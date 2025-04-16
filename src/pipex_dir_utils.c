/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_dir_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 17:19:11 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	check_and_create_dir(char *tmp, char *p, int i)
{
	p[i] = '\0';
	if (access(tmp, F_OK) != 0)
	{
		if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
		{
			perror(tmp);
			ft_putstr_fd("Warning: Could not create directory\n", 2);
		}
	}
	p[i] = '/';
}

static void	create_final_dir(char *tmp)
{
	if (access(tmp, F_OK) != 0)
		mkdir(tmp, 0755);
}

static void	create_output_dir_recursive(char *path)
{
	char	*tmp;
	char	*p;
	int		i;
	int		start;

	tmp = ft_strdup(path);
	if (!tmp)
		return ;
	p = tmp;
	start = 0;
	if (*p == '/')
	{
		p++;
		start = 1;
	}
	i = 0;
	while (p[i])
	{
		if (p[i] == '/')
			check_and_create_dir(tmp, p, i + start);
		i++;
	}
	create_final_dir(tmp);
	free(tmp);
}

void	create_output_dir(char *outfile_path)
{
	char	*last_slash;
	char	*dir_path;

	last_slash = ft_strrchr(outfile_path, '/');
	if (!last_slash)
		return ;
	dir_path = ft_substr(outfile_path, 0, last_slash - outfile_path);
	if (!dir_path)
		return ;
	create_output_dir_recursive(dir_path);
	free(dir_path);
}
