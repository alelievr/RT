#define M_PI 3.1415926535897932384626433832795
#define EPSI 0.001f
#define AMBIENT 0.2

struct	Coupe_S
{
	float	pos;
	vec3	rot;
};

bool decoupe(vec3 centre, vec3 inter, Coupe_S c, Coupe_S c2)
{
	vec3 pos = centre + (c.pos * normalize(c.rot));
	vec3 pos2 = centre + (c2.pos * normalize(c2.rot));

	float d = (c.rot.x * pos.x + c.rot.y * pos.y + c.rot.z * pos.z) * -1;

	if (c.rot.x * inter.x + c.rot.y * inter.y + c.rot.z * inter.z + d > 0)
		return(true);

	return(false);
}

/* Intersection rayon / plan */

/*void plane(vec3 norm, vec3 pos, vec3 color, vec4 data, Ray r, inout Hit h)
{
	float t = (dot(norm,pos) - (dot (norm, r.pos))) / dot (norm, r.dir);
	if (t < EPSI)
		return;
	if (t < h.dist) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		h.color = color;
		h.norm = (faceforward (norm, norm, r.dir));
		h.data = data;
	}
}*/

/*
vec4 atlas_fetch(vec4, vec2 obj_uv) {
    vec2 uv = //ODO
    return texture(altlas_sampler, uv);
}
*/

/* Intersection rayon / plan limité (obsolete on le fera en mesh) */

void plane (vec3 norm, vec3 pos, vec3 rot, float data, Material mat, Ray r, inout Hit h) {
	float t = (dot(norm,pos) - (dot (norm, r.pos))) / dot (norm, r.dir);
	Hit tmp = h;

	if (t < EPSI)
		return;

	if (t < h.dist) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = (faceforward (norm, norm, r.dir));
    h.mat = mat;
  }
	if (data == 0)
		return;
	else if(h.pos.x > pos.x + data/2 || h.pos.x < pos.x - data/2  || h.pos.y > pos.y + data/2 || h.pos.y < pos.y - data/2 || h.pos.z > pos.z + data/2 || h.pos.z < pos.z - data/2)
		h = tmp;
}

/* Intersection rayon / sphère */

void sphere (vec3 pos, float data, Material mat, Ray r, inout Hit h) {
	vec3 d = r.pos - pos;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	//Coupe_S coupe;
	//coupe.pos = 0.5;
	//coupe.rot = vec3(0,1,0);

	//Coupe_S coupe2;
	//coupe2.pos = 1.2;
	//coupe2.rot = vec3(0,0,1);

	if (t < 0)//|| decoupe(pos, h.pos, coupe, coupe2))
		return;

	if (t < h.dist) {
		h.dist = t + EPSI;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = (h.pos - pos);
		h.mat = mat;
	}
	return;
}

/* intersection rayon / cylindre */

void cyl (vec3 v, vec3 dir, float data, Material mat, Ray r, inout Hit h) {
	vec3 d = r.pos - v;

	dir = normalize(dir);
	float a = dot(r.dir,r.dir) - pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - dot(r.dir, dir) * dot(d, dir));
	float c = dot(d, d) - pow(dot(d, dir), 2) - data * data;
	float g = b*b - 4*a*c;

	if (g < 0)
		return;

	float t1 = (-sqrt(g) - b) / (2*a);
	//float t2 = (sqrt(g) - b) / (2*a);

	if (t1 < 0)
		return ;

	if (t1 < h.dist){
		h.dist = t1 - EPSI;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = dir * (dot(r.dir, dir) * h.dist + dot(r.pos - v, dir));
		vec3 tmp = h.pos - v;
		h.norm = tmp - temp;
		h.mat = mat;
	}
	/*else if (t2 >= 0 && t2 < h.dist){
	  h.dist = t2;
	  h.pos = r.pos + r.dir * t2;
	  vec3 temp = rot * (dot(r.dir, rot) * h.dist + dot(r.pos - v, rot));
	  vec3 tmp = h.pos - v;
	  h.color = color;
	  h.norm = temp - tmp;
	  }*/
}

/* Fonction du calcul de l'intersection entre un rayon et un cone */
void cone(vec3 v, vec3 dir, float data, Material mat, Ray r, inout Hit h)
{
	vec3 d = r.pos - v;

	dir = normalize(dir);
	float a = dot(r.dir, r.dir) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - (1 + pow(tan(data * M_PI / 180), 2)) * dot(r.dir, dir) * dot(d , dir));
	float c = dot(d, d) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(d, dir), 2);

	float g = b*b - 4*a*c;
	if (g <= 0)
		return ;
	float t1 = (-sqrt(g) - b) / (2*a);
	if (t1 < EPSI)
		return ;

	if (t1 < h.dist){
		h.dist = t1 ;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - v, dir))) * (1 + pow(tan(data * M_PI / 180), 2));
		vec3 tmp = h.pos - v;
		h.norm = tmp - temp;
		h.mat = mat;
	}
	/*else if (t2 > 0){
	  h.dist = t2;
	  h.pos = r.pos + r.dir * h.dist;
	  vec3 temp = (rot * (dot(r.dir, rot) * h.dist + dot(r.pos - v, rot))) * (1 + pow(tan(f), 2));
	  vec3 tmp = h.pos - v;
	  h.color = color;
	  h.norm = temp - tmp;
	  }*/
}

