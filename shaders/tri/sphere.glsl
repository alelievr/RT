void sphere (vec3 pos, float data, Material mat, Ray r, inout Hit h){
	vec3 d = r.pos - pos;
	/*
	Coupe co;
	co.rot = vec3(0,0,-1);
	co.dist = 1;

	Coupe co2;
	co2.rot = vec3(0,1,-1);
	co2.dist = 1;
 	*/
	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	if (t < 0)
		return;
	/*
	vec3 inter = r.pos + r.dir * t;

	if (decoupe(pos, inter, co, co2, data, mat, r, h))
		return;
	*/
	if (t < h.dist) {
		h.dist = t + EPSI;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = h.pos - pos;
		h.mat = mat;
		vec3 n = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
	}
	return;
}
