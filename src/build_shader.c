/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_shader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avially <avially@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 19:50:38 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/29 07:16:13 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXTURES	512
#define NEW_LINE_LIST ({t_line_list *m; if (!(m = (t_line_list *)malloc(sizeof(t_line_list)))) ft_exit("malloc error !"); m->next = NULL; m->line = NULL; m;})
#define LIST_INSERT(l, s) {t_line_list *tmp = NEW_LINE_LIST; tmp->line = s; tmp->next = l->next; l->next = tmp;}
#define LIST_APPEND(l, s) {t_line_list *tmp = NEW_LINE_LIST; tmp->line = s;	tmp->next = l->next; l->next = tmp; l = tmp;}
#define ISTYPE(x) (obj->primitive.type == x + 1)
#define ISPRIMITIVE (ISTYPE(SPHERE) || ISTYPE(PLANE) || ISTYPE(CYLINDRE) || ISTYPE(CONE) || ISTYPE(CUBE) || ISTYPE(CUBE_TROUE) || ISTYPE(GLASS) || ISTYPE(DISK))
#define ISLIGHT (ISTYPE(POINT_LIGHT) || ISTYPE(DIRECTIONAL_LIGHT) || ISTYPE(SPOT_LIGHT))
#define MAX(x, y) ((x > y) ? x : y)
#define MAX_SHADER_FILE_SIZE	0xF000
#define GET_UVS(img) img.atlas_uv.x, img.atlas_uv.y, img.atlas_uv.z, img.atlas_uv.w
#define GET_VEC4(vec) vec.x, vec.y, vec.z, vec.w
#define LOAD_TEXTURE(m, p, o) printf("texture: %s\n", #p); if (m->has_##p && m->p.file[0]) m->p.data = load_image(build_path(scene_directory, m->p.file), &m->p.width, &m->p.height, &m->p.channels); else m->p.data = generate_image_from_data(m->o, &m->p.width, &m->p.height);
#define LOAD_TEXTURE_ATLAS(m, p, o, aw, ah) LOAD_TEXTURE(m, p, o); *aw += m->p.width; *ah = MAX(*ah, m->p.height);
#define TEXTURE_REPEAT(tex, x, y) ({int _x = x % tex->width; int _y = y % tex->height; apply_channel_mask_pixel((*(unsigned int *)(tex->data + _y * tex->width * tex->channels + _x * tex->channels)), tex->channels);})
#define MIN(x, y) ((x) < (y)) ? (x) : (y)
#define OCTET(x, c) (((x) >> c) & 0xFF)
#define FUSION_PIXEL_COMPONENT(p1, m1, p2, m2, c) (OCTET(m2, c) == 0x00) ? OCTET(p1, c) << c : (((OCTET(p2 & m2, c)) * (OCTET(p1 & m1, c)) / 255) << c)
#define FUSION_TEXTURE_ATLAS(m, p1, p2, aw, ah, m1, m2) { int new_x, new_y; *aw -= m->p1.width; fusion_texture(&m->p1, &m->p2, m1, m2, &new_x, &new_y); *aw += m->p1.width; *ah = MAX(*ah, m->p1.height); }
#define W			0xFF000000
#define XYZ		0xFFFFFFFF
#define COMPUTE_OFFSET(m, p) (t_vec4){(float)*offset_x / (float)atlas->width, (float)*offset_y / (float)atlas->height, (float)(*offset_x + m->p.width) / (float)atlas->width, (float)(*offset_y + m->p.height) / (float)atlas->height}
#define ADD_TEXTURE_ATLAS(m, p) add_subimage(atlas, *offset_x, *offset_y, &m->p); m->p.atlas_uv = COMPUTE_OFFSET(m, p); *offset_x += m->p.width; printf("uv: %f/%f - %f/%f\n", m->p.atlas_uv.x, m->p.atlas_uv.y, m->p.atlas_uv.z, m->p.atlas_uv.w)

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
	shader_file->post_processing = shader_file->raytrace_lights;
	LIST_APPEND(shader_file->main_image_begin, strdup("\n/* Static MainImage */\n"));
	LIST_APPEND(shader_file->main_image_begin, strdup(main_image_start_text));
	LIST_APPEND(shader_file->uniform_begin, strdup("/* Generated uniforms */\n"));
}

