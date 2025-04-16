/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:02:22 by kizuna            #+#    #+#             */
/*   Updated: 2025/04/15 20:38:30 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strnchr(const char *s, int c, int start, int end)
{
	while (s[start] != (const char)end)
	{
		if (((unsigned char *)s)[start] == (unsigned char)c)
			return (start);
		start++;
	}
	return (-1);
}
