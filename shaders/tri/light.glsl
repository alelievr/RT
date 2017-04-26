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

	shadow.dir = d;
	shadow.pos = pos + d * EPSI;
	shad = scene(shadow);

	if (shad.dist < h.dist){
		return ((1 - atlas_fetch(shad.mat.texture, shad.uv).w));
	}
	return (1);
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
  Ray cam = r;
  float   reflection;
  float   opacity;
  int     i = -1;
  float   limit = 1;
  vec3    color_hit = vec3(0);
  vec3    color = vec3(0);

  while (1 == 1)
  {
    h = scene(r);
    reflection = atlas_fetch(h.mat.reflection, h.uv).x;
    opacity = atlas_fetch(h.mat.texture, h.uv).w;

    if (h.dist < 1e20){
        color_hit = atlas_fetch(h.mat.texture, h.uv).xyz;

        color += light(pos_light, light_color, r, h) * limit;
        color += iAmbient * color_hit;

        if (opacity < 1)
        {
            r.pos = h.pos + r.dir * EPSI;
            r.dir = refraction(cam.dir, h.norm, atlas_fetch(h.mat.refraction, h.uv).w);

						light_color = light_color * opacity + color_hit * 1 - opacity;
            limit *= 1 - opacity;
            if (limit == 0)
                return (color);
        }
        else if (reflection > 0)
        {
            r.pos = h.pos;
            r.dir = r.dir - 2.0 * dot(h.norm, r.dir) * h.norm;

            limit *= reflection;
            if (limit == 0)
                return (color);
        }
        else
            return (color);
      }
      else
          return(color);
    }
}
