/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 15:04:26 by gcollet           #+#    #+#             */
/*   Updated: 2025/04/16 19:44:16 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Main Function */
int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	file_result;
	int	cmd_result;

	if (argc >= 5)
	{
		file_result = setup_files(argc, argv, &i);
		cmd_result = process_commands(argv, envp, i, argc);
		if (file_result)
			return (1);
		return (cmd_result);
	}
	usage();
	return (EXIT_FAILURE);
}
