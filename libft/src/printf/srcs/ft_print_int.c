/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:18:03 by kishino           #+#    #+#             */
/*   Updated: 2025/03/04 18:23:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_write_space_y(t_tab *tab, int diff)
{
	int	i;

	i = -1;
	if (diff > 0)
	{
		while (++i < tab->field_width
			- (tab->keta_count + tab->acu_keta_diff + tab->negative_other))
		{
			write(1, " ", 1);
			tab->len++;
		}
	}
}

void	ft_write_minus_space(t_tab *tab, int count, int diff, char mode)
{
	if (mode == 'y')
	{
		ft_write_space_y(tab, diff);
	}
	else
	{
		if (tab->negative_other == 1 && tab->flag_index == 1 && tab->dot == 0)
			write(1, "-", 1);
		if (diff > 0)
			ft_diff_one_or_more(tab, count);
		if (tab->negative_other == 1 && (tab->flag_index != 1
				|| (tab->flag_index == 1 && tab->dot == 1)))
			write(1, "-", 1);
		else if (tab->negative_other == 3)
			write(1, "+", 1);
		else if (tab->negative_other == 2)
			write(1, " ", 1);
	}
}

int	ft_putnbr_int(t_tab *tab, long int nbr, char mode)
{
	int	len;

	len = 0;
	if (nbr < 0)
		nbr *= -1;
	if (mode == 'w')
	{
		ft_put_zero(tab);
		if (nbr == 0 && (tab->field_width - tab->accuracy_width) > 0
			&& tab->accuracy_width == 0 && tab->dot > 0)
		{
			return (0);
		}
		if (tab->field_width == 0 && nbr == 0
			&& tab->accuracy_width == 0 && tab->dot == 1)
		{
			tab->len--;
			return (0);
		}
		ft_putnbr_write_len(nbr, tab->base, mode, "0123456789abcdef");
	}
	else if (mode == 'l')
		len = ft_putnbr_write_len(nbr, tab->base, mode, "0123456789abcdef");
	return (len);
}

void	ft_flag_hyphen(t_tab *tab, int d);
void	ft_flag_nonhyphen(t_tab *tab, int d);

void	ft_print_int(t_tab *tab, int base)
{
	int	d;

	d = (va_arg(tab->args, int));
	tab->base = base;
	if (tab->flag_index != 2 && tab->field_width >= 0)
		ft_flag_nonhyphen(tab, d);
	else if (tab->flag_index == 2 || tab->field_width < 0)
		ft_flag_hyphen(tab, d);
}
