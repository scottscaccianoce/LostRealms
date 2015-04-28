uniform vec3 CameraPos;
uniform vec3 lightdir;
uniform float tween;

attribute vec3 NextVertex;

varying vec2 texCord;
varying float shade;
varying float fog;

void main(void)
{
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightDir = normalize(vec3(lightdir));
	shade = max(dot(normal,lightDir),0.0) + 0.7;
	
	
	texCord = vec2(gl_MultiTexCoord0);
	vec4 aPos;
	aPos.xyz = mix(gl_Vertex.xyz, NextVertex, tween);
	aPos.w = 1.0;
	
	gl_Position = gl_ModelViewProjectionMatrix * aPos;
	
    //gl_Position = gl_ModelViewProjectionMatrix * vec4(NextVertex, 1.0);
  
	float d = distance(vec3(gl_ModelViewProjectionMatrix *aPos).xyz, vec3(0.0, 0.0 ,0.0));
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);


}       