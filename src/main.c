/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/07/11 18:11:03 by alelievr          #+#    #+#             */
/*   Updated  2016/07/25 18:53:04 by alelievr         ###   ########.fr       */
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

vec4		mouse = {0, 0, 0, 0};
vec4		move = {0, 0, 0, 0};
vec2		window = {WIN_W, WIN_H};
vec3		forward = {0, 0, 1};
int			keys = 0;
int			input_pause = 0;
long		lastModifiedFile[0xF0] = {0};

float points[] = {
   	-1.0f,  -1.0f,
    -1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f,
   	-1.0f,  -1.0f,
};

static void		usage(const char *n) __attribute__((noreturn));
static void		usage(const char *n)
{
	printf("usage: %s <shader>\n", n);
	exit(0);
}


GLuint		createVBO(void)
{
	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	return vbo;
}

GLuint		createVAO(GLuint vbo, int program)
{
	GLint		fragPos;
	GLuint		vao = 0;

	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);

	fragPos = glGetAttribLocation(program, "fragPosition");
	glEnableVertexAttribArray(fragPos);
	glVertexAttribPointer(fragPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);

	return vao;
}

void		updateUniforms(GLint *unis, GLint *images)
{
	struct timeval	t;
	static int		frames = 0;
	static time_t	lTime = 0;
	static int		glTextures[] = {GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8};

	if (lTime == 0)
		lTime = time(NULL);
	gettimeofday(&t, NULL);

	if (!input_pause)
		glUniform1f(unis[0], (float)(time(NULL) - lTime) + (float)t.tv_usec / 1000000.0);
	glUniform1i(unis[1], frames++);
	glUniform4f(unis[2], mouse.x, WIN_H - mouse.y, mouse.y, mouse.y);
	glUniform3f(unis[3], forward.x, forward.y, forward.z);
	glUniform4f(unis[4], move.x, move.y, move.z, move.w);
	glUniform2f(unis[5], window.x, window.y);

	int		i = 0;
	for (int j = 0; j < 8; j++)
		if (images[j] != 0)
		{
			glActiveTexture(glTextures[i++]);
			glBindTexture(GL_TEXTURE_2D, images[j]);
			glUniform1i(unis[10 + j], images[j]);
		}
}

