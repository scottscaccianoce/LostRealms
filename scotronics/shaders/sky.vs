varying float vertheight;


void main(void)
{
	vertheight = gl_Vertex.y;
	vec4 wvp = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = wvp.xyww; //we do this so the z is not enabled for dept testing
	
}       