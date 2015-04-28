

varying vec4 waterTex0;
varying vec4 waterTex1;
varying vec4 waterTex2;
varying vec4 waterTex3;
varying vec4 waterTex4;
uniform vec4 viewpos, lightpos;

//unit 0 = water_reflection
//unit 1 = water_refraction
//unit 2 = water_normalmap
//unit 3 = water_dudvmap
//unit 4 = water_depthmap

uniform vec2 map0_offset;
uniform vec2 map1_offset;

varying vec3 ReflectionMapSamplingPos;
varying vec2 BumpMapSamplingPos;
varying vec2 RefractionMapSamplingPos;
varying vec2 texin;

varying float fog;
varying vec4 mpos;



void main(void)

{
/*
    vec4 mpos, temp;
    vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
    vec4 norm = vec4(0.0, 1.0, 0.0, 0.0);
    vec4 binormal = vec4(0.0, 0.0, 1.0, 0.0);
    mat4 mvp = gl_ModelViewProjectionMatrix;
    mat4 mtx = gl_TextureMatrix[0];
    temp = viewpos - gl_Vertex;
    waterTex4.x = dot(temp, tangent);
    waterTex4.y = dot(temp, binormal);
    waterTex4.z = dot(temp, norm);
    waterTex4.w = 0.0;
    temp = lightpos - gl_Vertex;
	waterTex0.x = dot(temp, tangent);
    waterTex0.y = dot(temp, binormal);
    waterTex0.z = dot(temp, norm);
    waterTex0.w = 0.0;
    mpos = mvp * gl_Vertex;
	waterTex1 = vec4(vec2(gl_MultiTexCoord0) + map0_offset, 0.0, 0.0);
	waterTex2 = vec4(vec2(gl_MultiTexCoord0) + map1_offset, 0.0, 0.0);
    waterTex3 = mpos;
	    gl_Position = ftransform();

	*/
	
	mpos = gl_ModelViewProjectionMatrix * gl_Vertex;


	ReflectionMapSamplingPos.x = 0.50 * (mpos.w + mpos.x);
	ReflectionMapSamplingPos.y = 0.50 * (mpos.w + mpos.y);
	ReflectionMapSamplingPos.z = mpos.w;


	//Texture coords
	BumpMapSamplingPos = vec2(gl_MultiTexCoord0) + map0_offset ;
	RefractionMapSamplingPos = vec2(gl_MultiTexCoord0) + map1_offset ;
	texin = vec2(gl_MultiTexCoord0);
	
	
	vec4 eyePos = gl_ModelViewMatrix * gl_Vertex;
	//fog =  abs(eyePos.z/eyePos.w);
	
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	float d = distance(vec3(gl_ModelViewProjectionMatrix * gl_Vertex).xyz, vec3(0.0, 0.0 ,0.0));
	fog = clamp((gl_Fog.end - d) / (gl_Fog.end - gl_Fog.start), 0.0, 1.0);


	
}
