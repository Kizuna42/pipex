/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_long_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 18:30:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/03/04 18:23:32 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	handle_null_pointer_with_precision(t_tab *tab)
{
	int	i;
	int	precision;

	precision = tab->accuracy_width;
	if (precision <= 1)
	{
		write(1, "0x0", 3);
		tab->len += 3;
		return ;
	}
	write(1, "0x", 2);
	tab->len += 2;
	i = 0;
	while (i < precision)
	{
		write(1, "0", 1);
		tab->len++;
		i++;
	}
}

void	handle_null_pointer_left(t_tab *tab, int width)
{
	int	i;

	write(1, "0x0", 3);
	tab->len += 3;
	i = 0;
	while (i < width - 3)
	{
		write(1, " ", 1);
		tab->len++;
		i++;
	}
}

void	handle_null_pointer_zero(t_tab *tab, int width)
{
	int	i;

	write(1, "0x", 2);
	tab->len += 2;
	i = 0;
	while (i < width - 3)
	{
		write(1, "0", 1);
		tab->len++;
		i++;
	}
	write(1, "0", 1);
	tab->len += 1;
}

void	handle_null_pointer_right(t_tab *tab, int width)
{
	int	i;

	i = 0;
	while (i < width - 3)
	{
		write(1, " ", 1);
		tab->len++;
		i++;
	}
	write(1, "0x0", 3);
	tab->len += 3;
}

void	handle_null_pointer(t_tab *tab)
{
	int	width;

	width = tab->field_width;
	if (tab->dot > 0 && tab->accuracy_width > 0)
	{
		handle_null_pointer_with_precision(tab);
		return ;
	}
	if (width <= 3)
	{
		write(1, "0x0", 3);
		tab->len += 3;
		return ;
	}
	if (tab->flag_index == 2)
		handle_null_pointer_left(tab, width);
	else if (tab->flag_index == 1 && tab->dot == 0)
		handle_null_pointer_zero(tab, width);
	else
		handle_null_pointer_right(tab, width);
}