/* Fonction du calcul de l'intersection entre un rayon et un cube (manque la rotation) */
void cube(vec3 pos, vec3 rot, float data, Material mat, Ray r, inout Hit hit)
{
	r.dir -= r.pos;
	r.pos += rot;
	r.dir += r.pos;
	plane(vec3(0, 0, 1),vec3(pos.x,pos.y,pos.z - data/2),rot, data, mat, r, hit);
	plane(vec3(0, 0, 1),vec3(pos.x,pos.y,pos.z + data/2),rot, data, mat, r, hit);
	plane(vec3(0, 1, 0),vec3(pos.x,pos.y - data/2,pos.z),rot, data, mat, r, hit);
	plane(vec3(0, 1, 0),vec3(pos.x,pos.y + data/2,pos.z),rot, data, mat, r, hit);
	plane(vec3(1, 0, 0),vec3(pos.x - data/2,pos.y,pos.z),rot, data, mat, r, hit);
	plane(vec3(1, 0, 0),vec3(pos.x + data/2,pos.y,pos.z),rot, data, mat, r, hit);
}

/*Hit		scene(Ray r)
{
	int i = -1;
	Hit		hit;
	hit.dist = 1e20;
	hit.mat.texture = vec4(0,0,0,0);
  //size + reflexion + ? + ?
	sphere(vec3(15, 5, -10), 5, Material(vec4(1,0,1,1), vec4(0,0,0,0), vec4(0,0,0,0), vec4(1,1,1,1), vec4(0,0,0,0), vec4(0,0,0,0), vec4(0,0,0,0)), r, hit );
	sphere(vec3(8, 9, -30), 5, Material(vec4(0.8,0.8,0.5,1), vec4(0,0,0,0), vec4(0,0,0,0), vec4(0,1,1,1), vec4(0,0,0,0), vec4(0,0,0,0), vec4(0,0,0,0)), r, hit );
	//sphere(vec3(8, 9, -30), vec3(0,0,1), vec4(5,1,1,0), r, hit);
	//sphere(vec3(15, 15, -45), vec3(0.8,0.5,0), vec4(5,1,1,0), r, hit);
	//sphere(vec3(20, 30, -50), vec3(0.75,0.5,0.55), vec4(4,1,1,0), r, hit);
	//cyl(vec3(0, 0, 0), vec3(0,0,1), vec3(0,0,1), vec4(60,0,1,0), r, hit);
	//cyl(vec3(0, 0, 0), vec3(1,0,0), vec3(1,0,0), vec4(1,0,1,0), r, hit);
	//cyl(vec3(0, 0, 0), vec3(0,1,0), vec3(0,1,0), vec4(1,0,1,0), r, hit);
	//cone(vec3(75, 10, 200), vec3(1,1,0), vec3(1,1,0), vec4(30,0,0,0), r, hit);
	plane(vec3(1,1,0),vec3(1,1,-6), vec3(0,0,0), 0.0, Material(vec4(0.5,0.7,0.8,0), vec4(0,0,0,0), vec4(0,0,0,0), vec4(0,1,1,1), vec4(0,0,0,0), vec4(0,0,0,0), vec4(0,0,0,0)), r, hit);
	//plane(vec3(0,1,0),vec3(50,-280,-30),vec3(0,0,0),vec3(1,1,1),vec4(0,0,0,0), r, hit);
	//plane(vec3(1,1,1),vec3(50,-280,-30),vec3(0,0,0),vec3(0.3,0.5,0.6),vec4(0,0,0,0), r, hit);
	//cube(vec3(5, 15, -25), vec3(0,0,0), vec3(0.8,0.5,0.5), vec4(2,1,1,0), r, hit);
	//sphere(vec3(45, 20, 25),vec3(255,255,255),vec4(0.5, 0.6, 0.5, 0.2), r, hit);
	return hit;
}*/

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

  color = vec3(AMBIENT * vec3(1, 1, 1));
	h.dist = sqrt(v3.x + v3.y + v3.z);
	if (h.dist > 1e20)
		return (color);
	if (shadows(h.pos, d, h))
		return (color);
	color += (limit(dot(h.norm, d), 0.0, 1.0)) * vec3(1, 1, 1);
  return (color);
}

/* Définition de la light */
vec3		calc_light(vec3 pos, Ray ref, Hit h)
{
	Hit	h2 = h;
	vec3 lambert;
	vec3 reflect = vec3(0,0,0);
	vec3 ambient = vec3(h.mat.texture.xyz) * AMBIENT;
	int		i = 0;
	float on_off = 1;
	lambert = light(pos, ref, h);
	while (++i < 5)
	{
		h = h2;
		ref.dir = h.norm;
		ref.pos = h.pos;
		h2 = scene(ref);
		on_off = on_off * h.mat.reflection.x;
		reflect += light(pos, ref, h2) * on_off;
	}
	return (lambert + reflect);
}
