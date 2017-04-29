float	damier(vec2 uv)
{
	return mod(floor(10 * uv.x) + floor(10 * uv.y), 2.0);
}

float	brick(vec2 uv)
{
	return 	smoothstep(0.4, 0.5, max(abs(fract(8.0 * uv.x - 0.5 * mod(floor(8.0 * uv.y), 2.0)) - 0.5), abs(fract(8.0 * uv.y) - 0.5)));
}

float rand(vec2 n)
{ 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float	noise(vec2 p)
{
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);

	float	res = mix(	mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
			mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

float noise_mouve(vec2 p)
{
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);

	float	res = mix(	mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
			mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return 20 * sin(iGlobalTime * res*res);
}
