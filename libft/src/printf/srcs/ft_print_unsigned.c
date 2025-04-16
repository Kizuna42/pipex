/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:22:32 by kishino           #+#    #+#             */
/*   Updated: 2025/03/04 17:56:07 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static int	ft_putnbr_uint(t_tab *tab, unsigned int nbr, char mode)
{
	int	len;

	len = 0;
	if (mode != 'l')
	{
		ft_put_zero(tab);
		if (nbr == 0 && (tab->field_width - tab->accuracy_width) > 0
			&& tab->accuracy_width == 0 && tab->dot > 0)
		{
			write(1, " ", 1);
			return (1);
		}
		if ('a' < tab->fmt && tab->fmt < 'z')
			len += ft_putnbr_write_len(nbr, tab->base, 'w', "0123456789abcdef");
		else
			len += ft_putnbr_write_len(nbr, tab->base, 'w', "0123456789ABCDEF");
	}
	else
		len += ft_putnbr_write_len(nbr, tab->base, 'l', "");
	return (len);
}

void	ft_diff_over_zero(t_tab *tab);
void	ft_write_prefix(t_tab *tab, unsigned int x);
void	ft_set_negative_other(t_tab *tab, unsigned int x);

static void	ft_handle_flags(t_tab *tab, unsigned int x)
{
	int	diff;

	tab->keta_count += ft_putnbr_uint(tab, x, 'l');
	ft_set_negative_other(tab, x);
	diff = ft_set_diff(tab);
	if (tab->flag_index != 2)
	{
		if (diff > 0)
			ft_diff_over_zero(tab);
		ft_write_prefix(tab, x);
	}
	else
	{
		ft_write_prefix(tab, x);
	}
	tab->len += ft_putnbr_uint(tab, x, 'w');
	if (tab->flag_index == 2 && diff > 0)
		ft_diff_over_zero(tab);
}

void	ft_print_unsigned(t_tab *tab, int base)
{
	unsigned int	x;

	x = (va_arg(tab->args, unsigned int));
	tab->base = base;
	if (tab->dot > 0 && x == 0
		&& tab->accuracy_width == 0 && tab->field_width == 0)
		return ;
	ft_handle_flags(tab, x);
}
