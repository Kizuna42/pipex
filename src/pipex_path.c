/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 17:18:35 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;
	char	*temp;

	i = 0;
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	while (paths && paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*env_path;
	char	*cmd_path;
	char	**cmd_parts;

	if (!cmd || !*cmd)
		return (NULL);
	cmd_parts = ft_split(cmd, ' ');
	if (!cmd_parts)
		return (NULL);
	env_path = get_env_path(envp);
	if (!env_path)
	{
		free_array(cmd_parts);
		return (NULL);
	}
	paths = ft_split(env_path, ':');
	cmd_path = get_cmd_path(cmd_parts[0], paths);
	free_array(cmd_parts);
	free_array(paths);
	return (cmd_path);
}
