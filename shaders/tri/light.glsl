float		limit(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

float			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;

	shadow.dir = d;
	shadow.pos = pos;
	shad = scene(shadow);

	if (shad.dist < h.dist){
		vec3	col = atlas_fetch(shad.mat.texture, shad.uv).xyz;
		float	t = (col.x + col.y + col.z) / 3;
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
	vec3 color;
	vec3 ambient;
	vec3 col = atlas_fetch(h.mat.texture, h.uv).xyz;

  ambient = vec3(AMBIENT * col);
	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20)
		return (color);
	color = (limit(dot(h.norm, d), 0.0, 1.0)) * col;
	return (color * shadows(h.pos, d, h) + ambient);
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

/* Définition de la light */
vec3		calc_light(vec3 pos, Ray r, Hit h)
{
	vec3 bump = atlas_fetch(h.mat.bump, h.uv).xyz;
	if (bump != vec3(0))
		h.norm = normalize(bump + h.norm);
	Hit	h2 = h;
	Ray ref;
	vec3 reflect = vec3(0,0,0);
	float reflection = atlas_fetch(h.mat.reflection, h.uv).x;
	//vec3 ambient = vec3(atlas_fetch(h.mat.texture, h.uv).xyz) * AMBIENT;
	int		i = -1;
	float on_off = 1;
	vec3 lambert = light(pos, r, h);
	float	transparency = atlas_fetch(h.mat.transparency, h.uv).x;
	float	refrac = atlas_fetch(h.mat.refraction, h.uv).x;
	if (reflection > EPSI)
	{
		while (++i < 3)
		{
			//vec3 ambient = vec3(atlas_fetch(h.mat.texture, h.uv).xyz) * AMBIENT;
				h = h2;
				ref.dir = h.norm;
				ref.pos = h.pos;
				h2 = scene(ref);
				reflection = atlas_fetch(h.mat.reflection, h.uv).x;
				on_off = on_off * reflection;
				reflect += light(pos, ref, h2) * on_off;
			}
	}
	if (transparency > EPSI)
	{
		h = h2;
		Ray trans;
		trans.pos = h.pos;
		trans.dir = refraction(r.dir, h.norm, refrac);//refraction(refraction(r.dir, h.norm, refrac), , refrac);
		h2 = scene(trans);
	 	lambert = light(pos, trans, h2) * transparency + lambert * (1 - transparency);
	 }
	return (lambert + reflect);
}
