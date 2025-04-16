/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 15:51:39 by kizuna           ###   ########.fr       */
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
	char	*cmd_name;

	if (!cmd || !*cmd)
		return (NULL);
	cmd_name = ft_split(cmd, ' ')[0];
	env_path = get_env_path(envp);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	cmd_path = get_cmd_path(cmd_name, paths);
	free(cmd_name);
	free_array(paths);
	return (cmd_path);
}

void	init_pipex(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		perror(argv[1]);
		ft_putstr_fd("Warning: Input file error.\n", 2);
		ft_putstr_fd("Command will not receive input.\n", 2);
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
		error_exit(argv[4]);
	pipex->cmd1_args = ft_split(argv[2], ' ');
	pipex->cmd2_args = ft_split(argv[3], ' ');
	pipex->cmd1_path = NULL;
	pipex->cmd2_path = NULL;
}
