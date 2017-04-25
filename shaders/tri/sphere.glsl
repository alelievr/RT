void sphere (vec3 pos, vec3 rot, float data, Coupes coupe, Material mat, Ray r, inout Hit h){
	vec3 d = r.pos - pos;

	bool dec = false;
	bool dec1 = false;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	float t1 = (sqrt (g) - b) / a;

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
		h.norm = normalize((h.pos - pos) + atlas_fetch(h.mat.bump, h.uv).xyz);
		h.mat = mat;
		vec3 n = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		h.norm = normalize((h.pos - pos) + atlas_fetch(h.mat.bump, h.uv).xyz);
		return;
	}

	if ((t < EPSI || dec) && !dec1){
		if (t1 > EPSI && t1 < h.dist){
			h.dist = t1;
			h.pos = r.pos + r.dir * h.dist;
			h.norm = -1 * (h.pos - pos);
			h.mat = mat;
			vec3 n = normalize(h.pos - pos);
			h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		}
		return;
	}
}
