vec3 raycast(vec3 r0, vec3 rd);

void		mainImage(vec2 coord)
{
	vec2	uv = (coord / iResolution) * 2 - 1;
	vec3	cameraPos = iMoveAmount.xyz;
	vec3	cameraDir = iForward;
	vec3	col;

	//window ratio correciton:
	uv.x *= iResolution.x / iResolution.y;

	//perspective view
	float	fov = 1.5;
	vec3	forw = normalize(iForward);
	vec3	right = normalize(cross(forw, vec3(0, 1, 0)));
	vec3	up = normalize(cross(right, forw));
	vec3	rd = normalize(uv.x * right + uv.y * up + fov * forw);

	fragColor = vec4(raycast(cameraPos, rd), 1);
}
