/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s_null.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:18:22 by kishino           #+#    #+#             */
/*   Updated: 2025/03/04 18:04:05 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_flag_nonhyphen_null(t_tab *tab, char *null, long int diff)
{
	int	i;

	i = 0;
	while (i < diff)
	{
		if (tab->flag_index == 1)
			write(1, "0", 1);
		else
			write(1, " ", 1);
		tab->len++;
		i++;
	}
	if (tab->dot == 0 || tab->accuracy_width < 0)
		tab->len += ft_putstr(null);
	else
		tab->len += ft_putnstr(null, tab->accuracy_width);
}

static void	ft_flag_hyphen_null(t_tab *tab, char *null, long int diff)
{
	int	i;

	i = 0;
	if (tab->dot == 0)
		tab->len += ft_putstr(null);
	else
		tab->len += ft_putnstr(null, tab->accuracy_width);
	while (i < diff)
	{
		write(1, " ", 1);
		tab->len++;
		i++;
	}
}

void	ft_print_s_null(t_tab *tab)
{
	long int	diff;
	char		*null;

	null = "(null)";
	tab->keta_count = ft_strlen(null);
	diff = tab->field_width - tab->keta_count;
	if (diff < tab->field_width - tab->accuracy_width
		&& tab->dot != 0 && tab->accuracy_width >= 0)
		diff = tab->field_width - tab->accuracy_width;
	if (tab->flag_index != 2)
		ft_flag_nonhyphen_null(tab, null, diff);
	else if (tab->flag_index == 2)
		ft_flag_hyphen_null(tab, null, diff);
}
