/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_flag.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:55:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/03/04 18:00:08 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_diff_over_zero(t_tab *tab)
{
	int	count;

	count = 0;
	while (count < tab->field_width
		- (tab->keta_count + tab->acu_keta_diff + tab->negative_other))
	{
		if (tab->flag_index == 1 && tab->dot == 0)
		{
			write(1, "0", 1);
			tab->len++;
			count++;
		}
		else
		{
			write(1, " ", 1);
			tab->len++;
			count++;
		}
	}
}

void	ft_write_prefix(t_tab *tab, unsigned int x)
{
	if (tab->flag_index == 0 && ((tab->base == 16) || (tab->base == 8)))
	{
		if (x == 0 && tab->base == 8 && tab->dot == 1
			&& tab->accuracy_width == 0)
			return ;
		if (x == 0 && tab->base == 16)
			return ;
		write(1, "0", 1);
		tab->len += 1;
		if (tab->base == 16 && x != 0)
		{
			if (tab->fmt == 'x')
				write(1, "x", 1);
			else if (tab->fmt == 'X')
				write(1, "X", 1);
			tab->len += 1;
		}
	}
}

void	ft_set_negative_other(t_tab *tab, unsigned int x)
{
	if (tab->flag_index == 0 && ((tab->base == 16) || (tab->base == 8)))
	{
		if (x == 0 && tab->base == 8 && tab->dot == 1
			&& tab->accuracy_width == 0)
			tab->negative_other = 0;
		else if (x == 0 && tab->base == 16)
			tab->negative_other = 0;
		else
		{
			if (tab->base == 16)
				tab->negative_other += 2;
			else
				tab->negative_other += 1;
		}
	}
}
