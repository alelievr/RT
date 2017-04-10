#define M_PI 3.1415926535897932384626433832795
#define EPSI 0.001f
#define AMBIENT 0.2

/* Découpe de sphère */
bool decoupe(vec3 centre, vec3 inter, Coupe c, Coupe c2)
{
	float d = (c.rot.x * c.pos.x + c.rot.y * c.pos.y + c.rot.z * c.pos.z) * -1;
	float d2 = (c2.rot.x * c2.pos.x + c2.rot.y * c2.pos.y + c2.rot.z * c2.pos.z) * -1;

	if (c.rot.x * inter.x + c.rot.y * inter.y + c.rot.z * inter.z + d > 0)
		return(true);
	if (c2.rot.x * inter.x + c2.rot.y * inter.y + c2.rot.z * inter.z + d > 0)
		return(true);
	return(false);
}

/* Intersection rayon / plan */
void plane(vec3 norm, vec3 pos, in Material mat, vec4 data, Ray r, inout Hit h)
{
	float t = (dot(norm,pos) - (dot (norm, r.pos))) / dot (norm, r.dir);
	Hit	tmp = h;
	if (t < EPSI)
		return;

	if (t < h.dist) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		h.color = color;
		h.norm = (faceforward (norm, norm, r.dir));
		h.data = data;
	}
	if (h.data.x != 0 && (h.pos.x > pos.x + data.x/2 || h.pos.x < pos.x - data.x/2  || h.pos.y > pos.y + data.x/2 || h.pos.y < pos.y - data.x/2 || h.pos.z > pos.z + data.x/2 || h.pos.z < pos.z - data.x/2))
		h = tmp;
}

/* Intersection rayon / sphère */
void sphere (vec3 pos, vec3 color, vec4 data, Ray r, inout Hit h) {
	vec3 d = r.pos - pos;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data.x * data.x;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	//	Coupe coupe;
	//	coupe.pos = vec3(1,2,-6);
	//	coupe.rot = vec3(0,1,0);

	//	Coupe coupe2;
	//	coupe2.pos = vec3(1,-2,-6);
	//	coupe2.rot = vec3(0,-1,0);

	if (t < 0) //|| decoupe(pos, h.pos, coupe, coupe2))
		return;

	if (t < h.dist) {
		h.dist = t + EPSI;
		h.pos = r.pos + r.dir * h.dist;
		h.color = color;
		h.norm = (h.pos - pos);
		h.data = data;
	}
	return;
}

/* intersection rayon / cylindre */
void cyl (vec3 v, vec3 dir, vec3 color, vec4 data, Ray r, inout Hit h) {
	vec3 d = r.pos - v;

	dir = normalize(dir);
	float a = dot(r.dir,r.dir) - pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - dot(r.dir, dir) * dot(d, dir));
	float c = dot(d, d) - pow(dot(d, dir), 2) - data.x * data.x;
	float g = b*b - 4*a*c;

	if (g < 0)
		return;
	float t1 = (-sqrt(g) - b) / (2*a);

	if (t1 < 0)
		return ;

	if (t1 < h.dist){
		h.dist = t1 - EPSI;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = dir * (dot(r.dir, dir) * h.dist + dot(r.pos - v, dir));
		vec3 tmp = h.pos - v;
		h.color = color;
		h.norm = tmp - temp;
		h.data = data;
	}
}

/* Fonction du calcul de l'intersection entre un rayon et un cone */
void cone(vec3 v, vec3 dir,vec3 color,vec4 data, Ray r, inout Hit h) {
	vec3 d = r.pos - v;

	dir = normalize(dir);
	float a = dot(r.dir, r.dir) - (1 + pow(tan(data.y), 2)) * pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - (1 + pow(tan(data.y), 2)) * dot(r.dir, dir) * dot(d , dir));
	float c = dot(d, d) - (1 + pow(tan(data.y), 2)) * pow(dot(d, dir), 2);

	float g = b*b - 4*a*c;
	if (g <= 0)
		return ;
	float t1 = (-sqrt(g) - b) / (2*a);
	if (t1 < EPSI)
		return ;

	if (t1 < h.dist){
		h.dist = t1 ;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - v, dir))) * (1 + pow(tan(data.x), 2));
		vec3 tmp = h.pos - v;
		h.color = color;
		h.norm = tmp - temp;
		h.data = data;
	}
}

/* Fonction du calcul de l'intersection entre un rayon et un cube (manque la rotation) */
void cube(vec3 pos, vec3 rot, vec3 color, vec4 data, Ray r, inout Hit hit)
{
	r.dir -= r.pos;
	r.pos += rot;
	r.dir += r.pos;
	plane(vec3(0, 0, 1),vec3(pos.x,pos.y,pos.z - data.x/2),color, data, r, hit);
	plane(vec3(0, 0, 1),vec3(pos.x,pos.y,pos.z + data.x/2),color,data, r, hit);
	plane(vec3(0, 1, 0),vec3(pos.x,pos.y - data.x/2,pos.z),color,data, r, hit);
	plane(vec3(0, 1, 0),vec3(pos.x,pos.y + data.x/2,pos.z),color,data, r, hit);
	plane(vec3(1, 0, 0),vec3(pos.x - data.x/2,pos.y,pos.z),color,data, r, hit);
	plane(vec3(1, 0, 0),vec3(pos.x + data.x/2,pos.y,pos.z),color,data, r, hit);
}

float		limit(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

bool			shadows(vec3 pos, vec3 d, Hit h)
{
	Ray		shadow;
	Hit		shad;

	shadow.dir = d;
	shadow.pos = pos + h.norm * EPSI;
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

  color = vec3(0,0,0);
	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20 || shadows(h.pos, d, h))
		return (color);
	color += (limit(dot(h.norm, d), 0.0, 1.0)) * vec3(1,1,1);
  return (color);
}

/* Définition de la light */
vec3		calc_light(vec3 pos, Ray ref, Hit h)
{
	Hit	h2 = h;
	vec3 lambert;
	vec3 reflect = vec3(0,0,0);
	int		i = 0;
	float on_off = 1;
	lambert = light(pos, ref, h) * h.color;
	while (++i < 5)
	{
		h = h2;
		ref.dir = h.norm;
		ref.pos = h.pos;
		h2 = scene(ref);
		on_off = on_off * h.data.y;
		reflect +=light(pos, ref, h2) * on_off * h2.color;
	}
	return lambert + reflect;
}


/* Création d'un rayon */
vec3	raytrace(vec3 ro, vec3 rd)
{
	Ray			r;
	Hit			h;
	vec3		color = vec3(0,0,0);
	vec3		ambient;
 	r.dir = rd;
	r.pos = ro;
	h = scene(r);
	ambient = h.color * AMBIENT;
	color += calc_light(vec3(15,15,-25), r, h);
	color += calc_light(vec3(100, 75, 0), r, h);
  return (color / 2 + ambient);
}
