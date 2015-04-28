
uniform vec3 lightdir;
uniform vec4 eyePosW;

uniform vec2 map0_offset;
uniform vec2 map1_offset;


varying vec2 tiledTexCord;
varying vec2 tiledTexCord1;


varying vec3 outLightDir;
varying vec3 toEyeT;
varying vec3 lightDirT;
varying float fog;

void main(void)
{
		
	mat3 TBN = mat3(1.0, 0.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0
	);
	
	
	vec3 normal;
	
	vec4 eyePostemp = gl_ModelViewMatrix * eyePosW;
	vec3 eyePosL = vec3(eyePostemp) / eyePosW.w;

	vec3 toEyeL = eyePosL - gl_Vertex.xyz;
	toEyeT = toEyeL * TBN;
	lightDirT = lightdir * TBN;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	outLightDir = normalize(vec3(lightdir) - gl_Vertex.xyz);
	tiledTexCord = vec2(gl_MultiTexCoord0) + map0_offset;
	tiledTexCord1 = vec2(gl_MultiTexCoord0) + map1_offset;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  
	float d = distance(vec3(gl_ModelViewProjectionMatrix *gl_Vertex).xyz, eyePosW.xyz);
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);



}       