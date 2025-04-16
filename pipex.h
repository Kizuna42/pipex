/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:25:03 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 16:07:10 by kizuna           ###   ########.fr       */
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

# define HEREDOC "here_doc"

/* 構造体 */
typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		**pipes;
	int		cmd_count;
	int		pipe_count;
	int		is_heredoc;
	char	**cmd_args;
	char	**cmd_paths;
	pid_t	*pids;
	char	*heredoc_limiter;
}	t_pipex;

/* 関数プロトタイプ */
void	init_pipex(t_pipex *pipex, int argc, char **argv);
void	exec_cmd(t_pipex *pipex, char **argv, char **envp, int cmd_index);
void	parent_process(t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	error_exit(char *message);
char	*find_path(char *cmd, char **envp);
void	close_pipes(t_pipex *pipex);
void	free_array(char **array);

/* ボーナス関数 */
int		is_heredoc(char *arg);
void	handle_heredoc(t_pipex *pipex);
void	create_pipes(t_pipex *pipex);
void	close_all_pipes(t_pipex *pipex);
void	wait_all_children(t_pipex *pipex);
void	open_files(t_pipex *pipex, char **argv, int argc);
int		is_limiter(char *line, char *limiter);

#endif
