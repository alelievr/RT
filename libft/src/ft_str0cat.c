/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str0cat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 15:29:42 by alelievr          #+#    #+#             */
/*   Updated: 2016/01/20 15:29:43 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str0cat(char *s1, char *s2)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = (int)ft_strlen(s1);
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = ft_strdup(s2);
	while (str[i])
	{
		s1[len + i] = str[i];
		i++;
	}
	return (s1);
}
