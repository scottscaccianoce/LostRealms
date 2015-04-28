uniform float elapsed;

void main(void)
{
	float speed = 1.5;
	vec3 pos = (gl_Normal * elapsed  * speed ) + gl_Vertex.xyz;
	
	
	
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos,1);
	
	//gl_PointSize = 8.0 + elapsed * 5.5;
	gl_PointSize = 8.0;
}       