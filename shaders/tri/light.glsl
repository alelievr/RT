float		limit(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

bool			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;

	shadow.dir = d;
	shadow.pos = pos;
	shad = scene(shadow);
	if (shad.dist < h.dist)
		return (true);
	return (false);
}

/* Définition de l'effet de la lumière sur les objets présents */
vec3		light(vec3 pos, Ray r, Hit h)
{
	vec3 v1 = pos - h.pos;
	vec3 v3 = v1 * v1;
	vec3 d = normalize(v1);
	vec3 color;

  color = vec3(AMBIENT * vec3(atlas_fetch(h.mat.texture, h.uv).xyz));
	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20)
		return (color);
	if (shadows(h.pos, d, h))
		return (color);
	color += (limit(dot(h.norm, d), 0.0, 1.0)) * vec3(atlas_fetch(h.mat.texture, h.uv).xyz);
  return (color);
}

/* Définition de la light */
vec3		calc_light(vec3 pos, Ray r, Hit h)
{
	Hit	h2 = h;
	Ray ref;
	vec3 reflect = vec3(0,0,0);
	//vec3 ambient = vec3(atlas_fetch(h.mat.texture, h.uv).xyz) * AMBIENT;
	int		i = 0;
	float on_off = 1;
	vec3 lambert = light(pos, r, h);
	// while (++i < 5)
	// {
	// h = h2;
	// ref.dir = h.norm;
	// ref.pos = h.pos;
	// h2 = scene(ref);
	// on_off = on_off * atlas_fetch(h.mat.reflection, h.uv).x;
	// reflect += light(pos, ref, h2) * on_off;
	// }
	if (atlas_fetch(h.mat.transparency, h.uv).x > EPSI)
	{
		h = h2;
		Ray trans;
		trans.pos = h.pos;
		trans.dir = r.dir;
		h2 = scene(trans);
		lambert = light(pos, trans, h2) + lambert * abs(atlas_fetch(h.mat.transparency, h.uv).x - 1);
	}
	return (lambert + reflect);
}
