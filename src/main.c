/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/25 23:30:02 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/28 06:11:52 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define VEC3_ADD_DIV(v1, v2, f) { v1.x += v2.x / (f); v1.y += v2.y / (f); v1.z += v2.z / (f); }
#define VEC3_UP ((vec3){0, 1, 0})

vec4	g_mouse = {0, 0, 0, 0};
vec4	g_move = {0, 0, 0, 1};
vec2	g_window = {WIN_W, WIN_H};
vec3	g_forward = {0, 0, 1};
int		g_keys = 0;
int		g_input_pause = 0;
long	g_last_modified_file[0xF00] = {0};
float	g_paused_time = 0;
float	g_fov = 1.2;
float	g_ambient = 0.2;
int		g_selected_object_pos = -1;
int		g_selected_object_dir = -1;
vec3	g_selected_position;
vec3	g_selected_rotation;

static void		usage(const char *n) __attribute__((noreturn));
static void		usage(const char *n)
{
	printf("usage: %s <shader>\n", n);
	exit(0);
}

GLuint		createVBO(void)
{
	float points[] = {
   		-1.0f,  -1.0f,
    	-1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f,
   		-1.0f,  -1.0f,
	};
	GLuint vbo;

	vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	return vbo;
}

GLuint		createVAO(GLuint vbo, int program)
{
	GLint		frag_pos;
	GLuint		vao = 0;

	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	frag_pos = glGetAttribLocation(program, "fragPosition");
	glEnableVertexAttribArray(frag_pos);
	glVertexAttribPointer(frag_pos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);

	return vao;
}

float		getCurrentTime(void)
{
	struct timeval	t;
	static time_t	l_time = 0;

	if (l_time == 0)
		l_time = time(NULL);
	gettimeofday(&t, NULL);
	return (float)(time(NULL) - l_time) + (float)t.tv_usec / 1000000.0 - g_paused_time;
}

void		updateUniforms(GLint *unis, GLint *images)
{
	static int		frames = 0;

	float ti = getCurrentTime();
	glUniform1f(unis[0], ti);
	glUniform1i(unis[1], frames++);
	glUniform4f(unis[2], g_mouse.x, WIN_H - g_mouse.y, g_mouse.y, g_mouse.y);
	glUniform3f(unis[3], g_forward.x, g_forward.y, g_forward.z);
	glUniform4f(unis[4], g_move.x, g_move.y, g_move.z, g_move.w);
	glUniform2f(unis[5], g_window.x, g_window.y);
  	glUniform1f(unis[6], g_fov);
  	glUniform1f(unis[7], g_ambient);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, images[0]);
	glUniform1i(unis[9], images[0]);

	if (g_selected_object_pos != -1)
		glUniform3f(g_selected_object_pos, g_selected_position.x, g_selected_position.y, g_selected_position.z);
	if (g_selected_object_dir != -1)
		glUniform3f(g_selected_object_dir, g_selected_rotation.x, g_selected_rotation.y, g_selected_rotation.z);
}

