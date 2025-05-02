/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 19:49:21 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/02 19:52:18 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*try_path_with_env(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;

	i = -1;
	allpath = ft_split(my_getenv("PATH", env), ':');
	if (!allpath)
		return (ft_strdup(cmd));
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(allpath);
			return (exec);
		}
		free(exec);
	}
	ft_free_tab(allpath);
	return (ft_strdup(cmd));
}

char	*get_path(char *cmd, char **env)
{
	char	**s_cmd;
	char	*exec;

	s_cmd = ft_split(cmd, ' ');
	if (access(s_cmd[0], F_OK | X_OK) == 0)
	{
		exec = ft_strdup(s_cmd[0]);
		ft_free_tab(s_cmd);
		return (exec);
	}
	exec = try_path_with_env(s_cmd[0], env);
	ft_free_tab(s_cmd);
	return (exec);
}
