/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/03 17:31:53 by alelievr          #+#    #+#             */
/*   Updated: 2016/03/08 15:51:54 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	register size_t		len;

	if (n >= 8)
	{
		while (!(n % 8))
		{
			*(unsigned char *)s++ = 0;
			n--;
		}
		len = n / 8;
		while (len--)
		{
			*(t_op *)s = 0;
			s += 8;
			n -= 8;
		}
	}
	while (n--)
		*(unsigned char *)s++ = 0;
}