static void	load_essencial_files(t_shader_file *shader_file, t_file *sources)
{
	const char *const	*files = (const char *const[]){"shaders/tri/scene.glsl", "shaders/tri/plane.glsl", "shaders/tri/sphere.glsl", "shaders/tri/cylinder.glsl", "shaders/tri/cone.glsl", "shaders/tri/cube.glsl", "shaders/tri/cubetroue.glsl", "shaders/tri/glass.glsl", "shaders/tri/disk.glsl", "shaders/tri/light.glsl", "shaders/tri/pproc.glsl", NULL};
	int					fd;
	char				line[0xF000];
	int					i;

	i = 0;
	while (*files)
	{
		if ((fd = open(*files, O_RDONLY)) == -1)
		{
			perror("open");
			exit(-1);
		}
		if (!file_is_regular(fd))
			ft_exit("bad file type: %s\n", *files);
		while (gl(line, &fd))
			LIST_APPEND(shader_file->function_begin, strdup(line));
		close(fd);
		strcpy(sources[i++].path, *files);
		files++;
	}
}

static char	*concat_line_list(t_shader_file *shader_file)
{
	static char		buff[MAX_SHADER_FILE_SIZE + 1];
	t_line_list		*line;
	t_line_list		*prev;
	int				index;

	index = 0;
	line = shader_file->begin;
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
	return (buff);
}

static char		*generate_material_line(t_material *mat)
{
	static char		line[0xF00];

	sprintf(line, "vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f)",
			GET_UVS(mat->texture),
			GET_UVS(mat->emission_map),
			GET_UVS(mat->specular_map),
			GET_UVS(mat->reflection_map),
			GET_UVS(mat->refraction_map),
			GET_UVS(mat->bumpmap));
	return (line);
}

static char		*generate_coupes_line(t_primitive *prim)
{
	static char		line[0xF00];

	sprintf(line, "vec4[5](vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f), vec4(%f, %f, %f, %f)), %d",
		GET_VEC4(prim->slice[0]),
		GET_VEC4(prim->slice[1]),
		GET_VEC4(prim->slice[2]),
		GET_VEC4(prim->slice[3]),
		GET_VEC4(prim->slice[4]),
		prim->nsl);
	return (line);
}

static char		*generate_scene_line(t_object *obj)
{
	static char		line[0xF00];

	if (ISTYPE(SPHERE))
		sprintf(line, "\tsphere(%s_position, %s_rotation, %f, Coupes(%s), Material(%s), r, hit);", obj->name, obj->name, obj->primitive.radius, generate_coupes_line(&obj->primitive), generate_material_line(&obj->material));
	else if (ISTYPE(PLANE))
		sprintf(line, "\tplane(%s_rotation, %s_position, 0, Coupes(%s), Material(%s), r, hit);", obj->name, obj->name, generate_coupes_line(&obj->primitive), generate_material_line(&obj->material));
	else if (ISTYPE(CYLINDRE))
		sprintf(line, "\tcyl(%s_position, %s_rotation, %f, Coupes(%s), Material(%s), r, hit);", obj->name, obj->name, obj->primitive.angle, generate_coupes_line(&obj->primitive), generate_material_line(&obj->material));
	else if (ISTYPE(CONE))
		sprintf(line, "\tcone(%s_position, %s_rotation, %f, Coupes(%s), Material(%s), r, hit);", obj->name, obj->name, obj->primitive.angle, generate_coupes_line(&obj->primitive), generate_material_line(&obj->material));
	else if (ISTYPE(CUBE_TROUE))
		sprintf(line, "\tcubetroue(%s_position, %s_rotation, %f, Material(%s), r, hit);", obj->name, obj->name, obj->primitive.height, generate_material_line(&obj->material));
	else if (ISTYPE(CUBE))
		sprintf(line, "\tcube(%s_position, %s_rotation, %f, Material(%s), r, hit);", obj->name, obj->name, obj->primitive.height, generate_material_line(&obj->material));
	else if (ISTYPE(GLASS))
		sprintf(line, "\tglass(%s_position, %s_rotation, %f, Material(%s), r, hit);", obj->name, obj->name, obj->primitive.height, generate_material_line(&obj->material));
	else if (ISTYPE(DISK))
		sprintf(line, "\tdisk(%s_rotation, %s_position, %f, Material(%s), r, hit);", obj->name, obj->name, obj->primitive.height, generate_material_line(&obj->material));
	else
		return (NULL);
	return (strdup(line));
}

