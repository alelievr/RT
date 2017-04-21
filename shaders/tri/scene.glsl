#define M_PI 3.1415926535897932384626433832795
#define EPSI 0.001f
#define UV_EPSI 0.01f
#define AMBIENT 0.2

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
    vec2 uv = vec2(coord.xy) + (vec2(coord.zw) - vec2(coord.xy)) * (mod(obj_uv, 1 - (UV_EPSI * 2)) + UV_EPSI);

    return texture(atlas, uv);
}

void disk (vec3 norm, vec3 pos, float data, Material mat, Ray r, inout Hit h) {
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
	vec3 sous = pos - h.pos;
	float dist = sous.x * sous.x + sous.y * sous.y + sous.z * sous.z;

	if(dist > data)
		h = tmp;
}

bool decoupe(vec3 centre, vec3 inter, Coupe c, Coupe c2,float data, Material m, Ray r,inout Hit h)
{
	Hit tmp;
	Hit	tmp2 = h;
	tmp = h;
	vec3 pos = centre +normalize(c.rot) * c.dist;
	vec3 pos2 = centre +  normalize(c2.rot) * c2.dist;

	float d = (c.rot.x * pos.x + c.rot.y * pos.y + c.rot.z * pos.z) * -1;
	float d2 = (c2.rot.x * pos2.x + c2.rot.y * pos2.y + c2.rot.z * pos2.z) * -1;

	bool t1 = (c.rot.x * inter.x + c.rot.y * inter.y + c.rot.z * inter.z + d < 0);
	bool t2 = (c2.rot.x * inter.x + c2.rot.y * inter.y + c2.rot.z * inter.z + d2 < 0);

	return !(t1 && t2);
}
