float		limit(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

/* Fonction de calcul du vecteur de refraction */
vec3		refraction(vec3 I, vec3 N, float n2)
{
	float	cosi = clamp(-1, 1, dot(I, N));
	float	etai = 1;
	float	etat = n2;
	vec3	n = N;
	if (cosi < 0)
		cosi = -cosi;
	else
	{
		float tmp = etat;
		etat = etai;
		etai = tmp;
		n = N;
	}
		float	eta = etai / etat;
		float	k = 1 - eta * eta * (1 - cosi * cosi);
		if (k < 0)
			return vec3(0);
		else
			return (eta * I + (eta * cosi - sqrt(k)) * n);
}

float			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;

	shadow.dir = d;
	shadow.pos = pos;
	shad = scene(shadow);

	if (shad.dist < h.dist){
		return (atlas_fetch(shad.mat.transparency, shad.uv).x);
	}
	return (1);
}

/* Définition de l'effet de la lumière sur les objets présents */
vec3		light(vec3 pos, Ray r, Hit h)
{
	vec3 v1 = pos - h.pos;
	vec3 v3 = v1 * v1;
	vec3 d = normalize(v1);

  vec3 col = atlas_fetch(h.mat.texture, h.uv).xyz;
	vec3 color = vec3(0);
	vec3 ambient = vec3(iAmbient * col);
	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20)
		return (color);
	color = (limit(dot(h.norm, d), 0.0, 1.0)) * col;
	return (color * shadows(h.pos, d, h) + ambient);
}

/*Definition de la reflection*/
vec3    freflection(vec3 pos, float reflection, Ray r, Hit h){
  Hit   h2 = h;
  float   on_off = 1;
  int   i= -1;
  vec3  color = vec3(0);
  Ray   ref;

	while (++i < 3)
	{
			h = h2;
		  ref.dir = h.norm;
			ref.pos = h.pos;
			h2 = scene(ref);
			reflection = atlas_fetch(h.mat.reflection, h.uv).x;
			on_off = on_off * reflection;
			color += light(pos, ref, h2) * on_off;
	}
  return (color);
}

/*Definition de la transparence*/
vec3    ftransparency(vec3 pos, float transparency, float refrac, Ray r, Hit h){
  vec3  color = vec3(0);
  Ray   trans;

	 trans.pos = h.pos;
	 trans.dir = refraction(r.dir, h.norm, refrac);
	 h = scene(trans);
	 color = light(pos, trans, h) * transparency;
   return color;
}

/* Définition des effets de lumiere */
vec3		calc_light(vec3 pos, Ray r, Hit h)
{
  float reflection = atlas_fetch(h.mat.reflection, h.uv).x;
	float	transparency = atlas_fetch(h.mat.transparency, h.uv).x;
	float	refrac = atlas_fetch(h.mat.refraction, h.uv).x;
  vec3 bump = atlas_fetch(h.mat.bump, h.uv).xyz;

  vec3  ref = vec3(0);
  vec3  tra = vec3(0);
	if (bump != vec3(0))
	 h.norm = normalize(bump + h.norm);
  vec3 lambert = light(pos, r, h) * (1 - transparency);
  if (reflection > EPSI)
    ref = freflection(pos, reflection, r, h);
  if (transparency > EPSI)
    tra = ftransparency(pos, transparency, refrac, r, h);
	return (lambert + tra + ref);
}
