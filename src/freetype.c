/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/02/13 17:43:15 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <dirent.h>
#include <stdbool.h>
#include <math.h>
#include <OpenGL/glext.h>

#define FILE_CHECK_EXT(x, y) (ft_strrchr(x, '.') != NULL && !ft_strcmp(ft_strrchr(x, '.') + 1, y))
#define MAXWIDTH 1024
#define MAX(x, y) (x > y) ? (x) : (y)

static FT_Library	g_library;
static GLuint		g_vbo;
static GLint		g_color_uniform;
static GLint		g_coord_attribute;
static GLint		g_font_program;
static GLint		g_tex_uniform;
/*
const static float points[] = {
	-1.0f,  -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f,
	-1.0f,  -1.0f,
};*/

typedef struct	s_point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
}				point;

typedef struct s_atlas
{
	GLuint			tex;

	unsigned int	w;
	unsigned int	h;

	struct {
		//advance
		float	ax;
		float	ay;

		//bitmap
		float	bh;
		float	bw;

		//bitmap_left/top
		float	bl;
		float	bt;

		//texture coords
		float	tx;
		float	ty;

	} c[128];
}				t_atlas;

static char	*foreach_font_file(void)
{
	struct dirent			*d;
	static DIR				*dir;
	static bool				init = false;
	static char				path[1024];

	if (!init && (init = true))
		dir = opendir("./fonts/");
	if (dir == NULL)
		return (NULL);
	while ((d = readdir(dir)))
	{
		if (FILE_CHECK_EXT(d->d_name, "ttf"))
		{
			ft_sprintf(path, "./fonts/%s", d->d_name);
			return (path);
		}
	}
	return (NULL);
}

typedef struct	s_vec2i
{
	int		x;
	int		y;
}				t_vec2i;

typedef struct	s_font
{
	GLuint	textureID;
	t_vec2i	size;
	t_vec2i	bearing;
	GLuint	advance;
}				t_font;

/*static t_font	*get_font_data(const int index)
{
	static t_font		fonts[0xF0][128];

	if (index >= 0 && index < 0xF0)
		return (fonts[index]);
	return (NULL);
}*/

static t_atlas	atlas;

void		load_fonts()
{
	int		error;
	FT_Face	face;
	int		index;


	g_font_program = create_font_program();

	g_coord_attribute = glGetAttribLocation(g_font_program, "coord");
	g_tex_uniform = glGetUniformLocation(g_font_program, "tex");
	g_color_uniform = glGetUniformLocation(g_font_program, "color");

	if (g_coord_attribute == -1 || g_tex_uniform == -1 || g_color_uniform == -1)
		puts("shader internal error"), exit(-1);

	glGenBuffers(1, &g_vbo);

	//fonts loading
	index = 0;
	error = FT_Init_FreeType(&g_library);
	if (error)
		ft_printf("can't initialize freetype\n"), exit(-1);
/*	while ((f = foreach_font_file())) //currently just load one font
	{
		if ((font = get_font_data(index++)) == NULL)
		{
			ft_printf("font limit reached, no more fonts will be loaded\n");
			break ;
		}
		if ((error = FT_New_Face(g_library, f, 0, &face)))
			ft_printf("error while loading font file: %f\n", f);
		FT_Set_Pixel_Sizes(face, 0, 24);
		ft_printf("%s font loaded\n", f);
	}*/

	const char *file = foreach_font_file();
	if (FT_New_Face(g_library, file, 0, &face))
		fprintf(stderr, "error while loading font file: [%s]\n", file), exit(-1);

	FT_GlyphSlot g = face->glyph;

	FT_UInt  glyph_index;

	unsigned int		rowh = 0;
	unsigned int		roww = 0;

	for (int i = 32; i < 128; i++) {
		glyph_index = FT_Get_Char_Index(face, i);
		error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
		if (error)
			continue;
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
		if (error)
			continue;
		if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
			atlas.w = MAX(atlas.w, roww);
			atlas.h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = MAX(rowh, g->bitmap.rows);
	}

	atlas.w = MAX(atlas.w, roww);
	atlas.h += rowh;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &atlas.tex);
	glBindTexture(GL_TEXTURE_2D, atlas.tex);
	glUniform1i(atlas.tex, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlas.w, atlas.h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int		ox = 0;
	int		oy = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++)
	{
		glyph_index = FT_Get_Char_Index( face, i );
		error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
		if (error)
			continue;
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
		if (error)
			continue;
		
		if (ox + g->bitmap.width + 1 > MAXWIDTH) {
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		atlas.c[i].ax = g->advance.x >> 6;
		atlas.c[i].ay = g->advance.y >> 6;

		atlas.c[i].bw = g->bitmap.width;
		atlas.c[i].bh = g->bitmap.rows;

		atlas.c[i].bl = g->bitmap_left;
		atlas.c[i].bt = g->bitmap_top;

		atlas.c[i].tx = ox / (float)atlas.w;
		atlas.c[i].bt = oy / (float)atlas.h;

		rowh = MAX(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}
}

void	deleteFonts(void)
{
	glDeleteTextures(1, &atlas.tex);
}

void	draw_text(const char *text, float x, float y)
{
	const uint8_t *p;

	/* Use the texture containing the atlas */
	glBindTexture(GL_TEXTURE_2D, atlas.tex);
	glUniform1i(g_tex_uniform, 0);

	glUniform4f(g_color_uniform, 0, 0, 0, 1);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(g_coord_attribute);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glVertexAttribPointer(g_coord_attribute, 4, GL_FLOAT, GL_FALSE, 0, 0);

	float sx = 2.f / window.x;
	float sy = 2.f / window.y;

	point coords[6 * strlen(text)];
	int c = 0;

	/* Loop through all c */
	for (p = (const uint8_t *)text; *p; p++) {
    	/* Calculate the vertex and texture coordinates */
    	float x2 = x + atlas.c[*p].bl * sx;
    	float y2 = -y - atlas.c[*p].bt * sy;
    	float w = atlas.c[*p].bw * sx;
    	float h = atlas.c[*p].bh * sy;

    	/* Advance the cursor to the start of the next character */
    	x += atlas.c[*p].ax * sx;
    	y += atlas.c[*p].ay * sy;

    	/* Skip glyphs that have no pixels */
    	if (!w || !h)
        	continue;

    	coords[c++] = (point) {
    		x2, -y2, atlas.c[*p].tx, atlas.c[*p].ty};
    	coords[c++] = (point) {
    		x2 + w, -y2, atlas.c[*p].tx + atlas.c[*p].bw / atlas.w, atlas.c[*p].ty};
    	coords[c++] = (point) {
    		x2, -y2 - h, atlas.c[*p].tx, atlas.c[*p].ty + atlas.c[*p].bh / atlas.h};
    	coords[c++] = (point) {
    		x2 + w, -y2, atlas.c[*p].tx + atlas.c[*p].bw / atlas.w, atlas.c[*p].ty};
    	coords[c++] = (point) {
    		x2, -y2 - h, atlas.c[*p].tx, atlas.c[*p].ty + atlas.c[*p].bh / atlas.h};
    	coords[c++] = (point) {
    		x2 + w, -y2 - h, atlas.c[*p].tx + atlas.c[*p].bw / atlas.w, atlas.c[*p].ty + atlas.c[*p].bh / atlas.h};
	}

	/* Draw all the character on the screen in one go */
	glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, c);

	glDisableVertexAttribArray(g_coord_attribute);
}
