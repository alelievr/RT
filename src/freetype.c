/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/25 23:36:27 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "ft2build.h"
#include FT_FREETYPE_H

static FT_Library	g_library;

static char	*foreach_font_file(void)
{
	struct dirent			*d;
	static DIR				*dir;
	static bool				init = false;
	static char				path[1024];

	if (!init)
		dir = opendir("./fonts/");
	if (dir == NULL)
		return (NULL);
	if ((d = readdir(dir)))
		return ft_sprintf(path, "./fonts/%s", d->d_name);
	return (NULL);
}

static FT_Face	getFaceData(const int index)
{
	static FT_Face		faces[0xF0];

	if (index > 0 && index < 0xF0)
		return faces[index];
	return (NULL);
}

void		loadFonts(void)
{
	int		error;
	char	*f;
	FT_Face	*face;
	int		index;

	index = 0;
	error = FT_Init_FreeType(&g_library);
	if (error)
		ft_printf("can't initialize freetype\n"), exit(-1);
	while ((f = foreach_font_file()))
	{
		if (!(face = getFaceData(index++)))
		{
			ft_printf("font limit reached, no more fonts will be loaded\n");
			break ;
		}
		if ((error = FT_New_Face(g_library, f, 0, &face)))
			ft_printf("error while loading font file: %f\n", f);
		FT_Set_Char_Size(face, 0, 48 * 64, g_window.x, g_window.y);
		FT_Set_Pixel_Sizes(face, 0, 48);
	}
}

static static void	my_draw_bitmap(FT_Bitmap *b, int _x, int _y, FT_GlyphSlot _slot, int _size)
{
	GLubyte		*bytes = new GLubyte[b->width * b->rows * 4];

	for(unsigned y = 0; y < b->rows; y++) {
		for(unsigned x = 0; x < b->width; x++) {
			for(int i = 0; i < 4; i++) {
				bytes[(x + y * b->width) * 4 + i] = _slot->bitmap.buffer[x + b->width * y];
			}
		}
	}

	static GLuint		tex = 0;

	if (tex == 0)
		glGenTextures(1, &tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, b->width, b->rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 400, 1000, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0); glVertex2d(_x + -00, _y + -00 + _size);
	glTexCoord2d(1.0,0.0); glVertex2d(_x +  b->width, _y + -00 + _size);
	glTexCoord2d(1.0,1.0); glVertex2d(_x +  b->width, _y +  b->rows + _size);
	glTexCoord2d(0.0,1.0); glVertex2d(_x + -00, _y +  b->rows + _size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	(void)_y;
	(void)_x;
}


void		drawText(int x, int y, const int fontIndex, const char *txt)
{
	FT_Face			face;
	FT_GlyphSlot	slot;
	int				error;

	if (face == NULL)
		ft_printf("font not found at intdex: %i\n", fontIndex), exit(-1);
	slot = face->glyph;
	while (*txt)
	{
		FT_UInt  glyph_index;
		glyph_index = FT_Get_Char_Index(face, *txt);
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue ;
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			continue;

  		my_draw_bitmap(&slot->bitmap,
                x + slot->bitmap_left,
                y - slot->bitmap_top);

		x += slot->advance.x >> 6;
		y += slot->advance.y >> 6;

		txt++;
	}
}