static void		append_uniforms(t_shader_file *shader_file, t_object *obj)
{
	static char		line[0xF000];

	if (!ISTYPE(CAMERA))
	{
		sprintf(line, "uniform vec3 %s_position = vec3(%f, %f, %f);", obj->name, obj->transform.position.x, obj->transform.position.y, obj->transform.position.z);
		LIST_APPEND(shader_file->uniform_begin, strdup(line));
		sprintf(line, "uniform vec3 %s_rotation = vec3(%f, %f, %f);", obj->name, obj->transform.euler_angles.x, obj->transform.euler_angles.y, obj->transform.euler_angles.z);
		LIST_APPEND(shader_file->uniform_begin, strdup(line));
	}
	if (ISLIGHT)
	{
		sprintf(line, "\tcolor += calc_color(r, %s_position, vec3(%f, %f, %f), %f);", obj->name, obj->light_prop.color.x, obj->light_prop.color.y, obj->light_prop.color.z, obj->light_prop.intensity);
		LIST_APPEND(shader_file->raytrace_lights, strdup(line));
	}
	else if (ISTYPE(CAMERA))
	{
		g_move.x = obj->transform.position.x;
		g_move.y = obj->transform.position.y;
		g_move.z = obj->transform.position.z;
		if (obj->camera.fov > 0)
			g_fov = obj->camera.fov;
		if (obj->camera.ambient > 0)
			g_ambient = obj->camera.ambient;
	}
}

static void		append_post_processing(t_shader_file *shader_file, t_camera *c)
{
	if (c->post_processing_mask & BLACK_AND_WHITE)
		LIST_APPEND(shader_file->post_processing, strdup("\tcolor = black_white(color);"));
	if (c->post_processing_mask & CARTOON)
		LIST_APPEND(shader_file->post_processing, strdup("\tcolor = cartoon(color, vec3(0, 0, 0), h, r);"));
	if (c->post_processing_mask & NIGHT_VISION)
		LIST_APPEND(shader_file->post_processing, strdup("\tcolor = nightvision(color);"));
	if (c->post_processing_mask & SEPIA)
		LIST_APPEND(shader_file->post_processing, strdup("\tcolor = sepia(color);"));
	if (c->post_processing_mask & VIGNETTING)
		LIST_APPEND(shader_file->post_processing, strdup("\tcolor = vignetting(color, gl_FragCoord.xy / iResolution.xy);"));
	printf("post processing mask: %i\n", c->post_processing_mask);
}

static void		tree_march(t_shader_file *shader_file, t_object *obj)
{
	int		n_light = 0;
	char	line[0xF000];

	while (obj)
	{
		if (obj->primitive.type == CAMERA + 1)
			append_post_processing(shader_file, &obj->camera);
		if (ISLIGHT)
			n_light++;
		append_uniforms(shader_file, obj);
		LIST_APPEND(shader_file->scene_begin, generate_scene_line(obj));
		if (obj->children)
			tree_march(shader_file, obj->children);
		obj = obj->brother_of_children;
	}
	if (n_light == 0)
	{
		sprintf(line, "\tcolor += calc_color(r, vec3(%f, %f, %f), vec3(%f, %f, %f), %f);", 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f);
		LIST_APPEND(shader_file->raytrace_lights, strdup(line));
	}
}

