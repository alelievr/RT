/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 20:29:19 by alelievr          #+#    #+#             */
/*   Updated: 2016/08/30 22:49:14 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"

int		main(void) {
	char	buff[0xF000];

	ft_concat(buff, (unsigned long long)10, 5, -30, F_DECIMAL | F_32BIT | F_SIGNED);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)-42, -5, -30, F_DECIMAL | F_32BIT | F_SIGNED);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)-42, -5, 30, F_DECIMAL | F_32BIT | F_SIGNED | F_ZERO | F_PLUS);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)0, 5, -30, F_DECIMAL | F_32BIT | F_SIGNED);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)56958458, 50, NOALIGN, F_HEXA | F_32BIT | F_SIGNED);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)56958458, 50, -30, F_HEXA | F_32BIT | F_SIGNED | F_UPPERCASE);
	printf("%s\n", buff);
	ft_concat(buff, (unsigned long long)88, NOPADD, NOALIGN, F_32BIT | F_SIGNED | F_LOWERCASE | F_BINARY);
	printf("%s\n", buff);
	ft_concat(buff, "yolol !", 3, 10, 0);
	printf("%s\n", buff);
	ft_concat(buff, (wchar_t *)L"AAπ |こんにちは|", 3, NOALIGN, 0);
	printf("%s\n", buff);
/*
	clock_t	big, mid, end;
	big = clock();
	for (int i = 0; i < 10000; i++)
		ft_concat(buff, (unsigned long long)0x42, -6, 20, F_HEXA | F_ZERO | F_PREFIX | F_32BIT);
	printf("here\n");
	mid = clock();
	for (int i = 0; i < 10000; i++)
		ft_sprintf(buff, "%i", 0x42);
	end = clock();
	printf("%ld\n%ld\n", mid - big, end - mid);*/
	return (0);
}
