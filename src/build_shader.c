/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_shader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 19:50:38 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/11 02:59:12 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

typedef struct	s_line_list
{
	char				*line;
	struct s_line_list	*next;
}				t_line_list;

typedef struct
{
	t_line_list		*begin;
	t_line_list		*function_begin;
	t_line_list		*uniform_begin;
	t_line_list		*main_image_begin;
	t_line_list		*raytrace_lights;
	t_line_list		*scene_begin;
	t_line_list		*scene_end;
}			t_shader_file;

#define MAX_TEXTURES	512

#define NEW_LINE_LIST ({t_line_list *m; if (!(m = (t_line_list *)malloc(sizeof(t_line_list)))) ft_exit("malloc error !"); m->next = NULL; m->line = NULL; m;})
#define LIST_INSERT(l, s) {t_line_list *tmp = NEW_LINE_LIST; tmp->line = s; tmp->next = l->next; l->next = tmp;}
#define LIST_APPEND(l, s) {t_line_list *tmp = NEW_LINE_LIST; tmp->line = s; tmp->next = l->next; l->next = tmp; l = tmp;}

#define ISTYPE(x) (obj->primitive.type == x)
#define ISPRIMITIVE (ISTYPE(SPHERE) || ISTYPE(PLANE) || ISTYPE(CYLINDRE) || ISTYPE(CONE))

#define MAX(x, y) ((x > y) ? x : y)

static void	init_shader_file(t_shader_file *shader_file)
{
	if (!(shader_file->begin = NEW_LINE_LIST))
		ft_exit("malloc error !");
	LIST_APPEND(shader_file->begin, strdup("#version 330\n/* AutoGenerated shader !* /\n\n/* Main and standard uniforms */\n"));
	LIST_APPEND(shader_file->begin, strdup(fragment_shader_text));
	shader_file->uniform_begin = shader_file->begin;
	shader_file->function_begin = shader_file->begin;
	LIST_APPEND(shader_file->function_begin, strdup("/* Static functions */\n"));
	LIST_APPEND(shader_file->function_begin, strdup(shader_header_text));
	shader_file->main_image_begin = shader_file->function_begin;
	shader_file->scene_begin = shader_file->function_begin;
	LIST_INSERT(shader_file->scene_begin, strdup("/* Generated scene from parser */\n"));
	shader_file->scene_end = shader_file->scene_begin->next;
	LIST_APPEND(shader_file->scene_begin, strdup(scene_start_text));
	LIST_APPEND(shader_file->scene_end, strdup(scene_end_text));
	shader_file->raytrace_lights = shader_file->scene_end;
	LIST_APPEND(shader_file->raytrace_lights, strdup(raytrace_start_text));
	LIST_INSERT(shader_file->raytrace_lights, strdup(raytrace_end_text));
	LIST_APPEND(shader_file->main_image_begin, strdup("\n/* Static MainImage */\n"));
	LIST_APPEND(shader_file->main_image_begin, strdup(main_image_start_text));
	LIST_APPEND(shader_file->uniform_begin, strdup("/* Generated uniforms */\n"));
}

static void	load_essencial_files(t_shader_file *shader_file)
{
	const char * const	*files = (const char * const[]){"shaders/new.glsl", NULL};
	struct stat			st;
	int					fd;
	char				line[0xF000];

	while (*files)
	{
		if ((fd = open(*files, O_RDONLY)) == -1)
			perror("open"), exit(-1);
		fstat(fd, &st);
		if (!S_ISREG(st.st_mode))
			ft_exit("bad file type: %s\n", *files);
		while (gl(line, &fd))
			LIST_APPEND(shader_file->function_begin, strdup(line));
		files++;
	}
}

#define MAX_SHADER_FILE_SIZE	0xF000
static char	*concat_line_list(t_shader_file *shader_file)
{
	static char		buff[MAX_SHADER_FILE_SIZE + 1];
	t_line_list		*line = shader_file->begin;
	t_line_list		*prev;
	int				index;

	index = 0;
	while (line)
	{
		if (line->line == NULL)
		{
			line = line->next;
			continue ;
		}
		strcpy(buff + index, line->line);
		index += strlen(line->line);
		free(line->line);
		buff[index++] = '\n';
		prev = line;
		line = line->next;
		free(prev);
	}
	write(1, buff, index);
	return buff;
}

static void		format_name(char *name)
{
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
}

static char		*generate_material_line(t_object *obj)
{
	static char		line[0xF00];

	sprintf(line, "vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), vec4(0, 0, 1, 1), vec4(0, 0, 1, 1)");
	return (line);
}

