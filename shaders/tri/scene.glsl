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

bool decoupe(vec3 centre, vec3 rot, vec3 inter, Coupes co)
{
	int i = 0;
	bool res = true;
	vec3 pos;
	float d;

	while (i < co.nsl)
	{
		co.t[i].xyz = rotate(co.t[i].xyz, rot, 1);
		pos = centre + normalize(co.t[i].xyz) * abs(co.t[i].w);
		d = (co.t[i].x * pos.x + co.t[i].y * pos.y + co.t[i].z * pos.z) * -1;

		if (co.t[i].w > 0)
			res = res && (co.t[i].x * inter.x + co.t[i].y * inter.y + co.t[i].z * inter.z + d > 0);
		else
			res = res && (co.t[i].x * inter.x + co.t[i].y * inter.y + co.t[i].z * inter.z + d < 0);
		i++;
	}
	return !res;
}
