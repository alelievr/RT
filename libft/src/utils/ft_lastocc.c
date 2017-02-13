/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lastocc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/27 17:10:57 by alelievr          #+#    #+#             */
/*   Updated: 2016/01/20 15:01:01 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lastocc(char *s, char c)
{
	int		i;

	i = (int)ft_strlen(s) - 1;
	while (i >= 0)
	{
		if (s[i] == c)
			return (i);
		i--;
	}
	return (-1);
}