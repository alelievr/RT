void sphere (vec3 pos, float data, Material mat, Ray r, inout Hit h){
	vec3 d = r.pos - pos;

	Coupe co;
	co.rot = vec3(0,0,-1);
	co.dist = 1;

	Coupe co2;
	co2.rot = vec3(0,1,-1);
	co2.dist = 1;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	float t1 = (sqrt (g) - b) / a;

	vec3 inter = r.pos + r.dir * t;

	if (t > 0 && t < h.dist && !decoupe(pos, inter, co, co2, data, mat, r, h)) {
		h.dist = t + EPSI;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = h.pos - pos;
		h.mat = mat;
		vec3 n = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		return;
	}

	vec3 inter1 = r.pos + r.dir * t1;

	if ((t < 0 || decoupe(pos, inter, co, co2, data, mat, r, h)) && !decoupe(pos, inter1, co, co2, data, mat, r, h)){
		if (t1 > 0 && t1 < h.dist){
			h.dist = t1 + EPSI;
			h.pos = r.pos + r.dir * h.dist;
			h.norm = -1 * (h.pos - pos);
			h.mat = mat;
			vec3 n = normalize(h.pos - pos);
			h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		}
		return;
	}
}
