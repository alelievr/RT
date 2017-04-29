vec3	sepia(vec3 color)
{
	vec3	col = vec3(	clamp(color.r * 0.393 + color.g * 0.769 + color.b * 0.189, 0.0, 1.0)
		 			  , clamp(color.r * 0.349 + color.g * 0.686 + color.b * 0.168, 0.0, 1.0)
		 			  , clamp(color.r * 0.272 + color.g * 0.534 + color.b * 0.131, 0.0, 1.0));
		return col;
}

vec3	black_white(vec3 color)
{
	float	t = max(color.r, max(color.g, color.b));
	return vec3(t, t, t);
}

vec3	cartoon(vec3 color, vec3 lpos, Hit h, Ray r)
{
	vec3	ldir = normalize(lpos - h.pos);
	float	intensity = dot(normalize(ldir), h.norm);
	float	factor = 0.5;

	if (intensity > 0.9)
		factor = 1.0;
	else if (intensity > 0.6)
		factor = 0.7;
	else if (intensity > 0.4)
		factor = 0.5;
	else if (intensity > 0.2)
		factor = 0.3;
	else if (intensity > 0.0)
		factor = 0.1;
	else
		factor = 0;
	color *= vec3(factor, factor, factor);

	vec3	L = (lpos - h.pos);
	vec3	V = normalize(r.pos - h.pos);
	vec3	H = normalize(L + V);

	float	edge = (dot(V, h.norm) > 0.5) ? 1 : 0;
	color *= edge;
	return color;
}

vec3	nightvision(vec3 color)
{
	float	t = max(max(color.r, color.g), color.b);
	return vec3(0.0, t, 0.0);
}

vec3	vignetting(vec3 color, vec2 coord)
{
	vec2	pos = coord;
	float	len = length(pos);
	float	vignette = smoothstep(0.9, 0.9 - 0.45, len);
	color = mix(color, color * vignette, 0.5);
	return color;
}
