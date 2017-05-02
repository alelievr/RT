/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <pmartine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/02 21:54:21 by pmartine          #+#    #+#             */
/*   Updated: 2017/05/03 01:59:01 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

char		*format_name(char *name)
{
	char	*ret;

	ret = name;
	while (*name)
	{
		if (*name == ':')
		{
			*name = 0;
			break ;
		}
		if (!isalnum(*name))
			*name = '_';
		name++;
	}
	return (ret);
}

void			display_objects(t_object *lst_obj)
{
	while (lst_obj)
	{
		if (lst_obj->children)
			display_objects(lst_obj->children);
		lst_obj = lst_obj->brother_of_children;
	}
}

bool		name_already_exists(t_object *obj, char *name)
{
	if (obj != NULL)
		return (false);
	while (obj)
	{
		if (!ft_strcmp(obj->name, name))
			return (true);
		obj = obj->brother_of_children;
	}
	return (false);
}

void			parse_rt_file(char *file, t_scene *scene)
{
	int			fd;
	char		line[0xF000 + 1];
	char		obj_name[256];
	int			indent_level;
	int			nb_object;
	t_object	*c;
	t_object	*o;

	c = NULL;
	nb_object = 0;
	INIT(int, line_count, 0);
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open Failed: %s\n", file);
	if (!file_is_regular(fd))
		ft_exit("bad file: %s\n", file);
	while (gl(line, &fd))
	{
		SKIP_EMPTY_LINE(line);
		if (check_obj_line(line, obj_name, &indent_level))
		{
			NEW_OBJECT(c, obj_name);
			c->indent_level = indent_level;
			scene->nb_object = ++nb_object;
			if (scene->nb_object == 1)
			{
				scene->root_view = c;
				o = c;
			}
			else if (o->indent_level < c->indent_level)
			{
				o->children = c;
				c->parent = o;
			}
			while (o->indent_level > c->indent_level)
				o = o->parent;
			if (scene->nb_object != 1 && o->indent_level == c->indent_level)
			{
				o->brother_of_children = c;
				c->parent = o->parent;
			}
			o = c;
			continue ;
		}
		if (indent_level >= 2)
			ft_exit("max indentation reached at line: %i\n", line_count);
		if (nb_object > 300)
			ft_exit("nb max object reached at line: %i\n", line_count);
		if (c == NULL)
			ft_exit("unexpected property without object: %s\n", line);
		if (indent_level == c->indent_level + 1)
		{
			A(c, line, primitive);
			A(c, line, transform);
			A(c, line, move);
			A(c, line, rotate);
			if (c->primitive.type >= 11)
				A(c, line, light_prop);
			if (c->primitive.type == 10)
				A(c, line, camera);
			if (c->primitive.type <= 9)
				A(c, line, material);
		}
		else
			ft_exit("bad indentation at line %i\n", line_count);
	}
	display_objects(scene->root_view);
	close(fd);
}
