uniform vec3 CameraPos;
uniform vec3 lightdir;

varying vec2 texCord;
varying float shade;
varying float fog;

void main(void)
{
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightDir = normalize(vec3(lightdir));
	shade = max(dot(normal,lightDir),0.0) + 0.4;
	
	
	texCord = vec2(gl_MultiTexCoord0);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  
	float d = distance(vec3(gl_ModelViewProjectionMatrix *gl_Vertex).xyz, vec3(0.0, 0.0 ,0.0));
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);


}       