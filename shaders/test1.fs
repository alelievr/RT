float iSphere(in vec3 ro, in vec3 rd, in vec4 sph)
{
    // a sphere centered at the origin has equation |xyz| = r
    // meaning, |xyz|^2 = r^2, meaning <xyz, xyz> = r^2
    // now, xyz = ro + t*rd, therefore |ro|^2+|t*rd|^2 + 2<ro, rd> t - r^2 = 0
	// |rd| = 1 (normalized) so equation reduce to |ro|^2+ t^2 + 2<ro, rd> t - r^2 = 0
    // which is a quadratic equation, so

    vec3 oc = ro - sph.xyz;
    float b = 2.0 *dot(oc, rd);
    float c = dot(oc,oc) - sph.w*sph.w;
    float h = b*b - 4.0 *c;
    if(h < 0.0) return -1.0; //no intersection

    //pick smaller one(i.e, close one)
    //not (-b+sqrt(h)) /2
    float t = (-b - sqrt(h))/ 2.0;
    return t;
}

void	mainImage(vec2 coord)
{
	vec2	uv = (coord / iResolution) * 2 - 1;
	vec3	cameraPos = -iMoveAmount.xyz;
	vec3	cameraDir = iForward;

	//window ratio correciton:
	uv *= iResolution.x / iResolution.y;

	//perspective view
	float	fov = 1.5;
	vec3	forw = normalize(iForward);
	vec3	right = normalize(cross(forw, vec3(0, 1, 0)));
	vec3	up = normalize(cross(right, forw));
	vec3	rd = normalize(uv.x * right + uv.y * up + fov * forw);

	if (iSphere(cameraPos, rd, vec4(0, 0, -10, 1)) != -1)
		fragColor = vec4(1, 1, 1, 1);
	else
		fragColor = vec4(0, 0, 0, 1);
}
