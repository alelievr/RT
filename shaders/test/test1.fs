#define MAX_REFLECTIONS 4

#define REFLECTION_STRENGTH 100000.0
#define FOG_STRENGTH 1.0
#define COLOR_STRENGTH 15.0
#define BRIGHTNESS 60.0

struct ray
{
	vec3 origin;
	vec3 dir;
};

struct material
{
	vec4		color;
	vec3		emmision;
	float		diffusion;
	bool		reflect;
};

struct hit
{
	vec3 point;
	vec3 norm;
	float dst;
	material mat;
};

void	iSphere(ray r, in vec4 sph, inout hit h, material m)
{
    vec3 oc = r.origin - sph.xyz;
    float b = 2.0 * dot(oc, r.dir);
    float c = dot(oc, oc) - sph.w*sph.w;
    float h1 = b*b - 4.0 *c;

    float t = (-b - sqrt(h1))/ 2.0;
    if (t < 0.0)
		return ;

	if (t < h.dst)
	{
		h.dst = t;
		h.point = r.origin + r.dir * t;
		h.norm = (h.point - sph.xyz) / sph.w;
		h.mat = m;
	}
}

void	iPlane(ray r, in vec4 plane, inout hit h, material m)
{
	float t = (plane.w - dot(plane.xyz, r.origin) / dot(plane.xyz, r.dir));

	if (t < 0.0)
		return ;

	if (t < h.dst)
	{
		h.dst = t;
		h.point = r.origin + r.dir * t;
		h.norm = faceforward(plane.xyz, plane.xyz, r.dir);
		h.mat = m;
	}
}

void	scene(ray r, inout hit h)
{
	material	purpleMaterial = material(vec4(.65, 0, 1, 1), vec3(0, 1, 1), 0.2, false);
	material	mirrorMaterial = material(vec4(1, 1, 1, 1), vec3(1, 1, 1), .1, true);

	iSphere(r, vec4(2, 2, 10, 1), h, purpleMaterial);
	iSphere(r, vec4(-2, 2, 10, 1), h, purpleMaterial);
	iPlane(r, vec4(0, 1, 0, 0), h, mirrorMaterial);
}

vec3	surface(ray r)
{
	hit		h;
	vec3	color = vec3(0);
	float	depth = 0;
	float	l = 0;

	h.dst = 1e120;
	for (int i = 0; i < MAX_REFLECTIONS; i++)
	{
		scene(r, h);

		if (h.dst < 1e100)
		{
			float c = dot(h.norm, r.dir);
			depth += (1.0 / REFLECTION_STRENGTH) + h.dst * FOG_STRENGTH;

			float d = 1.0 / (depth * depth);
			color = (color + c * c * d) * (1.0 - h.mat.color.xyz * d * COLOR_STRENGTH);
			if (!h.mat.reflect)
				break ;
			r = ray(h.point + h.norm * 0.0001, reflect(normalize(r.dir), h.norm));
		}
		else
			break ;
	}

	return color * BRIGHTNESS;
}

#undef BRIGHTNESS
#define BRIGHTNESS .7
#define BLUR_RADIUS 2
vec4 gaussianBlur( vec4 color, in vec2 fragCoord )
{
	float w = color.w;

	vec4 blur;
	for (int x = -BLUR_RADIUS; x <= BLUR_RADIUS; x++)
		for (int y = -BLUR_RADIUS; y <= BLUR_RADIUS; y++)
			blur += color * length(vec2(x,y));

	blur = blur / ((float(BLUR_RADIUS) *2. +1.) * (float(BLUR_RADIUS) *2. +1.));

	return mix(color, blur, w) / (BRIGHTNESS + w);
}

void	mainImage(vec2 coord)
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

	ray		camRay = ray(cameraPos, rd);

	col = surface(camRay);
	//fragColor = gaussianBlur(vec4(col, 1), coord);
	fragColor = vec4(col, 1);
}
