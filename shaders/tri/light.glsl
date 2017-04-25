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

	if (cosi < 0)
		cosi = -cosi;
	else
	{
		float tmp = etat;
		etat = etai;
		etai = tmp;
	}
	float	eta = etai / etat;
	float	k = 1 - eta * eta * (1 - cosi * cosi);
	if (k < 0)
		return I;
	else
		return (eta * I + (eta * cosi - sqrt(k)) * N);
}

float			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;

	shadow.dir = d;
	shadow.pos = pos;
	shad = scene(shadow);

	if (shad.dist < h.dist + EPSI){
		return ((1 - atlas_fetch(shad.mat.texture, shad.uv).w));
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
		return (ambient);
	color = (limit(dot(h.norm, d), 0.0, 1.0)) * col;
	return (color * shadows(h.pos, d, h) + ambient);
}

/*Definition de la transparence*/
vec3    fopacity(vec3 pos, float opacity, Ray r, Hit h){
  vec3  color = vec3(0);
  Ray   trans;
  float   on_off = 1;
	Hit h2 = h;

	int i = -1;
	while (++i < 3)
	{
		h = h2;
		trans.pos = h.pos;
	 	trans.dir = refraction(r.dir, h.norm, atlas_fetch(h.mat.refraction, h.uv).x);
	 	h2 = scene(trans);
		opacity = atlas_fetch(h.mat.texture, h.uv).w;
		on_off = on_off * (1 - opacity);
		color += light(pos, trans, h2) * on_off;
	}
   return color;
}



/*Definition de la reflection*/
vec3    freflection(vec3 pos, float reflection, Ray r, Hit h){
  Hit   h2 = h;
  float   on_off = 1;
  int   i= -1;
  vec3  color = vec3(0);
  Ray   ref;
	float opacity = 1;

	while (++i < 3 && on_off > 0)
	{
			h = h2;
		  ref.dir = h.norm;
			ref.pos = h.pos;
			h2 = scene(ref);
			reflection = atlas_fetch(h.mat.reflection, h.uv).x;
			on_off = on_off * reflection;
			opacity = atlas_fetch(h2.mat.texture, h2.uv).w;
			color += (fopacity(pos, opacity, ref, h2) + light(pos, ref, h2)) * on_off;
	}
  return (color);
}

vec3		fdeg(vec3 pos, float reflection, float opacity, Ray r, Hit h)
{
	Ray   ref;
	Ray   trans;
	Hit hre = h;
	Hit htr = h;
	Hit	hr;
	Hit ht;
	float   on_off_re = 1;
	float		on_off_tr = 1;
	vec3		color = vec3(0);
	int			i = -1;

	while((on_off_re > 0 || on_off_tr > 0) && ++i < 2)
	{
		reflection = atlas_fetch(hre.mat.reflection, hre.uv).x;
		on_off_re = on_off_re * reflection;
		if (on_off_re > 0)
		{
			hr = hre;
			ref.dir = hr.norm;
			ref.pos = hr.pos;
			hre = scene(ref);
			opacity = atlas_fetch(hre.mat.texture, hre.uv).w;
			color += (fopacity(pos, opacity, ref, hre) + light(pos, ref, hre)) * on_off_re;
		}
		opacity = atlas_fetch(htr.mat.texture, htr.uv).w;
		on_off_tr = on_off_tr * (1 - opacity);
		if (on_off_tr > 0)
		{
			ht = htr;
			trans.pos = ht.pos;
			trans.dir = refraction(r.dir, ht.norm, atlas_fetch(ht.mat.refraction, ht.uv).x);
			htr = scene(trans);
			reflection = atlas_fetch(htr.mat.reflection, htr.uv).x;
			color += (freflection(pos, reflection, trans, htr) + light(pos, trans, htr)) * on_off_tr;
		}
	}
	return (color);
}

/* Définition des effets de lumiere */
vec3		calc_light(vec3 pos, Ray r, Hit h)
{
  float reflection = atlas_fetch(h.mat.reflection, h.uv).x;
	float	opacity = atlas_fetch(h.mat.texture, h.uv).w;
	float	refrac = atlas_fetch(h.mat.refraction, h.uv).x;
  vec3 bump = atlas_fetch(h.mat.bump, h.uv).xyz;

  vec3  ref = vec3(0);
  vec3  tra = vec3(0);
	vec3  deg = vec3(0);

	if (bump != vec3(0))
		h.norm  = normalize(bump + h.norm);
  vec3 lambert = light(pos, r, h) * opacity;
  // if (reflection > 0)
  //   ref = freflection(pos, reflection, r, h);
  // if (opacity < 1)
  //   tra = fopacity(pos, opacity, r, h);
	if (reflection > 0 || opacity < 1)
		deg += fdeg(pos, reflection, opacity, r, h);
	return (lambert + deg);
}
