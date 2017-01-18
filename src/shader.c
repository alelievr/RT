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
	struct stat	st;
	char		*ret;

	if (fstat(fd, &st) == -1 || st.st_size == 0)
		return (NULL);
	ret = mmap(NULL, st.st_size + 1, PROT_READ | PROT_WRITE,
			MAP_PRIVATE, fd, 0);
	if (ret == MAP_FAILED)
	{
		perror("mmap");
		return (NULL);
	}
	ret[st.st_size] = 0;
	return (ret);
}

static char		**get_shader_fragment_sources(t_file *files, size_t num, bool fatal)
{
	char	**ret;
	size_t	i;

	ret = (char **)malloc(sizeof(char *) * (num + 2));
	ret[0] = (char *)fragment_shader_text;
	i = 0;
	while (i++ < num)
		if (!(ret[i] = get_file_source(files[i - 1].fd)) && fatal)
			ft_printf("failed to stat file/empty file: %s\n", files[i - 1].path), exit(-1);
		else if (fatal)
			ft_printf("loaded shader file: %s\n", files[i - 1].path);
	ret[i] = NULL;
	return (ret);
}

static GLuint		compile_shader_fragments(t_file *files, size_t num, bool fatal)
{
	GLuint		ret;
	char		**srcs;

	srcs = get_shader_fragment_sources(files, num, fatal);
	if (srcs[1] == NULL)
		return (0);
	ret = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ret, num + 1, (const char * const *)srcs, NULL);
	glCompileShader(ret);
	check_compilation(ret, fatal);
	return (ret);
}

static GLuint		compile_shader_vertex(bool fatal)
{
	GLuint		ret;

	ret = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ret, 1, &vertex_shader_text, NULL);
	glCompileShader(ret);
	check_compilation(ret, fatal);
	return (ret);
}

GLuint				create_program(t_file *files, size_t num, bool fatal)
{
	GLuint				program;
	const GLuint		shader_vertex = compile_shader_vertex(fatal);
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
