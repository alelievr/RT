/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 20:29:19 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/28 20:32:03 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"
#include <stdio.h>

int		main(void)
{
	char	buff[0xF000];

	ft_printf("olol %10.5i !\n", 42);
	printf("olol %10.5i !\n", 42);
	ft_printf("olol %.1s !\n", "ABCDEFGH");
	printf("olol %.5s !\n", "ABCDEFGH");
	ft_printf("olol %hhx !\n", 4200);
	printf("olol %hhx !\n", 4200);
	ft_printf("olol %p !\n", 0x424242424242);
	printf("olol %p !\n", 0x424242424242);
	(void)buff;
}