static unsigned char	*generate_image_from_data(float data, int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret;

	ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);
	printf("generated 1px texture from float: %f\n", data);
	memset(ret, (char)(data * 255), 4);
	*width = 1;
	*height = 1;
	return (ret);
}

static unsigned char	*generate_image_from_data(t_vec3 data, int *width, int *height) __attribute__((overloadable))
{
	unsigned char	*ret;

	ret = (unsigned char *)malloc(sizeof(unsigned char) * 4);
	printf("generated 1px texture from color: %f/%f/%f\n", data.x, data.y, data.z);
	ret[0] = data.x;
	ret[1] = data.y;
	ret[2] = data.z;
	ret[3] = 255;
	*width = 1;
	*height = 1;
	return (ret);
}

static char		*build_path(char *dir, char *file)
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

static unsigned char	*load_image(char *path, int *width, int *height, int *channels)
{
	unsigned char *ret;

	ret = SOIL_load_image(path, width, height, channels, SOIL_LOAD_AUTO);
	return (ret);
}

unsigned int		channeltomask(int chan)
{
	unsigned int	ret;

	ret = 0;
	if (chan == 0)
		return (-1);
	while (chan--)
		ret |= 255 << ((8 * chan));
	return (ret);
}

unsigned int		apply_channel_mask_pixel(unsigned int pixel, int chans)
{
	pixel &= channeltomask(chans);
	if (chans == 3)
	{
		pixel |= 0xFF000000;
	}
	return (pixel);
}

static void			fusion_texture(t_image *dst, t_image *src, int dst_mask, int src_mask, int *new_tex_width, int *new_tex_height)
{
	int					x;
	int					y;
	unsigned char		*img_dst;
	unsigned int		src_pixel;
	unsigned int		dst_pixel;
	unsigned int		result_pixel;

	*new_tex_width = MAX(dst->width, src->width);
	*new_tex_height = MAX(dst->height, src->height);
	img_dst = malloc(sizeof(unsigned char) * 4 * (*new_tex_height) * (*new_tex_width));
	printf("tex size: %i/%i\n", *new_tex_width, *new_tex_height);
	if (FOR(x = 0, x < *new_tex_width, x++))
	{
		if (FOR(y = 0, y < *new_tex_height, y++))
		{
			src_pixel = TEXTURE_REPEAT(src, x, y);
			dst_pixel = TEXTURE_REPEAT(dst, x, y);
			result_pixel = 0;
			result_pixel |= FUSION_PIXEL_COMPONENT(dst_pixel, dst_mask, src_pixel, src_mask, 0);
			result_pixel |= FUSION_PIXEL_COMPONENT(dst_pixel, dst_mask, src_pixel, src_mask, 8);
			result_pixel |= FUSION_PIXEL_COMPONENT(dst_pixel, dst_mask, src_pixel, src_mask, 16);
			result_pixel |= FUSION_PIXEL_COMPONENT(dst_pixel, dst_mask, src_pixel, src_mask, 24);
			*(unsigned int *)(img_dst + (*new_tex_width) * y * 4 + x * 4) = result_pixel;
		}
	}
	dst->channels = 4;
	dst->data = img_dst;
}

