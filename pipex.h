/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 15:51:56 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft/libft.h"

/* 構造体 */
typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd1_args;
	char	**cmd2_args;
	char	*cmd1_path;
	char	*cmd2_path;
}	t_pipex;

/* 関数プロトタイプ */
void	init_pipex(t_pipex *pipex, char **argv);
void	child_process1(t_pipex *pipex, char **envp);
void	child_process2(t_pipex *pipex, char **envp);
void	parent_process(t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	error_exit(char *message);
char	*find_path(char *cmd, char **envp);
void	close_pipes(t_pipex *pipex);
void	free_array(char **array);

#endif