vec3		vec3_cross(vec3 v1, vec3 v2)
{
	return (vec3){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

#define VEC3_ADD_DIV(v1, v2, f) { v1.x += v2.x / f; v1.y += v2.y / f; v1.z += v2.z / f; }
#define VEC3_UP ((vec3){0, 1, 0})
void		update_keys(void)
{
	vec3	right;
	vec3	up;

	right = vec3_cross(forward, VEC3_UP);
	up = vec3_cross(forward, right);
	if (BIT_GET(keys, RIGHT))
		VEC3_ADD_DIV(move, right, 10);
	if (BIT_GET(keys, LEFT))
		VEC3_ADD_DIV(move, -right, 10);
	if (BIT_GET(keys, UP))
		VEC3_ADD_DIV(move, up, 10);
	if (BIT_GET(keys, DOWN))
		VEC3_ADD_DIV(move, -up, 10);
	if (BIT_GET(keys, FORWARD))
		VEC3_ADD_DIV(move, forward, 10);
	if (BIT_GET(keys, BACK))
		VEC3_ADD_DIV(move, -forward, 10);
	if (BIT_GET(keys, PLUS))
		move.w += MOVE_AMOUNT;
	if (BIT_GET(keys, MOIN))
		move.w -= MOVE_AMOUNT;
}

void		loop(GLFWwindow *win, GLuint program, GLuint vao, GLint *unis, GLint *images)
{
	float ratio;
	int width, height;
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
		glfwSetCursorPos(win, window.x / 2, window.y / 2);
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
	unis[10] = glGetUniformLocation(program, "iChannel0");
	unis[11] = glGetUniformLocation(program, "iChannel1");
	unis[12] = glGetUniformLocation(program, "iChannel2");
	unis[13] = glGetUniformLocation(program, "iChannel3");
	unis[14] = glGetUniformLocation(program, "iChannel4");
	unis[15] = glGetUniformLocation(program, "iChannel5");
	unis[16] = glGetUniformLocation(program, "iChannel6");
	unis[17] = glGetUniformLocation(program, "iChannel7");
	return unis;
}

#define	FILE_CHECK_EXT(x, y) (ft_strrchr(x, '.') != NULL && !ft_strcmp(ft_strrchr(x, '.') + 1, y))
char		**getSourceFiles(void)
{
	static char		*files[0xF00];
	char			buff[0xF00];
	int				i;
	struct dirent	*d;
	DIR				*dir;

	i = 0;
	if (!(dir = opendir("./shaders")))
		perror("opendir"), exit(-1);
	while ((d = readdir(dir)))
	{
		if (!FILE_CHECK_EXT(d->d_name, "fs"))
			continue ;
		ft_sprintf(buff, "./shaders/%s", d->d_name);
		files[i++] = strdup(buff);
	}
	files[i] = NULL;
	return files;
}

int			*getFilesFds(char **fnames)
{
	static int		fds[0xF0];
	int				i;
	struct stat		st;

	i = 0;
	fnames--;
	while (*++fnames)
	{
		if (!FILE_CHECK_EXT(*fnames, "fs"))
			continue ;
		fds[i] = open(*fnames, O_RDONLY);
		fstat(fds[i], &st);
		lastModifiedFile[i] = st.st_mtime;
		if (fds[i] == -1 || !S_ISREG(st.st_mode))
			printf("not a valid file: %s\n", *fnames), exit(-1);
		i++;
	}
	return fds;
}

void		checkFileChanged(GLuint *program, char **files, int *fds)
{
	struct stat		st;
	const int		*fd_start = fds;

	fds--;
	while (*++fds)
	{
		if (!FILE_CHECK_EXT(*files, "fs"))
			continue ;
		stat(*files, &st);
		if (lastModifiedFile[fds - fd_start] != st.st_mtime)
		{
			lastModifiedFile[fds - fd_start] = st.st_mtime;
			close(*fds);
			*fds = open(*files, O_RDONLY);
			GLint new_program = create_program((int *)fd_start, false);
			if (new_program != 0)
			{
				glDeleteProgram(*program);
				*program = new_program;
				printf("shader reloaded !\n");
				getUniformLocation(*program);
			}
		}
		files++;
	}
}

int			checkFileExtention(char *file, char **exts)
{
	char	*ext = file + strlen(file) - 1;

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
	int				k = 0;

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

void		display_window_fps(void)
{
	static int		frames = 0;
	static double	last_time = 0;
	double			current_time = glfwGetTime();

	frames++;
	if (current_time - last_time >= 1.0)
	{
		printf("%sfps:%.3f%s", "\x1b\x37", 1.0 / (1000.0 / (float)frames) * 1000.0, "\x1b\x38");
		frames = 0;
		fflush(stdout);
		last_time++;
	}
}

int			main(int ac, char **av)
{
	if (ac < 1)
		usage(*av);

	char		**srcs = getSourceFiles();
	const int	*fds = getFilesFds(srcs);
	double		t1;

	GLFWwindow *win = init("Re Tweet");

	GLuint		program = create_program((int *)fds, true);
	GLuint		vbo = createVBO();
	GLuint		vao = createVAO(vbo, program);
	GLint		*unis = getUniformLocation(program);
	GLint		*images = loadImages(av + 2);

	printf("max textures: %i\n", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	while ((t1 = glfwGetTime()), !glfwWindowShouldClose(win))
	{
		checkFileChanged(&program, srcs, (int *)fds);
		loop(win, program, vao, unis, images);
		display_window_fps();
	}
	close(*fds);
	glfwTerminate();
	return (0);
}
