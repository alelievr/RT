/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 11:09:47 by alelievr          #+#    #+#             */
/*   Updated: 2016/07/27 16:02:35 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(char const *s, int c)
{
	const unsigned long	*lptr = (const unsigned long *)s;
	unsigned long		r_c;

	r_c = (unsigned char)c | (c << 8);
	r_c |= r_c << 16;
	r_c |= r_c << 32;
	while (42)
	{
		if (LONGCHR(*lptr, r_c))
		{
			s = (const char *)lptr;
			IFRET__(s[0] == c && ft_strlen(s) >= 0, (char *)(s + 0));
			EIFRET__(s[1] == c && ft_strlen(s) >= 1, (char *)(s + 1));
			EIFRET__(s[2] == c && ft_strlen(s) >= 2, (char *)(s + 2));
			EIFRET__(s[3] == c && ft_strlen(s) >= 3, (char *)(s + 3));
			EIFRET__(s[4] == c && ft_strlen(s) >= 4, (char *)(s + 4));
			EIFRET__(s[5] == c && ft_strlen(s) >= 5, (char *)(s + 5));
			EIFRET__(s[6] == c && ft_strlen(s) >= 6, (char *)(s + 6));
			EIFRET__(s[7] == c && ft_strlen(s) >= 7, (char *)(s + 7));
			ERET__(NULL);
		}
		if (LONGCHR_NULL(*lptr))
			return (NULL);
		lptr++;
	}
}
