/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_long_flag.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 18:32:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/03/04 18:23:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putnbr_ulong(t_tab *tab, unsigned long nbr, char mode)
{
	int	len;

	len = 0;
	if (mode != 'l')
	{
		ft_put_zero(tab);
		if (nbr == 0 && (tab->field_width - tab->accuracy_width) > 0
			&& tab->accuracy_width == 0 && tab->dot > 0)
			return (1);
		else
			len += ft_putnbr_write_len(
					nbr, tab->base, mode, "0123456789abcdef");
	}
	else
		len += ft_putnbr_write_len(nbr, tab->base, mode, "0123456789abcdef");
	return (len);
}

void	ft_flag_hyphen_ulong(t_tab *tab, unsigned long p, int diff)
{
	int	count;

	if (tab->fmt == 'p')
	{
		write(1, "0x", 2);
		tab->len += 2;
	}
	tab->len += ft_putnbr_ulong(tab, p, 'w');
	count = 0;
	if (diff > 0)
	{
		while (count < tab->field_width
			- (tab->keta_count + tab->acu_keta_diff + tab->negative_other))
		{
			write(1, " ", 1);
			tab->len++;
			count++;
		}
	}
}

void	ft_flag_nonhyphen_ulong(t_tab *tab, unsigned long p, int diff)
{
	int	count;

	count = 0;
	if (diff > 0)
		ft_diff_one_or_more(tab, count);
	if (tab->fmt == 'p')
	{
		write(1, "0x", 2);
		tab->len += 2;
	}
	tab->len += ft_putnbr_ulong(tab, p, 'w');
}
