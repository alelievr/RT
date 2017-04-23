void sphere (vec3 pos, float data, Coupes coupe, Material mat, Ray r, inout Hit h){
	vec3 d = r.pos - pos;

	float a = dot (r.dir, r.dir);
	float b = dot (r.dir, d);
	float c = dot (d, d) - data * data;

	float g = b*b - a*c;

	if (g < EPSI)
		return;

	float t = (-sqrt (g) - b) / a;
	float t1 = (sqrt (g) - b) / a;

	if (t < 0 && t1 < 0)
		return;

	vec3 inter = r.pos + r.dir * t;
	vec3 inter1 = r.pos + r.dir * t1;

	if (t > EPSI && t < h.dist && !decoupe(pos, inter, coupe)) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		h.norm = h.pos - pos;
		h.mat = mat;
		vec3 n = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		return;
	}

<<<<<<< HEAD
	vec3 inter1 = r.pos + r.dir * t1;

	if ((t < EPSI || decoupe(pos, inter, coupe)) && !decoupe(pos, inter1, coupe)){
		if (t1 > EPSI && t1 < h.dist){
			h.dist = t1;
=======
	if ((t < 0  || decoupe(pos, inter, coupe)) && !decoupe(pos, inter1, coupe)){
		if (t1 > 0 && t1 < h.dist){
			h.dist = t1 + EPSI;
>>>>>>> 4682732237ceb1c95fb7dd7ed8f550e6ff3e79e6
			h.pos = r.pos + r.dir * h.dist;
			h.norm = -1 * (h.pos - pos);
			h.mat = mat;
			vec3 n = normalize(h.pos - pos);
			h.uv = vec2(-(0.5 + (atan(n.z, n.x)) / (M_PI)), (0.5 - asin(n.y)) / M_PI);
		}
		return;
	}
}
