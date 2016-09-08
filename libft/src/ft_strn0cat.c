/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 10:19:29 by alelievr          #+#    #+#             */
/*   Updated: 2016/01/20 15:29:53 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strn0cat(char *s1, char *s2, int l)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = (int)ft_strlen(s1);
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = ft_strdup(s2);
	while (str[i] && i < l)
	{
		s1[len + i] = str[i];
		i++;
	}
	return (s1);
}
