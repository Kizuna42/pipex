/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:13:20 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:13:23 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	ft_initialize(t_tab *tab)
{
	tab->len = 0;
	tab->field_width = 0;
	tab->keta_count = 0;
	tab->accuracy_width = 0;
	tab->acu_keta_diff = 0;
	tab->negative_other = 0;
	tab->base = 0;
	tab->flag_index = -1;
	tab->asterisk = 0;
	tab->flag_kind = "#0- +";
	tab->dot = 0;
	tab->error_percent_nonfmt = 0;
	tab->fmt = '\0';
}

void	ft_initialize_again(t_tab *tab)
{
	tab->field_width = 0;
	tab->accuracy_width = 0;
	tab->negative_other = 0;
	tab->flag_index = -1;
	tab->keta_count = 0;
	tab->acu_keta_diff = 0;
	tab->base = 0;
	tab->asterisk = 0;
	tab->dot = 0;
	tab->fmt = '\0';
}
