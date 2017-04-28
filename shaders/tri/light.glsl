float		limit(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

vec3      refraction(vec3 dir, vec3 norm, float r)
{
  float k = 1.0 - r * r * (1.0 - dot(norm, dir) * dot(norm, dir));
  vec3 ref;

  if (k < 0.0)
      ref = vec3(0);
  else
      ref = r * dir - (r * dot(norm, dir) + sqrt(k)) * norm;
  return (ref);
}

float			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;
	float t;
	float ret = 1;

	shadow.dir = d;
	shadow.pos = pos + d * EPSI;
	shad = scene(shadow);
	t = shad.dist;
	while (t < h.dist + EPSI)
	{
		shadow.pos = shad.pos + d * EPSI;
		ret *= (1 - atlas_fetch(shad.mat.texture, shad.uv).w);
		shad = scene(shadow);
		t += shad.dist;
		if (ret == 0)
			return (0);
	}
	return (ret);
}

/* Définition de l'effet de la lumière sur les objets présents */
vec3		light(vec3 pos, vec3 light_color, Ray r, Hit h)
{
	vec3 v1 = pos - h.pos;
	vec3 v3 = v1 * v1;
	vec3 d = normalize(v1);
  float coef = 0;
  vec3 color = vec3(0);
  vec3 col = atlas_fetch(h.mat.texture, h.uv).xyz;
  vec3 bump = atlas_fetch(h.mat.bump, h.uv).xyz;
  vec3 spec = vec3(1);

	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20)
		return (color);
	if (!h.inside)
		h.norm = normalize(normalize(h.norm) + bump);
  coef = (limit(dot(h.norm, d), 0.0, 1.0));
	color = coef * col;
  if (coef > 0)
  {
    vec3 refdir = d - 2.0 * dot(h.norm, d) * h.norm;
    coef = max(dot(r.dir, refdir), 0);
    spec *= pow(coef, 30) * atlas_fetch(h.mat.specular, h.uv).x;
  }
  //return (((color + spec) * shadows(h.pos, d, h)));
	return (color * shadows(h.pos, d, h));
}

vec3	 calc_color(Ray r, vec3 pos_light, vec3 light_color)
{
  Hit h;
  float   reflection;
  float   opacity;
  int     i = -1;
  float   limit = 1;
  vec3    color_hit = vec3(0);
  vec3    color = vec3(0);
	float refrac;

  while (++i < 10)
  {
		h.dist = 0;
    h = scene(r);

    reflection = atlas_fetch(h.mat.reflection, h.uv).x;
    opacity = atlas_fetch(h.mat.texture, h.uv).w;

    color_hit = atlas_fetch(h.mat.texture, h.uv).xyz;

    color += light(pos_light, light_color, r, h) * limit;
    color += iAmbient * color_hit * limit;
		color *= opacity;

    if (opacity < 1)
    {
				refrac = atlas_fetch(h.mat.refraction, h.uv).x * 10;
				if (refrac > 1)
					r.dir = refraction(r.dir, h.norm , (1 / refrac));
				r.pos = h.pos + r.dir * EPSI;
				limit *= (1 - opacity);
				// if (limit < 0.1)
				// 	return color;
		}
    else if (reflection > 0)
    {
        r.dir = r.dir - 2.0 * dot(normalize(h.norm), r.dir) * normalize(h.norm);
				r.pos = h.pos + r.dir * EPSI;
				limit *= reflection;
				// if (limit < 0.1)
				// 	return color;
    }
    else
        return (color);
  }
}
