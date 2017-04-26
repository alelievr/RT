/* Generated uniforms */

uniform vec3 olol_position;
uniform vec3 olol_rotation;
/* Static functions */

struct      Ray
{
    vec3    dir;
    vec3    pos;
};

struct  Coupe
{
    vec3    pos;
    vec3    rot;
};

struct  Material
{
	vec4		texture;
	vec4		emission;
	vec4		transparency;
	vec4		specular;
	vec4		reflection;
	vec4		refraction;
	vec4		bump;
};

struct      Hit
{
   float  		dist;
   vec3    	norm;
   vec3    	pos;
	Material	mat;
	vec2		uv;
};

Hit       scene(Ray r);
vec3    raytrace(vec3 ro, vec3 rd);

#define M_PI 3.1415926535897932384626433832795
#define EPSI 0.001f
#define AMBIENT 0.2

/* rotation d'un point auour des 3 axes */
vec3 rotate(vec3 point, vec3 rot, int t)
{
	rot = rot * M_PI / 180;

	mat3 rotation = mat3(cos(rot.z) * cos(rot.y),-1 * cos(rot.y) * sin(rot.z),sin(rot.y),
						sin(rot.z) * cos(rot.x) + sin(rot.y) * sin(rot.x) * cos(rot.z),-1 * sin(rot.x) * sin(rot.y) * sin(rot.z) + cos(rot.x) * cos(rot.z),-1 * sin(rot.x) * cos(rot.y),
						-1 * sin(rot.y) * cos(rot.z) * cos(rot.x) + sin(rot.x) * sin(rot.z),cos(rot.x) * sin(rot.y) * sin(rot.z) + sin(rot.x) * cos(rot.z), cos(rot.x) * cos(rot.y));
	if (t == 1)
		return (point * rotation);
	return (point * inverse(rotation));

}


vec4 atlas_fetch(vec4 coord, vec2 obj_uv)
{
    vec2 uv = vec2(coord.xy) + (vec2(coord.zw) - vec2(coord.xy)) * obj_uv;
    return texture(iChannel0, uv);
}


/* Intersection rayon / plan limité */
void plane (vec3 norm, vec3 pos, float data, Material mat, Ray r, inout Hit h) {
	norm = normalize(norm);
	float t = (dot(norm,pos) - (dot (norm, r.pos))) / dot (norm, r.dir);
	Hit tmp = h;
	vec3 plus = pos + data/2;
	vec3 moin = pos - data/2;

	if (t < EPSI)
		return;

	if (t < h.dist) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = faceforward(norm, norm, r.dir);
    h.mat = mat;
		vec3	u = vec3(norm.y, norm.z, -1 * norm.x);
		vec3	v = cross(u, norm);
		h.uv = vec2(dot(h.pos, u), dot(h.pos, v));
	  }
	if (data == 0)
		return;
	else if(h.pos.x > plus.x || h.pos.x < moin.x || h.pos.y > plus.y || h.pos.y < moin.y || h.pos.z > plus.z || h.pos.z < moin.z)
		h = tmp;
}

/* Intersection rayon sphère */

void sphere (vec3 pos, float data, Material mat, Ray r, inout Hit h) {
	vec3 d = r.pos - pos;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;

	if (t < 0)
		return;

	if (t < h.dist) {
		h.dist = t + EPSI;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = (h.pos - pos);
		h.mat = mat;
		vec3 n = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
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
		vec3	d = h.pos - (v * r.dir);
		h.uv = vec2(-(0.5 + (atan(d.z, d.x) / (M_PI * 0.25))), -((d.y / M_PI) - floor(d.y / M_PI)));
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
		vec3	d = normalize(h.pos - v);
		h.uv = vec2(-(0.5 + (atan(h.norm.z, h.norm.x) / (M_PI * 2))), (h.norm.y /  M_PI) - floor(h.norm.y / M_PI));
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
	Hit h;
	h = hit;
	r.pos -= pos;
	r.pos = rotate(r.pos, rot, 1);
	r.dir = rotate(r.dir, rot, 1);
	plane(vec3(0, 0, 1),vec3(0,0,data/2), data, mat, r, hit);
	plane(vec3(0, 0, 1),vec3(0,0,-data/2), data, mat, r, hit);
	plane(vec3(0, 1, 0),vec3(0,data/2,0), data, mat, r, hit);
	plane(vec3(0, 1, 0),vec3(0,-data/2,0), data, mat, r, hit);
	plane(vec3(1, 0, 0),vec3(data/2,0,0), data, mat, r, hit);
	plane(vec3(1, 0, 0),vec3(-data/2,0,0), data, mat, r, hit);
	if (h.dist != hit.dist){
		hit.pos = rotate(hit.pos, rot, 0);
		hit.pos += pos;
		hit.norm = rotate(hit.norm, rot, 0);
	}
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
	vec3 ambient = vec3(atlas_fetch(h.mat.texture, h.uv).xyz) * AMBIENT;
	int		i = 0;
	float on_off = 1;
	vec3 lambert = light(pos, r, h);
	while (++i < 5)
	{
	h = h2;
	ref.dir = h.norm;
	ref.pos = h.pos;
	h2 = scene(ref);
	on_off = on_off * atlas_fetch(h.mat.reflection, h.uv).x;
	reflect += light(pos, ref, h2) * on_off;
	}
	return (lambert + reflect);
}

/* Static MainImage */

void        mainImage(vec2 coord)
{
	vec2    uv = (coord / iResolution) * 2 - 1;
	vec3    cameraPos = iMoveAmount.xyz + vec3(0, 0, -2);
	vec3    cameraDir = iForward;
	vec3    col;

	//window ratio correciton:
	uv.x *= iResolution.x / iResolution.y;

	//perspective view
	float   fov = 1.5;
	vec3    forw = normalize(iForward);
	vec3    right = normalize(cross(forw, vec3(0, 1, 0)));
	vec3    up = normalize(cross(right, forw));
	vec3    rd = normalize(uv.x * right + uv.y * up + fov * forw);
	fragColor = vec4(raytrace(cameraPos, rd), 1);
}

Hit     scene(Ray r)
{
	int i = -1;
	Hit     hit;
	hit.dist = 1e20;
	hit.mat.texture = vec4(0,0,0,0);

/* Generated scene from parser */

    return hit;
}

vec3    raytrace(vec3 ro, vec3 rd)
{
    Ray         r;
    Hit         h;
    vec3        color = vec3(0,0,0);
    vec3        ambient;
    r.dir = rd;
    r.pos = ro;
    h = scene(r);

	return (color / 2);
}
