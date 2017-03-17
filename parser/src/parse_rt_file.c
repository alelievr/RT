/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/20 22:01:00 by vdaviot           #+#    #+#             */
/*   Updated: 2017/03/17 04:15:54 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include "keywords.h"

#define NEW_OBJECT() (t_object *)malloc(sizeof(t_object))
#define SS(t) while(*t&&ft_isspace(*t))t++; 
#define SKIP_EMPTY_LINE(l) {char*t=l;SS(t);if (*t==0)continue;}

bool			check_obj_line(char *line, char *obj_name, int *indent_level)
{
	char	*k;
	int		space_number;

	space_number = 0;
	(*indent_level) = 0;
	while (*line && (*line == '\t' || *line == ' '))
	{
		if (*line == '\t')
			(*indent_level)++;
		else if (*line == ' ')
			space_number++;
		if (space_number == 4 && !(space_number = 0))
			(*indent_level)++;
		line++;
	}
	if (!*line)
		return false;
	k = line;
	while (*line && *line++ != ':')
		;
	if (!*line)
	{
		if (FOREACH(g_restricted_keywords, keyword))
		{
			if (!ft_strcmp(k, keyword))
				return false;
		}
		ft_strlcpy(obj_name, k, 256);
		return true;
	}
	else
		return false;
}

void			fill_prop_vec3(t_vec3 *pos, char *line, const char *prop)
{
	pos->x = 42;
	pos->y = 42;
	pos->z = 42;
}

void			fill_prop_int(int *type, char *line, const char *prop)
{
	if (strcmp(prop, "type"))
		return ;
	*type = 42;
}

#define FILL_PROP(X, line, pname) _Generic((X), t_vec3 *: fill_prop_vec3, int *: fill_prop_int)(X, line, pname)
#define A(c, line, p1, p2) FILL_PROP(&c-> p1, line, #p1); FILL_PROP(&c-> p2, line, #p2);
void			parse_rt_file(char *file, t_scene *scene)
{
	int			fd;
	char		line[0xF000 + 1];
	char		obj_name[256];
	int			indent_level;
	t_object	*current_object;

	INIT(int, line_count, 0);
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open Failed");
	while (gl(line, &fd))
	{
		SKIP_EMPTY_LINE(line);
		if (check_obj_line(line, obj_name, &indent_level))
		{
			current_object = NEW_OBJECT();
			current_object->indent_level = indent_level;
			continue ;
		}
		if (indent_level >= 10)
			ft_exit("max indentation reached at line: %i\n", line_count);
		if (indent_level == current_object->indent_level + 1)
		{
			A(current_object, line, primivite.type, transform.position);
			printf("object pos: %f/%f/%f\n", current_object->transform.position.x, current_object->transform.position.y, current_object->transform.position.z);
			printf("line: %s\n", line);
		}
		else
			ft_exit("bad indentation at line %i\n", line_count);
	}
	(void)scene;
	close(fd);
}
