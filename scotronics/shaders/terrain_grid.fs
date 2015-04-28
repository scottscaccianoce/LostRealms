


varying vec3 posW;
uniform int draw_circle;
uniform float radius;
uniform vec3 circle_pos;


void main (void)  
{     
	vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
	
	if (draw_circle == 1 ) {
		
       float dist = sqrt((posW.x - circle_pos.x) * (posW.x - circle_pos.x) + (posW.z - circle_pos.z) * (posW.z - circle_pos.z));
       if( dist <= radius && dist > (radius * 0.95))
	   {
			color = vec4(1.0,1.0,1.0,1.0);
	   }
	}
	gl_FragColor = color;
}    