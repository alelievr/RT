vec4 yolo(vec2 coord)
{
	vec2 uv = coord / iResolution;
	vec3 c1 = texture(iChannel0, uv).xyz;
	vec3 c2 = texture(iChannel1, uv).xyz;
	return vec4(mix(c1, c2, .5), 1);
}
