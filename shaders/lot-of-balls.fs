struct ray {
	vec3 p;
	vec3 d;
};

struct material {
	vec3 color;
	vec3 emmision;
	float diffusion;
};

struct hit {
	vec3 p;
	vec3 n;
	float t;
	material m;
};

float time = iGlobalTime;

void plane (vec3 v, float f, ray r, material m, inout hit h) {
	float t = (f - dot (v, r.p)) / dot (v, r.d);

	if (t < 0.0)
		return;

	if (t < h.t) {
		h.t = t;
		h.p = r.p + r.d * t;
		h.n = (faceforward (v, v, r.d));

		h.m = m;
	}
}

bool sphere (vec3 v, float f, ray r, material m, inout hit h) {
	vec3 d = r.p - v;

	float a = dot (r.d, r.d);
	float b = dot (r.d, d);
	float c = dot (d, d) - f * f;

	float g = b*b - a*c;

	if (g < 0.0)
		return false;

	float t = (-sqrt (g) - b) / a;

	if (t < 0.0)
		return false;

	if (t < h.t) {
		h.t = t;
		h.p = r.p + r.d * t;
		h.n = (h.p - v) / f * (sin(iGlobalTime) / 2 - .6);

		h.m = m;
		return true;
	}
}


void rayTrace(ray r, inout hit h) {
	material m;
	m.color = vec3(1, 1, 1);

	if (sphere(vec3(0, 0, -2), 1., r, m, h))
		fragColor = vec4(1, 1, 1, 1);
	else
		fragColor = vec4(0, 0, 0, 1);
}


void mainImage( in vec2 fragCoord ) {
	vec2 p = (fragCoord / iResolution) * 2 - 1;
	p.x *= iResolution.x/iResolution.y;
	
	// camera	
	vec3 cameraPosition = vec3( cos( 0.232*time) * 10., 6. + 3. * cos(0.3 * time), time * 30);
	vec3 ro = cameraPosition;
	vec3 ta = ro + iRotation;
	
	float roll = -0.15*sin(0.5*time);
	// camera tx
	vec3 cw = normalize( ta-ro );
	vec3 cp = vec3( sin(roll), cos(roll),0.0 );
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv = normalize( cross(cu,cw) );
	vec3 rd = normalize( p.x*cu + p.y*cv + 1.5*cw );
	
	// raytrace
	int material;
	vec3 normal, intersection;
	float dist;

	hit		h;
	ray		cameraRay;

	cameraRay.p = ro;
	cameraRay.d = rd;
	
	rayTrace(cameraRay, h);

	/*col = pow( col, vec3(EXPOSURE, EXPOSURE, EXPOSURE) );	
	col = clamp(col, 0.0, 1.0);*/
	
}
