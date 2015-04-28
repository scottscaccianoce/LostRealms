uniform float elapsed;

void main(void)
{
	float speed = 5.5;
	vec3 pos = (gl_Normal * elapsed  * speed ) + gl_Vertex.xyz;
	
	//pos.x += sin(elapsed * speed) * 0.5;
	//pos.z += cos(elapsed * speed)* 0.5;
	
	
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos,1);
	
	gl_PointSize = 7.0;
}       