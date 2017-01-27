#line 2

struct		Ray
{
	vec3	dir;
	vec3	pos;
};

struct		Hit
{
	float	dist;
	vec3	norm;
	vec3	pos;
};

void plane (vec3 v, float f, Ray r, inout Hit h) {
    float t = (f - dot (v, r.pos)) / dot (v, r.dir);

    if (t < 0.0)
        return;

    if (t < h.dist) {
        h.dist = t;
        h.pos = r.pos + r.dir * t;
        h.norm = /*normalize*/ (faceforward (v, v, r.dir));
    }
}

void sphere (vec3 v, float f, Ray r, inout Hit h) {
    vec3 d = r.pos - v;

    float a = dot (r.dir, r.dir);
    float b = dot (r.dir, d);
    float c = dot (d, d) - f * f;

    float g = b*b - a*c;

    if (g < 0.0)
        return;

    float t = (-sqrt (g) - b) / a;

    if (t < 0.0)
        return;

    if (t < h.dist) {
        h.dist = t;
        h.pos = r.pos + r.dir * t;
        h.norm = (h.pos - v) / f;
    }
}

Hit		scene(Ray r)
{
	Hit		hit;

	hit.dist = 1e20;
	sphere(vec3(0, 0, 0), 1, r, hit);

	return hit;
}

vec3	raycast(vec3 ro, vec3 rd)
{
	vec3		color = vec3(0);
	Ray			r;
	Hit			hit;

	r.dir = rd;
	r.pos = ro;
	hit = scene(r);
	color = vec3(hit.norm);
	return color;
}
