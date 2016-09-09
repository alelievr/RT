bool testSphere( out vec3 normal, in vec3 ray, in vec3 sphere, in float radius )
{
    float A = dot( ray, ray );
    float B = dot( ray, sphere )*(-2.0);
    float C = dot( sphere, sphere ) - radius*radius;
    float det = B*B - 4.0*A*C;
    if( det < 0.0 ) return false;
    
    float t = (-B+sqrt(det)) / (2.0*A);
    
    vec3 hit = ray*t;
   
    normal = normalize( hit - sphere );
    return true;
}

bool scene( out vec3 normal, in vec3 ray )
{
    if( testSphere( normal, ray, vec3(0,0,5), 1.0 ))
        return true;
    return false;
}

void mainImage( in vec2 fragCoord )
{
	vec2 uv = fragCoord / iResolution;
    vec3 ray = normalize( vec3( (fragCoord.xy - iResolution.xy * 0.5) / iResolution.xx, 1.0 )) + iMoveAmount.xyz;
    vec3 L = normalize( vec3(1.0,1.0,1.0));
    vec3 normal;
    
    float Y = fragCoord.y / iResolution.y;
    
	//vec3 Sky = vec3( 0.45, 0.75, 1.0 )*Y*0.5;
    vec3 Sky = vec3(0);
    
    vec3 frag = Sky;
    
    if(scene( normal, ray ))
    {
        float dotnormalL =  max( dot( normal, L ), 0.0 );
        vec3 H = normalize( ray - reflect(ray,normal));
        float dotHL = max( dot( H, L ), 0.0 );
        float spec = pow( dotHL, 32.0 );
        
        float Albedo = 0.9;
        
        float F = pow( 1.0 - max( dot( ray, normal ), 0.0 ), 5.0);  // fresnel
        
        float Ambient = Albedo + (1.0-Albedo)*F;
        
        frag = vec3(1,0.5,0.5)*dotnormalL*Albedo;
        frag += vec3(1,1,1)*spec*Albedo;
        frag += Sky * Ambient;
    }
    fragColor = vec4( frag, 1.0 );
}
