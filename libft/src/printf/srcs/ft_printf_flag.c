/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 16:22:38 by kishino           #+#    #+#             */
/*   Updated: 2025/04/16 00:25:12 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_handle_field_width(t_tab *tab, int *i)
{
	if (tab->copy[*i] == '*')
	{
		*i += 1;
		tab->asterisk += 1;
	}
	if ('0' <= tab->copy[*i] && tab->copy[*i] <= '9')
	{
		tab->field_width = ft_atoi(&tab->copy[*i]);
		while ('0' <= tab->copy[*i] && tab->copy[*i] <= '9')
			*i += 1;
	}
}

static void	ft_handle_precision(t_tab *tab, int *i)
{
	if (tab->copy[*i] == '.')
	{
		*i += 1;
		tab->dot += 1;
		if ('0' <= tab->copy[*i] && tab->copy[*i] <= '9')
		{
			tab->accuracy_width = ft_atoi(&tab->copy[*i]);
			while ('0' <= tab->copy[*i] && tab->copy[*i] <= '9')
				*i += 1;
		}
	}
	if (tab->copy[*i] == '*')
	{
		*i += 1;
		tab->asterisk += 1;
	}
}

void	ft_printf_flag(t_tab *tab, int *i)
{
	while (tab->copy[*i] == '-' || tab->copy[*i] == '0')
	{
		if (tab->copy[*i] == '-')
			tab->flag_index = 2;
		*i += 1;
	}
	if (ft_strnchr(tab->flag_kind, tab->copy[*i], 0, '\0') >= 0)
		ft_process_flag(tab, i);
	ft_handle_field_width(tab, i);
	ft_handle_precision(tab, i);
}
