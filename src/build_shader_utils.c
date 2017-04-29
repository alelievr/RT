/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_shader_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/29 14:32:36 by pmartine          #+#    #+#             */
/*   Updated: 2017/04/29 14:45:52 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "build_shader_include.h"
#include "shaderpixel.h"

char			*generate_material_line(t_material *mat)
{
	static char		line[0xF00];

	sprintf(line, "vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), \
vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), \
vec4(%f, %f, %f, %f)",
			GET_UVS(mat->texture),
			GET_UVS(mat->emission_map),
			GET_UVS(mat->specular_map),
			GET_UVS(mat->reflection_map),
			GET_UVS(mat->refraction_map),
			GET_UVS(mat->bumpmap));
	return (line);
}

unsigned char	*generate_image_from_data(float data, \
int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret;

	ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);
	printf("generated 1px texture from float: %f\n", data);
	memset(ret, (char)(data * 255), 4);
	*width = 1;
	*height = 1;
	return (ret);
}

unsigned char	*generate_image_from_data(t_vec3 data, \
int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret;

	ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);
	printf("generated 1px texture from color: %f/%f/%f\n", \
	data.x, data.y, data.z);
	ret[0] = data.x;
	ret[1] = data.y;
	ret[2] = data.z;
	ret[3] = 255;
	*width = 1;
	*height = 1;
	return (ret);
}

char			*build_path(char *dir, char *file)
{
	static char		path[1024];
	char			*f;

	f = file + strlen(file) - 1;
	while (isspace(*f))
		*f-- = 0;
	f = file;
	while (isspace(*f))
		f++;
	strcpy(path, dir);
	strcat(path, file);
	return (path);
}

unsigned char	*load_image(char *path, int *width, int *height, \
	int *channels)
{
	unsigned char *ret;

	ret = SOIL_load_image(path, width, height, channels, SOIL_LOAD_AUTO);
	return (ret);
}
