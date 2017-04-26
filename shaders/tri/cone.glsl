void cone(vec3 pos, vec3 rot, float data, Coupes coupe, Material mat, Ray r, inout Hit h)
{
	vec3 d = r.pos - pos;
	vec3 dir = rotate(vec3(0,0,1), rot, 1);

	bool dec = false;
	bool dec1 = false;

	float a = dot(r.dir, r.dir) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - (1 + pow(tan(data * M_PI / 180), 2)) * dot(r.dir, dir) * dot(d , dir));
	float c = dot(d, d) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(d, dir), 2);

	float g = b*b - 4*a*c;

	if (g <= 0)
		return;

	float t = (-sqrt(g) - b) / (2*a);
	float t1 = (sqrt(g) - b) / (2*a);

	vec3 inter = r.pos + r.dir * t;
	vec3 inter1 = r.pos + r.dir * t1;

	if (coupe.nsl > 0)
	{
		dec = decoupe(pos, rot, inter, coupe);
		dec1 = decoupe(pos, rot, inter1, coupe);
	}

	if (t > EPSI && t < h.dist && !dec) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - pos, dir))) * (1 + pow(tan(data * M_PI / 180), 2));
		vec3 tmp = h.pos - pos;
		h.norm = tmp - temp;
		h.mat = mat;
		vec3	d = normalize(h.pos - pos);
		h.uv = vec2(-1 * (0.5 + (atan(h.norm.z, h.norm.x) / (M_PI * 2))), (h.norm.y /  M_PI) - floor(h.norm.y / M_PI));
		h.inside = false;
		return;
	}

	if ((t < EPSI || dec) && !dec1){
		if (t1 > EPSI && t1 < h.dist){
			h.dist = t1 ;
			h.pos = r.pos + r.dir * h.dist;
			vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - pos, dir))) * (1 + pow(tan(data * M_PI / 180), 2));
			vec3 tmp = h.pos - pos;
			h.norm = temp - tmp;
			h.mat = mat;
			vec3	d = normalize(h.pos - pos);
			h.uv = vec2(-1 * (0.5 + (atan(h.norm.z, h.norm.x) / (M_PI * 2))), (h.norm.y /  M_PI) - floor(h.norm.y / M_PI));
			h.inside = true;
		}
		return;
	}
	return;
}
