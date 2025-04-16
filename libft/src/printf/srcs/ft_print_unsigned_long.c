/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_long.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:22:23 by kishino           #+#    #+#             */
/*   Updated: 2025/03/04 18:23:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_print_unsigned_long(t_tab *tab, int base)
{
	unsigned long	p;
	int				diff;

	p = (va_arg(tab->args, unsigned long));
	tab->base = base;
	tab->fmt = 'p';
	if (p == 0)
	{
		handle_null_pointer(tab);
		return ;
	}
	tab->negative_other = 2;
	tab->keta_count += ft_putnbr_ulong(tab, p, 'l');
	diff = ft_set_diff(tab);
	if ((tab->dot > 0 && (p == 0 || !p) && tab->accuracy_width == 0
			&& (tab->field_width >= 0 && tab->field_width <= 2)))
	{
		write(1, "0x", 2);
		tab->len += 2;
		return ;
	}
	if (tab->flag_index != 2)
		ft_flag_nonhyphen_ulong(tab, p, diff);
	else if (tab->flag_index == 2)
		ft_flag_hyphen_ulong(tab, p, diff);
}
