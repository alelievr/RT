/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keywords.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avially <avially@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 00:14:55 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/03 19:03:34 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYWORDS_H
# define KEYWORDS_H

const char * g_restricted_keywords[] = {
	"type:", "pos:", "rot:", "color:", "csg:", "radius:", "scale:", "slice:",
	"mesh:", "filter:", "texture:", "emission color:",
	"highlight color:", "transparency:", "reflection:", "refraction:",
	"bumpmap:", "illum:", "emission color map:", "highlight color map:",
	"refraction map:", "reflection map:", "ambiant color:", NULL
};

#define END -1
#define V(x) {#x, x}
const struct { char *name; int value; } illum_restricted_keywords[] = {
	V(AMBIANT), V(CAST_SHADOW), V(RECEIVE_SHADOW), V(REFLECT), V(END)
};

const struct {char *name; int value; } mask_restricted_keywords[] = {
	V(SEPIA), V(CARTOON), V(ANTIALIASING), V(DEPTH_OF_FIELD), V(BLOOM),
	V(LENS_FLARE), V(ACES_TONEMAPPING), V(VIGNETTING), V(DALTONIZE),
	V(NIGHT_VISION), V(END)
};

const char * type_restricted_keywords[] = {
	"SPHERE", "PLANE", "CYLINDRE", "CONE", "CUBE", "CAMERA", "POINT_LIGHT",
	"SPOT_LIGHT", "DIRECTIONAL_LIGHT", NULL
};

#endif
