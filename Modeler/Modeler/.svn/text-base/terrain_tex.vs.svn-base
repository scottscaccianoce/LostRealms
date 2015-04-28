
uniform vec3 lightdir;
uniform vec3 CameraPos;
uniform vec3 lightPos;

varying vec3 posW;
varying vec2 tiledTexCord;
varying vec2 tiledTexCord2;
varying vec2 tiledTexCord3;


varying vec2 alphaMapCord;
varying float shade;
varying float fog;

varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;

void main(void)
{
/*
	vec3 normal, lightDir;
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	lightDir = normalize(vec3(lightdir));
	shade = max(dot(normal,lightDir),0.0) + 0.7;
	
	
	tiledTexCord = vec2(gl_MultiTexCoord0) * 50.0 * 4.0;
	tiledTexCord2 = vec2(gl_MultiTexCoord0) * 3.0 * 4.0;
	tiledTexCord3 = vec2(gl_MultiTexCoord0) * 24.0 * 4.0;
	
	alphaMapCord = vec2(gl_MultiTexCoord0);
   
   
	posW = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	float d = distance(vec3(gl_ModelViewProjectionMatrix *gl_Vertex).xyz, vec3(0.0, 0.0, 0.0));
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);
*/
	
	//=====================================
	vVaryingNormal = gl_NormalMatrix * gl_Normal;
	vec4 pos4 = gl_ModelViewMatrix *gl_Vertex;
	vec3 pos3 = pos4.xyz/ pos4.w;
	vVaryingLightDir = normalize(lightPos - pos3);

	
	tiledTexCord = vec2(gl_MultiTexCoord0) * 50.0 * 4.0;
	tiledTexCord2 = vec2(gl_MultiTexCoord0) * 3.0 * 4.0;
	tiledTexCord3 = vec2(gl_MultiTexCoord0) * 24.0 * 4.0;
	
	alphaMapCord = vec2(gl_MultiTexCoord0);
   
   
	posW = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	float d = distance(vec3(gl_ModelViewProjectionMatrix *gl_Vertex).xyz, vec3(0.0, 0.0, 0.0));
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);
	
	
	
}       