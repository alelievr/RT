/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 10:06:00 by alelievr          #+#    #+#             */
/*   Updated: 2016/11/04 11:23:21 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(char const *s1)
{
  	char	*str;
  	int		i;

  	i = 0;
  	str = (char *)malloc(sizeof(char) * ft_strlen(s1) + 1);
  	if (str == NULL)
  		return (NULL);
	ft_strcpy(str, s1);
  	return (str);
}
