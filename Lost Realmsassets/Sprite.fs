
varying vec2 texCord;

uniform sampler2D spriteTex;
uniform vec3 colorKey;

void main (void)  
{     
	vec3 color = texture2D(spriteTex , texCord ).rgb;
	if ( color == colorKey) 
		discard;

	gl_FragColor = vec4(color, 1.0); 
}    