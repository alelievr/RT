struct ray
{
	vec3 origin;
	vec3 direction;
};

struct hit
{
	vec3 point;
	vec3 normal;
	float distance;
	//material
};

void	iSphere(ray r, in vec4 sph, out hit h)
{
    // a sphere centered at the origin has equation |xyz| = r
    // meaning, |xyz|^2 = r^2, meaning <xyz, xyz> = r^2
    // now, xyz = ro + t*rd, therefore |ro|^2+|t*rd|^2 + 2<ro, rd> t - r^2 = 0
	// |rd| = 1 (normalized) so equation reduce to |ro|^2+ t^2 + 2<ro, rd> t - r^2 = 0
    // which is a quadratic equation, so

    vec3 oc = r.origin - sph.xyz;
    float b = 2.0 *dot(oc, r.direction);
    float c = dot(oc, oc) - sph.w*sph.w;
    float h1 = b*b - 4.0 *c;
    if(h1 < 0.0)
		return ;

    //pick smaller one(i.e, close one)
    //not (-b+sqrt(h)) /2
    float t = (-b - sqrt(h1))/ 2.0;
	h.distance = t;
	h.point = r.origin + r.direction * t;
	h.normal = (h.point - sph.xyz) / sph.w * (sin(iGlobalTime) / 2 - .6);
}

void	mainImage(vec2 coord)
{
	vec2	uv = (coord / iResolution) * 2 - 1;
	vec3	cameraPos = -iMoveAmount.xyz;
	vec3	cameraDir = iForward;

	//window ratio correciton:
	uv.x *= iResolution.x / iResolution.y;

	//perspective view
	float	fov = 1.5;
	vec3	forw = normalize(iForward);
	vec3	right = normalize(cross(forw, vec3(0, 1, 0)));
	vec3	up = normalize(cross(right, forw));
	vec3	rd = normalize(uv.x * right + uv.y * up + fov * forw);

	ray		camRay = ray(cameraPos, rd);
	hit		h;

	iSphere(camRay, vec4(0, 0, -10, 1), h);
	if (h.distance != 0)
	{
		fragColor = vec4(h.normal, 1);
	}
	else
		fragColor = vec4(0, 0, 0, 1);
}