static char		*generate_scene_line(t_object *obj)
{
	static char		line[0xF00];

	if (ISTYPE(SPHERE))
		sprintf(line, "\tsphere(%s_position, %f, Material(%s), r, hit);", obj->name, obj->primitive.radius, generate_material_line(obj));
//	else if (ISTYPE(PLANE))
//		sprintf(line, "\tplane(%s_rotation, %s_position, vec3(0, 1, 0), Material(%s), r, hit);", obj->name, obj->name, generate_material_line(obj));
	else if (ISTYPE(CYLINDRE))
		sprintf(line, "\tcyl(%s_position, %s_rotation, %f, Material(%s), r, hit);", obj->name, obj->name, obj->primitive.angle, generate_material_line(obj));
	else if (ISTYPE(CONE))
		sprintf(line, "\tsphere(%s_position, %s_rotation, Material(%s), r, hit);", obj->name, obj->name, generate_material_line(obj));
	else
		return (NULL);
	return (strdup(line));
}

static void		append_uniforms(t_shader_file *shader_file, t_object *obj)
{
	static char		line[0xF000];

	if (ISPRIMITIVE)
	{
		sprintf(line, "uniform vec3 %s_position;", obj->name);
		LIST_APPEND(shader_file->uniform_begin, strdup(line));
		sprintf(line, "uniform vec3 %s_rotation;", obj->name);
		LIST_APPEND(shader_file->uniform_begin, strdup(line));
	}
	else if (!ISTYPE(CAMERA)) //lighs
	{
		sprintf(line, "\tcolor += calc_light(vec3(%f, %f, %f), r, h);", obj->transform.position.x, obj->transform.position.y, obj->transform.position.z);
		LIST_APPEND(shader_file->raytrace_lights, strdup(line));
	}
}

static void		tree_march(t_shader_file *shader_file, t_scene *scene)
{
	int			obj_count = 0;
	t_object	*obj;

	obj = scene->root_view;
	while (obj_count < scene->nb_object)
	{
		format_name(obj->name);
		append_uniforms(shader_file, obj);
		LIST_APPEND(shader_file->scene_begin, generate_scene_line(obj));

		if (obj->children)
			obj = obj->children;
		else if (obj->brother_of_children)
			obj = obj->brother_of_children;
		else
			obj = obj->parent->brother_of_children;
		obj_count++;
	}
}

static unsigned char	*generate_image_from_data(float data, int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);

	memset(ret, (char)(data * 255), 4);
	*width = 1;
	*height = 1;
	return ret;
}

static unsigned char	*generate_image_from_data(t_vec3 data, int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);

	printf("generated 1px texture from color: %f/%f/%f\n", data.x, data.y, data.z);
	ret[0] = data.x;
	ret[1] = data.y;
	ret[2] = data.z;
	ret[3] = 255;
	*width = 1;
	*height = 1;
	return ret;
}

#define LOAD_TEXTURE(m, p, o) if (m->has_##p) m->p.data = SOIL_load_image(m->p.file, &m->p.width, &m->p.height, 0, SOIL_LOAD_RGBA); else m->p.data = generate_image_from_data(m->o, &m->texture.width, &m->texture.height);
#define LOAD_TEXTURE_ATLAS(m, p, o, aw, ah) LOAD_TEXTURE(m, p, o); *aw += m->p.width; *ah = MAX(*ah, m->p.height);

static void		load_textures_if_exists(t_material *m, int *atlas_width, int *atlas_height)
{
	LOAD_TEXTURE_ATLAS(m, texture, color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, bumpmap, bump, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, emission_map, emission_color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, highlight_map, highlight_color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, reflection_map, reflection, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, refraction_map, refraction, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, transparency_map, transparency, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, specular_map, specular, atlas_width, atlas_height);
}

static void		load_atlas(t_scene *scene)
{
	int			obj_count = 0;
	t_object	*obj;
	int			atlas_width = 0;
	int			atlas_height = 0;

	obj = scene->root_view;
	while (obj_count < scene->nb_object)
	{
		format_name(obj->name);

		load_textures_if_exists(&obj->material, &atlas_width, &atlas_height);

		if (obj->children)
			obj = obj->children;
		else if (obj->brother_of_children)
			obj = obj->brother_of_children;
		else
			obj = obj->parent->brother_of_children;
		obj_count++;
	}
}

char		*build_shader(t_scene *root)
{
	t_shader_file		shader_file;

	init_shader_file(&shader_file);
	load_essencial_files(&shader_file);

	load_atlas(root);

	tree_march(&shader_file, root);

	return concat_line_list(&shader_file);
}
