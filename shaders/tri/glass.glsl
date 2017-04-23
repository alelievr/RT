void glass(vec3 pos, vec3 rot, float data, Material mat, Ray r, inout Hit hit)
{
	Hit h;
	h = hit;
	Coupes c;
  float hauteur = 3 * data;
	// r.pos = rotate(r.pos - pos, rot, 1);
	// r.dir = rotate(r.dir, rot, 1);

  vec3 dir = rotate(vec3(0,0,1),rot,1);
  c.t[0] = vec4(0,0,1,data / 3);
  c.t[1] = vec4(0);
  sphere(pos + (data / 2 + hauteur / 2) * dir, data, c, mat, r, h);
  c.t[0] = vec4(0,0,1,hauteur/2);
  c.t[1] = vec4(0,0,1,hauteur/2);
  c.t[2] = vec4(0);
  cyl(pos, vec3(0,0,0), data / 8, c, mat, r, h);
  c.t[0] = vec4(0,0,1,0);
  c.t[1] = vec4(0,0,1,-0.3);
  c.t[2] = vec4(0);
  cone(pos - (hauteur / 2) * dir, vec3(0,0,0), 80, c, mat, r, h);
	// if (h.dist != hit.dist){
	// 	hit.pos = rotate(hit.pos, rot, 0);
	// 	hit.pos += pos;
	// 	hit.norm = rotate(hit.norm, rot, 0);
	// }
}
