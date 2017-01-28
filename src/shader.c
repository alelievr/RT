/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/09/08 21:25:14 by alelievr          #+#    #+#             */
/*   Updated  2016/09/08 21:25:20 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

static void		check_compilation(GLuint shader, bool fatal)
{
	GLint	is_compiled;
	GLint	max_length;
	char	buff[0xF000];

	is_compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, buff);
		printf("%s\n", buff);
		glDeleteShader(shader);
		if (fatal)
		{
			printf("shader compilation error, exiting\n");
			exit(-1);
		}
	}
}

static void		check_link(GLuint program, bool fatal)
{
	GLint	is_linked;
	GLint	max_length;
	char	buff[0xF000];

	is_linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&is_linked);
	if (is_linked == GL_FALSE)
	{
		max_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
		glGetProgramInfoLog(program, max_length, &max_length, buff);
		printf("%s\n", buff);
		glDeleteProgram(program);
		if (fatal)
		{
			printf("link error, exiting\n");
			exit(-1);
		}
	}
}

static char		*get_file_source(int fd)
{
	struct stat		st;
	int				ret;
	static char		buff[0xF0000];
	static char		*loadedFiles[0xF00];

	if (fstat(fd, &st) == -1 || st.st_size == 0)
		return (NULL);
	if ((ret = read(fd, buff, sizeof(buff))) == -1)
	{
		perror("read");
		return (NULL);
	}
	if (loadedFiles[fd] != NULL && ret == 0)
		return loadedFiles[fd];
	else if (ret != 0)
		free(loadedFiles[fd]);
	buff[ret] = 0;
	loadedFiles[fd] = strdup(buff);
	return (loadedFiles[fd]);
}

static char		**get_shader_fragment_sources(t_file *files, size_t num, bool fatal)
{
	char	**ret;
	size_t	i;

	ret = (char **)malloc(sizeof(char *) * (num + 2));
	ret[0] = (char *)fragment_shader_text;
	i = 0;
	while (i++ < num)
		if (!(ret[i] = get_file_source(files[i - 1].fd)))
			if (fatal)
				ft_printf("failed to stat file/empty file: %s\n", files[i - 1].path), exit(-1);
			else
				return (NULL);
		else if (fatal)
			ft_printf("loaded shader file: %s\n", files[i - 1].path);
	ret[i] = NULL;
	return (ret);
}

static GLuint		compile_shader_fragments(t_file *files, size_t num, bool fatal)
{
	GLuint		ret;
	char		**srcs;

	if (!(srcs = get_shader_fragment_sources(files, num, fatal)) || srcs[1] == NULL)
		return (0);
	ret = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ret, num + 1, (const char * const *)srcs, NULL);
	glCompileShader(ret);
	check_compilation(ret, fatal);
	return (ret);
}

static GLuint		compile_shader_vertex(const char *src, bool fatal)
{
	GLuint		ret;

	ret = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ret, 1, &src, NULL);
	glCompileShader(ret);
	check_compilation(ret, fatal);
	return (ret);
}

GLuint				create_program(t_file *files, size_t num, bool fatal)
{
	GLuint				program;
	const GLuint		shader_vertex = compile_shader_vertex(vertex_shader_text, fatal);
	const GLuint		shader_fragment = compile_shader_fragments(files, num, fatal);

	if (shader_vertex == 0 || shader_fragment == 0)
		return (0);
	program = glCreateProgram();
	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	glLinkProgram(program);
	check_link(program, fatal);
	return (program);
}

GLuint				create_font_program()
{
	GLuint			program;
	const GLuint	shader_vertex = compile_shader_vertex(vertex_shader_font, true);
	GLuint			shader_fragment;
	GLuint			ret;

	ret = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ret, 1, &fragment_shader_font, NULL);
	glCompileShader(ret);
	check_compilation(ret, true);
	shader_fragment = ret;
	if (shader_vertex == 0 || shader_fragment == 0)
		ft_printf("can't compile font shaders\n"), exit(-1);
	program = glCreateProgram();
	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	glLinkProgram(program);
	check_link(program, true);
	return (program);
}
