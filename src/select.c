/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 05:53:41 by pmartine          #+#    #+#             */
/*   Updated: 2017/05/02 20:22:54 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"

void		select_object(void)
{
	t_scene		*scene;
	t_object	*obj;
	int			i;
	char		buff[0xF000];

	scene = get_scene(NULL);
	if (g_selected_object_index < 0)
		g_selected_object_index = scene->nb_object - 1;
	if (g_selected_object_index > scene->nb_object - 1)
		g_selected_object_index = 0;
	obj = scene->root_view;
	i = 0;
	while (obj)
	{
		if (g_selected_object_index == i)
		{
			if (obj->primitive.type == CAMERA + 1)
				return (g_selected_object_index++, select_object());
			g_selected_object.pos = &obj->transform.initial_position;
			g_selected_object.dir = &obj->transform.euler_angles;
			sprintf(buff, "%s_position", obj->name);
			g_selected_object.pos_uniform = \
				glGetUniformLocation(get_program(-1), buff);
			sprintf(buff, "%s_rotation", obj->name);
			g_selected_object.dir_uniform = \
				glGetUniformLocation(get_program(-1), buff);
			printf("selected object: %s\n", obj->name);
		}
		obj = obj->brother_of_children;
		i++;
	}
}
