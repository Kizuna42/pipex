/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:47:15 by kishino           #+#    #+#             */
/*   Updated: 2025/04/16 00:25:38 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

size_t			ft_strlen(const char *s);
int				ft_strnchr(const char *s, int c, int start, int end);
char			*ft_strchr(const char *s, int c);
int				ft_atoi(const char *nptr);
int				ft_putstr(const char *s);
int				ft_putnstr(const char *str, long int end);

typedef struct s_tab
{
	va_list		args;
	int			len;
	const char	*copy;
	const char	*flag_kind;
	char		fmt;
	long int	field_width;
	long int	keta_count;
	long int	accuracy_width;
	long int	acu_keta_diff;
	long int	base;
	int			negative_other;
	int			flag_index;
	int			asterisk;
	int			dot;
	int			error_percent_nonfmt;
}				t_tab;

int				ft_printf(const char *fmt, ...);
int				ft_set_diff(t_tab *tab);
int				ft_diff_one_or_more(t_tab *tab, int count);
int				ft_putnbr_write_len(unsigned long nb, int base,
					char mode, char *putlist);
int				ft_putnbr_ulong(t_tab *tab, unsigned long nbr, char mode);
int				ft_putnbr_int(t_tab *tab, long int nbr, char mode);
void			ft_print_c(t_tab *tab);
void			ft_print_percent(t_tab *tab);
void			ft_print_s(t_tab *tab);
void			ft_print_s_null(t_tab *tab);
void			ft_initialize(t_tab *tab);
void			ft_initialize_again(t_tab *tab);
void			ft_print_unsigned(t_tab *tab, int base);
void			ft_print_unsigned_long(t_tab *tab, int base);
void			ft_print_int(t_tab *tab, int base);
void			ft_set_field_accu_fmt(t_tab *tab, int i);
void			ft_flag_hyphen_unsigned(t_tab *tab, int i, unsigned int x);
void			ft_printf_flag(t_tab *tab, int *i);
void			ft_put_zero(t_tab *tab);
void			ft_flag_hyphen_ulong(t_tab *tab, unsigned long p, int diff);
void			ft_flag_nonhyphen_ulong(t_tab *tab, unsigned long p, int diff);
void			handle_null_pointer(t_tab *tab);
void			handle_null_pointer_with_precision(t_tab *tab);
void			handle_null_pointer_left(t_tab *tab, int width);
void			handle_null_pointer_zero(t_tab *tab, int width);
void			handle_null_pointer_right(t_tab *tab, int width);
void			ft_write_minus_space(t_tab *tab, int count, int diff,
					char mode);
void			ft_flag_hyphen(t_tab *tab, int d);
void			ft_flag_nonhyphen(t_tab *tab, int d);
void			ft_diff_over_zero(t_tab *tab);
void			ft_write_prefix(t_tab *tab, unsigned int x);
void			ft_set_negative_other(t_tab *tab, unsigned int x);

/* ft_printf_flag_utils.c */
void			ft_update_flag_index(t_tab *tab, int current_flag);
void			ft_process_flag_priority(t_tab *tab, int *i);
void			ft_process_flag(t_tab *tab, int *i);

#endif
