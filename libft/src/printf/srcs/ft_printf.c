/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 16:22:55 by kishino           #+#    #+#             */
/*   Updated: 2025/04/16 00:17:36 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_put_zero(t_tab *tab)
{
	int	i;

	i = 0;
	while (i < tab->accuracy_width - tab->keta_count)
	{
		write(1, "0", 1);
		tab->len++;
		i++;
	}
}

static void	ft_printf_choice_format(t_tab *tab, int i)
{
	if (tab->copy[i] == '%')
		ft_print_percent(tab);
	else if (tab->copy[i] == 's')
		ft_print_s(tab);
	else if (tab->copy[i] == 'c')
		ft_print_c(tab);
	else if (tab->copy[i] == 'd' || tab->copy[i] == 'i')
		ft_print_int(tab, 10);
	else if (tab->copy[i] == 'x' || tab->copy[i] == 'X')
		ft_print_unsigned(tab, 16);
	else if (tab->copy[i] == 'u')
		ft_print_unsigned(tab, 10);
	else if (tab->copy[i] == 'o')
		ft_print_unsigned(tab, 8);
	else if (tab->copy[i] == 'p')
		ft_print_unsigned_long(tab, 16);
	else if (tab->field_width > 0)
		tab->error_percent_nonfmt = 1;
}

void	ft_printf_putstr(t_tab *tab)
{
	int	i;

	i = 0;
	while (tab->copy[i] != '\0')
	{
		if (tab->copy[i] == '%')
		{
			i += 1;
			ft_printf_flag(tab, &i);
			ft_set_field_accu_fmt(tab, i);
			ft_printf_choice_format(tab, i);
			if (tab->error_percent_nonfmt == 1)
				return ;
		}
		else
		{
			write(1, &tab->copy[i], 1);
			tab->len++;
		}
		ft_initialize_again(tab);
		i++;
	}
}

void	ft_printf_main(t_tab *tab, const char *fmt)
{
	tab->copy = fmt;
	if (ft_strnchr(tab->copy, '%', 0, '\0') == -1)
	{
		ft_putstr(tab->copy);
		tab->len = ft_strlen(tab->copy);
	}
	else
		ft_printf_putstr(tab);
}

int	ft_printf(const char *fmt, ...)
{
	t_tab	*tab;
	int		len;

	if (fmt == NULL)
		return (-1);
	tab = (t_tab *)malloc(sizeof(t_tab));
	if (!tab)
		return (-1);
	ft_initialize(tab);
	if (fmt && *fmt != *"")
	{
		va_start(tab->args, fmt);
		ft_printf_main(tab, fmt);
		va_end(tab->args);
	}
	len = tab->len;
	free(tab);
	return (len);
}
