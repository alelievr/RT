/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <pmartine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/02 21:54:21 by pmartine          #+#    #+#             */
/*   Updated: 2017/05/03 03:20:14 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

void      parse_rt_file_end(int indent_level, int nb_object, int line_count, t_object *c, char *line)
{
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

bool      parse_rt_file_boucle(char *line, t_scene **scene, int *indent_level, t_object **c, int *nb_object)
{
	char		obj_name[256];
	t_object	*o;

	o = *c;
	if (check_obj_line(line, obj_name, indent_level))
	{
		NEW_OBJECT((*c), obj_name);
		(*c)->indent_level = (*indent_level);
		(*scene)->nb_object = ++(*nb_object);
		if ((*scene)->nb_object == 1)
		{
			(*scene)->root_view = (*c);
			o = *c;
		}
		else
			o->brother_of_children = (*c);
		o = (*c);
		return (true);
	}
	return (false);
}

void			parse_rt_file(char *file, t_scene *scene)
{
	int			fd;
	char		line[0xF000 + 1];
	t_object	*c;

	c = NULL;
	INIT(int, indent_level, 0);
	INIT(int, nb_object, 0);
	INIT(int, line_count, 0);
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open Failed: %s\n", file);
	if (!file_is_regular(fd))
		ft_exit("bad file: %s\n", file);
	while (gl(line, &fd))
	{
		SKIP_EMPTY_LINE(line);
		if (parse_rt_file_boucle(line, &scene, &indent_level, &c, &nb_object))
			continue ;
		parse_rt_file_end(indent_level, nb_object, line_count, c, line);
		close(fd);
	}
}