vec3		vec3_cross(vec3 v1, vec3 v2)
{
	return (vec3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

void		update_keys(void)
{
	vec3	right;
	vec3	up;

	right = vec3_cross(g_forward, VEC3_UP);
	up = vec3_cross(g_forward, right);
	if (BIT_GET(g_keys, RIGHT))
		VEC3_ADD_DIV(g_move, right, 10 / g_move.w);
	if (BIT_GET(g_keys, LEFT))
		VEC3_ADD_DIV(g_move, -right, 10 / g_move.w);
	if (BIT_GET(g_keys, UP))
		VEC3_ADD_DIV(g_move, up, 10 / g_move.w);
	if (BIT_GET(g_keys, DOWN))
		VEC3_ADD_DIV(g_move, -up, 10 / g_move.w);
	if (BIT_GET(g_keys, FORWARD))
		VEC3_ADD_DIV(g_move, g_forward, 10 / g_move.w);
	if (BIT_GET(g_keys, BACK))
		VEC3_ADD_DIV(g_move, -g_forward, 10 / g_move.w);
	if (BIT_GET(g_keys, PLUS))
		g_move.w *= 1 + g_move_AMOUNT;
	if (BIT_GET(g_keys, MOIN))
		g_move.w *= 1 - g_move_AMOUNT;
}

void		loop(GLFWwindow *win, GLuint program, GLuint vao, GLint *unis, GLint *images)
{
	float	ratio;
	int		width;
	int		height;

	glfwGetFramebufferSize(win, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	update_keys();
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);
	updateUniforms(unis, images);
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	glfwSwapBuffers(win);
	if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		glfwSetCursorPos(win, g_window.x / 2, g_window.y / 2);
	glfwPollEvents();
}

GLint		*getUniformLocation(GLuint program)
{
	static GLint unis[0xF0];

	unis[0] = glGetUniformLocation(program, "iGlobalTime");
	unis[1] = glGetUniformLocation(program, "iFrame");
	unis[2] = glGetUniformLocation(program, "iMouse");
	unis[3] = glGetUniformLocation(program, "iForward");
	unis[4] = glGetUniformLocation(program, "iMoveAmount");
	unis[5] = glGetUniformLocation(program, "iResolution");
  	unis[6] = glGetUniformLocation(program, "iFov");
  	unis[7] = glGetUniformLocation(program, "iAmbient");
	unis[9] = 8;
	return unis;
}

t_scene	*get_scene(t_scene *scene)
{
	static t_scene	*scn = NULL;

	if (scene != NULL)
		scn = scene;
	return (scn);
}

int		get_program(int p)
{
	static int		program = -1;

	if (p != -1)
		program = p;
	return p;
}

static char		*get_scene_directory(char *scene_dir)
{
	static char		*scene_directory = NULL;

	if (scene_dir)
		scene_directory = scene_dir;
	return scene_directory;
}

void		initSourceFiles(t_file *files, size_t max, size_t *num)
{
	struct stat		st;

	*num = 0;
	while (42)
	{
		if (*num >= max)
			break ;
		files[*num].fd = open(files[*num].path, O_RDONLY);
		if (stat(files[*num].path, &st) == -1)
			return ;
		g_last_modified_file[files[*num].fd] = st.st_mtime;
		printf("initialized source file: %s\n", files[*num].path);
		if (!S_ISREG(st.st_mode) || !(st.st_mode & S_IRUSR))
			printf("not a valid file: %s\n", files[*num].path), exit(-1);
		else
			(*num)++;
	}
}

void		checkFileChanged(GLuint *program, t_file *files, size_t num)
{
	struct stat		st;
	size_t			i;
	int				fd;
	int				atlas_id;

	i = 0;
	while (i < num)
	{
		if (!FILE_CHECK_EXT(files[i].path, "glsl"))
			continue ;
		stat(files[i].path, &st);
		if (g_last_modified_file[files[i].fd] != st.st_mtime)
		{
			printf("file modified: %s\n", files[i].path);
			g_last_modified_file[files[i].fd] = st.st_mtime;
			close(files[i].fd);
			if ((fd = open(files[i].path, O_RDONLY)) != -1)
				files[i].fd = fd;
			else
				break ;
			char *file = build_shader(get_scene(NULL), get_scene_directory(NULL), &atlas_id, files);
			GLint new_program = create_program(file, false);
			if (new_program != 0)
			{
				glDeleteProgram(*program);
				*program = new_program;
				printf("shader reloaded !\n");
				getUniformLocation(*program);
			}
		}
		i++;
	}
}

int			checkFileExtention(char *file, char **exts)
{
	char	*ext;

	ext = file + strlen(file) - 1;
	while (ext != file && *ext != '.')
		ext--;
	ext++;
	while (*exts)
	{
		if (!strcmp(ext, *exts))
			return (1);
		exts++;
	}
	return (0);
}

GLint		*loadImages(char **av)
{
	static GLint	texts[0xF0];
	int				k;

	k = 0;
	for (int i = 0; av[i]; i++)
	{
		if (!checkFileExtention(av[i], (char *[]){"png", "jpg", "tiff", "jpeg", "tga", NULL}))
			continue ;
		texts[k] = SOIL_load_OGL_texture(av[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);
		if (texts[k] == 0)
			printf("can't load texture: %s\n", SOIL_last_result()), exit(-1);
		k++;
	}
	return texts;
}

void		display_window_fps(GLFWwindow *win)
{
	static int		frames = 0;
	static double	last_time = 0;
	double			current_time;
	char			fps[0xF00];

	current_time = glfwGetTime();
	frames++;
	if (current_time - last_time >= 1.0)
	{
		sprintf(fps, "Re Tweet - fps [%.3i ]", (int)(1.0 / (1000.0 / (float)frames) * 1000.0));
		glfwSetWindowTitle(win, fps);
		frames = 0;
		last_time++;
	}
}

static void load_scene_directory(char *scene_file)
{
	static char		path[1024];
	char			*off;

	if ((off = strrchr(scene_file, '/')))
	{
		strncpy(path, scene_file, off - scene_file);
		path[off - scene_file] = 0;
	}
	else
		strcpy(path, ".");
	strcat(path, "/");
	get_scene_directory(path);
}

int			main(int ac, char **av)
{
	static t_file	sources[0xF00];
	size_t			num;
	double			t1;
	t_scene			scene;
	char			*program_source;
	int				atlas_id;

	if (ac < 1)
		usage(*av);
  	parse_rt_file(av[1], &scene);
	printf("test parse\n");
	load_scene_directory(av[1]);

	get_scene(&scene);

	GLFWwindow *win = init("Re Tweet");

	program_source = build_shader(&scene, get_scene_directory(NULL), &atlas_id, sources);

	initSourceFiles(sources, 0xF00, &num);

	GLuint		program = create_program(program_source, true);
	GLuint		vbo = createVBO();
	GLuint		vao = createVAO(vbo, program);
    GLint		*unis = getUniformLocation(program);
    GLint		*images = (int[]){atlas_id};

	get_program(program);

	while ((t1 = glfwGetTime()), !glfwWindowShouldClose(win))
	{
		checkFileChanged(&program, sources, num);
		loop(win, program, vao, unis, images);
		display_window_fps(win);
	}
	glfwTerminate();
	return (0);
}
