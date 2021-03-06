/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ispunct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/27 01:59:05 by alelievr          #+#    #+#             */
/*   Updated: 2014/11/27 02:00:25 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_ispunct(int c)
{
	if (c != ' ' && !ft_isalnum(c) && ft_isprint(c))
		return (1);
	return (0);
}
