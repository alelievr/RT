void glass(vec3 pos, vec3 rot, float data, Material mat, Ray r, inout Hit h)
{
	Hit hit;
	hit = h;
	Coupes c;
	// rot = rot * iGlobalTime;
  float hauteur = 2 * data;
	r.pos = rotate(r.pos - pos, rot, 1);
	r.dir = rotate(r.dir, rot, 1);

	vec3 dir = vec3(0,0,1);
  c.t[0] = vec4(0,0,1,-data / 3);
  c.nsl = 1;
  sphere(pos + (data + hauteur / 2) * dir, vec3(0), data, c, mat, r, h);
  c.t[0] = vec4(0,0,1,-hauteur/2);
  c.t[1] = vec4(0,0,-1,-hauteur/2);
	c.nsl = 2;
  cyl(pos, vec3(0), data / 8, c, mat, r, h);
  c.t[0] = vec4(0,0,1,0);
  c.t[1] = vec4(0,0,-1,-hauteur/12);
	c.nsl = 2;
	cone(pos - (hauteur / 2) * dir, vec3(0), 80, c, mat, r, h);
	 if (h.dist != hit.dist){
	 	h.pos = rotate(h.pos, rot, 0);
	 	h.pos += pos;
	 	h.norm = rotate(h.norm, rot, 0);
	}
}
