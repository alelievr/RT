void cone(vec3 pos, vec3 rot, float data, Coupes coupe, Material mat, Ray r, inout Hit h)
{
	vec3 d = r.pos - pos;
	vec3 dir = rotate(vec3(0,0,1), rot, 1);

	float a = dot(r.dir, r.dir) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(r.dir, dir), 2);
	float b = 2 * (dot(r.dir, d) - (1 + pow(tan(data * M_PI / 180), 2)) * dot(r.dir, dir) * dot(d , dir));
	float c = dot(d, d) - (1 + pow(tan(data * M_PI / 180), 2)) * pow(dot(d, dir), 2);

	float g = b*b - 4*a*c;

	if (g <= 0)
		return ;

	float t = (-sqrt(g) - b) / (2*a);
	float t1 = (sqrt(g) - b) / (2*a);

	vec3 inter = r.pos + r.dir * t;

	if (t > 0 && t < h.dist && !decoupe(pos, inter, coupe, rot)) {
		h.dist = t;
		h.pos = r.pos + r.dir * h.dist;
		vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - pos, dir))) * (1 + pow(tan(data * M_PI / 180), 2));
		vec3 tmp = h.pos - pos;
		h.norm = tmp - temp;
		h.mat = mat;
		vec3	d = normalize(h.pos - pos);
		h.uv = vec2(-(0.5 + (atan(h.norm.z, h.norm.x) / (M_PI * 2))), (h.norm.y /  M_PI) - floor(h.norm.y / M_PI));
		return;
	}

	vec3 inter1 = r.pos + r.dir * t1;

	if ((t < 0 || decoupe(pos, inter, coupe, rot)) && !decoupe(pos, inter1, coupe, rot)){
		if (t1 > 0 && t1 < h.dist){
			h.dist = t1 ;
			h.pos = r.pos + r.dir * h.dist;
			vec3 temp = (dir * (dot(r.dir, dir) * h.dist + dot(r.pos - pos, dir))) * (1 + pow(tan(data * M_PI / 180), 2));
			vec3 tmp = h.pos - pos;
			h.norm = tmp - temp;
			h.mat = mat;
			vec3	d = normalize(h.pos - pos);
			h.uv = vec2(-(0.5 + (atan(h.norm.z, h.norm.x) / (M_PI * 2))), (h.norm.y /  M_PI) - floor(h.norm.y / M_PI));
		}
		return;
	}
}
