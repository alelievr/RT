void cyl (vec3 pos, vec3 rot, float data, Material mat, Ray r, inout Hit h) {
	vec3 d = r.pos - pos;
	vec3 dir = rotate(vec3(0,0,1),rot,1);
	dir = normalize(dir);
	float a = dot(r.dir,r.dir) - pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - dot(r.dir, dir) * dot(d, dir));
	float c = dot(d, d) - pow(dot(d, dir), 2) - data * data;
	float g = b*b - 4*a*c;

	if (g <= 0)
		return;

	float t1 = (-sqrt(g) - b) / (2*a);
	//float t2 = (sqrt(g) - b) / (2*a);

	if (t1 < EPSI)
		return ;

	if (t1 < h.dist){
		h.dist = t1;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = dir * (dot(r.dir, dir) * h.dist + dot(r.pos - pos, dir));
		vec3 tmp = h.pos - pos;
		h.norm = tmp - temp;
		h.mat = mat;
		vec3	d = h.pos -dot(pos, r.dir);
		h.uv = vec2(-(0.5 + (atan(d.z, d.x) / (M_PI * 0.25))), -((d.y / M_PI) - floor(d.y / M_PI)));
	}
}