static void		load_textures_if_exists(t_material *m, char *scene_directory, int *atlas_width, int *atlas_height)
{
	LOAD_TEXTURE_ATLAS(m, texture, color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, bumpmap, bump, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, emission_map, emission_color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, highlight_map, highlight_color, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, reflection_map, reflection, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, refraction_map, refraction / 10, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, opacity_map, opacity, atlas_width, atlas_height);
	LOAD_TEXTURE_ATLAS(m, specular_map, specular, atlas_width, atlas_height);
	FUSION_TEXTURE_ATLAS(m, texture, opacity_map, atlas_width, atlas_height, XYZ, W);
}

static void		load_atlas(t_object *obj, char *scene_directory, int *atlas_width, int *atlas_height)
{
	while (obj)
	{
		printf("\033[38;5;42mobj: %s\033[0m\n", obj->name);
		load_textures_if_exists(&obj->material, scene_directory, atlas_width, atlas_height);
		if (obj->children)
			load_atlas(obj->children, scene_directory, atlas_width, atlas_height);
		obj = obj->brother_of_children;
	}
}

static void		add_subimage(t_atlas *atlas, int offset_x, int offset_y, t_image *img)
{
	unsigned char	*begin;
	unsigned char	*imgdata;
	int				x;
	int				y;
	unsigned int	color;

	imgdata = img->data;
	x = 0;
	y = 0;
	printf("channels: %i\n", img->channels);
	while (y < img->height)
	{
		begin = atlas->data + (offset_x * 4) + ((offset_y + y) * atlas->width * 4);
		x = 0;
		while (x < img->width)
		{
			color = (*(unsigned int *)imgdata) & channeltomask(img->channels);
			*(unsigned int *)begin = color;
			begin += 4;
			imgdata += img->channels;
			x++;
		}
		y++;
	}
	free(img->data);
}

static void		add_object_textures_to_atlas(t_material *mat, t_atlas *atlas, int *offset_x, int *offset_y)
{
	ADD_TEXTURE_ATLAS(mat, texture);
	ADD_TEXTURE_ATLAS(mat, bumpmap);
	ADD_TEXTURE_ATLAS(mat, emission_map);
	ADD_TEXTURE_ATLAS(mat, highlight_map);
	ADD_TEXTURE_ATLAS(mat, reflection_map);
	ADD_TEXTURE_ATLAS(mat, refraction_map);
	ADD_TEXTURE_ATLAS(mat, opacity_map);
	ADD_TEXTURE_ATLAS(mat, specular_map);
}

static unsigned int	build_atlas(t_object *obj, int atlas_width, int atlas_height, bool first)
{
	t_atlas			atlas;
	int				offset_x;
	int				offset_y;

	offset_x = 1;
	offset_y = 0;
	atlas_width += 1;
	atlas.width = atlas_width;
	atlas.height = atlas_height;
	atlas.data = (unsigned char *)malloc(sizeof(int) * atlas_width * atlas_height);
	memset(atlas.data, 0, sizeof(int) * atlas_width * atlas_height);
	if (first)
		ft_memcpy(atlas.data, (unsigned char *)(char[4]){0, 0, 0, 255}, 4);
	glGenTextures(1, &atlas.id);
	while (obj)
	{
		add_object_textures_to_atlas(&obj->material, &atlas, &offset_x, &offset_y);
		if (obj->children)
			build_atlas(obj->children, atlas_width, atlas_height, false);
		obj = obj->brother_of_children;
	}
	glBindTexture(GL_TEXTURE_2D, atlas.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas_width, atlas_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.data);
	return (atlas.id);
}

char		*build_shader(t_scene *root, char *scene_directory, int *atlas_id, t_file *sources)
{
	t_shader_file		shader_file;
	int					atlas_width;
	int					atlas_height;

	atlas_width = 0;
	atlas_height = 0;
	init_shader_file(&shader_file);
	load_essencial_files(&shader_file, sources);
	load_atlas(root->root_view, scene_directory, &atlas_width, &atlas_height);
	*atlas_id = build_atlas(root->root_view, atlas_width, atlas_height, true);
	tree_march(&shader_file, root->root_view);
	return (concat_line_list(&shader_file));
}
