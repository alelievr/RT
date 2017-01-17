/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/01 23:14:24 by alelievr          #+#    #+#             */
/*   Updated: 2016/11/19 01:55:32 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		main(void)
{
	const char *table[] = {"YOLO", "OLOL", NULL};

	if (FOREACH(table, t))
		printf("%s\n", t);

	LIST	l;

	LIST_INIT(l, false);
	LIST_PUSH_BACK(l, 12);
	printf("data: %i\n", LIST_CURRENT(l, int));
/*	LIST_PUSH_BACK(l, "yolo !");

	printf("data: %s\n", LIST_CURRENT(l, char *));
	printf("data: %s\n", LIST_CURRENT2(l));*/
}
