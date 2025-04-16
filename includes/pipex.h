/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:38:51 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/16 19:37:16 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"

/* Mandatory functions */
void	error(void);
char	*find_path(char *cmd, char **envp);
int		pipex_get_next_line(char **line);
void	execute(char *argv, char **envp);

/* Bonus functions */
int		open_file(char *argv, int i);
void	usage(void);
void	child_process(char *argv, char **envp);
void	here_doc(char *limiter, int argc);
void	here_doc_child(int *fd, char *limiter);
int		setup_files(int argc, char **argv, int *i);
int		setup_heredoc(int argc, char **argv, int *i);
int		setup_regular(int argc, char **argv, int *i);
int		process_commands(char **argv, char **envp, int i, int argc);

#endif
