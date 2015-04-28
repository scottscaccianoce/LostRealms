uniform vec4 color;
uniform vec4 color2;
uniform float percent;

void main (void)  
{     
	vec3 c1 = ( 1.0 - percent) * color.rgb;
	vec3 c2 = (   percent) * color2.rgb;
	vec3 c3 = c1 + c2;

	gl_FragColor = vec4(color.rgb,color.a);
	
}    