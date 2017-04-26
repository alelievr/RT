vec4	sepia(vec3 color)
{
	color.r = dot(color, vec3(.393, .769, .189));
	color.g = dot(color, vec3(.349, .686, .168));
	color.b = dot(color, vec3(.272, .534, .131));
	return vec4(color, 1);
}

vec4	black_white(vec3 color)
{
	float	t = max(color.r, max(color.g, color.b));
	return vec4(t, t, t, 1);
}

/* Encore quelques bugs avec le calculs de contours */
vec4	cartoon(vec3 color, vec3 light_pos, Hit h, Ray r)
{
    vec3	ldir = normalize(lpos - h.pos);
    float	intensity = dot(normalize(ldir), h.norm);
    float	factor = 0.5;

    if (intensity > 0.95)
        factor = 1.0;
    else if (intensity > 0.8)
        factor = 0.9;
    else if (intensity > 0.75)
        factor = 0.8;
    else if (intensity > 0.6)
        factor = 0.7;
    else if (intensity > 0.5)
        factor = 0.6;
    else if (intensity > 0.4)
        factor = 0.5;
    else if (intensity > 0.3)
        factor = 0.4;
    else if (intensity > 0.2)
        factor = 0.3;
    else if (intensity > 0.1)
        factor = 0.2;
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
    return vec4(color, 1.0);
}

vec3	antialiasing()
{

}

vec3	depth_of_field()
{

}

vec3	bloom()
{

}

vec3	lens_flare()
{

}

vec3	aces_tonemap()
{

}

vec3	vignetting()
{

}

vec3	daltonize()
{

}

vec3	nightvision(vec3 color)
{
	float	t = max(max(color.r, color.g), color.b);
	return vec4(0.0, t, 0.0, 1);
}
