/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 16:22:38 by kishino           #+#    #+#             */
/*   Updated: 2025/04/16 00:20:50 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_update_flag_index(t_tab *tab, int current_flag)
{
	if (current_flag == 2)
		tab->flag_index = 2;
	else if (current_flag == 3 && tab->flag_index != 2)
		tab->flag_index = 3;
	else if (current_flag == 4 && tab->flag_index != 2
		&& tab->flag_index != 3)
		tab->flag_index = 4;
}

void	ft_process_flag_priority(t_tab *tab, int *i)
{
	int	current_flag;
	int	index;

	while (1)
	{
		index = ft_strnchr(tab->flag_kind, tab->copy[*i], 0, '\0');
		if (index < 0)
			break ;
		current_flag = index;
		ft_update_flag_index(tab, current_flag);
		if (current_flag == 2)
			*i += 1;
		else
			*i += 1;
	}
}

void	ft_process_flag(t_tab *tab, int *i)
{
	tab->flag_index = ft_strnchr(tab->flag_kind, tab->copy[*i], 0, '\0');
	if (tab->flag_index >= 0)
		*i += 1;
	if (tab->flag_index == 2)
		return ;
	ft_process_flag_priority(tab, i);
}
