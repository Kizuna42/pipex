/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int_flag.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:54:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/03/04 18:23:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_flag_hyphen_sign(t_tab *tab, int *d)
{
	if (*d < 0)
	{
		*d *= -1;
		tab->len += 1;
		tab->negative_other = 1;
		write(1, "-", 1);
	}
	else if (*d >= 0 && tab->flag_index == 3)
	{
		tab->len += 1;
		tab->negative_other = 3;
		write(1, "+", 1);
	}
	else if (*d >= 0 && tab->flag_index == 4)
	{
		tab->len += 1;
		tab->negative_other = 2;
		write(1, " ", 1);
	}
}

void	ft_flag_hyphen(t_tab *tab, int d)
{
	int	diff;

	if (tab->field_width < 0)
		tab->field_width *= -1;
	tab->len += ft_putnbr_int(tab, d, 'l');
	ft_flag_hyphen_sign(tab, &d);
	tab->keta_count += ft_putnbr_int(tab, d, 'l');
	diff = ft_set_diff(tab);
	ft_putnbr_int(tab, d, 'w');
	ft_write_minus_space(tab, 0, diff, 'y');
}

static void	ft_flag_nonhyphen_sign(t_tab *tab, int *d, int *count)
{
	if (*d < 0)
	{
		tab->len += 1;
		if (tab->asterisk > 0 && tab->dot == 0 && *d > 0)
		{
			write(1, "-", 1);
			*count += 1;
		}
		else
		{
			*d *= -1;
			tab->negative_other = 1;
		}
	}
	else if (*d >= 0 && tab->flag_index == 3)
	{
		tab->len += 1;
		tab->negative_other = 3;
	}
	else if (*d >= 0 && tab->flag_index == 4)
	{
		tab->len += 1;
		tab->negative_other = 2;
	}
}

void	ft_flag_nonhyphen(t_tab *tab, int d)
{
	int	diff;
	int	count;

	count = 0;
	tab->len += ft_putnbr_int(tab, d, 'l');
	ft_flag_nonhyphen_sign(tab, &d, &count);
	tab->keta_count += ft_putnbr_int(tab, d, 'l');
	diff = ft_set_diff(tab);
	ft_write_minus_space(tab, count, diff, 'n');
	ft_putnbr_int(tab, d, 'w');
}
