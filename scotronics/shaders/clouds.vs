
varying vec2 uv;
varying float blend;

void main(void)
{

	vec4 wvp = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = wvp.xyww; //we do this so the z is not enabled for dept testing
  
	uv = vec2(gl_MultiTexCoord0).xy;
	blend = gl_MultiTexCoord0.z;

}       