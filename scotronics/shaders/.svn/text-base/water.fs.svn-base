varying vec2 tiledTexCord;
varying vec2 tiledTexCord1;

varying vec3 outLightDir;
varying vec3 toEyeT;
varying vec3 lightDirT;

uniform sampler2D map0;
uniform sampler2D map1;
uniform vec3 diffuseMtrl;
uniform vec3 ambientMtrl;
uniform vec3 specularMtrl;
uniform float specPower;

varying float fog;

void main (void)  
{     
	vec3 mtoEyeT = normalize(toEyeT);
	vec3 mlightDirT = normalize(lightDirT);

	vec3 normal0 = texture2D(map0 , tiledTexCord).rgb;
	vec3 normal1 = texture2D(map1 , tiledTexCord1).rgb;
	
	normal0 = 2.0 * normal0 - 1.0;
    normal1 = 2.0 * normal1 - 1.0;
	
	//avage vectors;
	vec3 normalT = normalize(0.5 * (normal0 + normal1));
	
	float s = max(dot(-mlightDirT, normalT), 0.0);
	vec3 r = reflect(-mlightDirT, normalT);
    float t  = pow(abs(max(dot(r, mtoEyeT), 0.0)), specPower);
	
	if ( s <= 0.0)
	  t = 0.0;
	
	// Compute the ambient, diffuse and specular terms separatly. 
	vec3 spec = t*(specularMtrl);
	vec3 diffuse = s*(diffuseMtrl);
	vec3 ambient = ambientMtrl;
	
	vec3 final = ambient + diffuse + spec;
	
   gl_FragColor = vec4(final, 0.7);  
}    