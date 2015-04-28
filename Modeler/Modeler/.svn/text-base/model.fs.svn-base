
varying vec2 texCord;
varying float shade;

uniform sampler2D modelTex;
varying float fog;

void main (void)  
{     
	vec3 color = texture2D(modelTex , texCord ).rgb * shade * 0.8;
	//vec3 color = texture2D(modelTex , texCord ).rgb  * 0.8;
	vec3 finalcolor = color * fog + (gl_Fog.color.rgb) * (1.0 - fog);
	gl_FragColor = vec4(finalcolor, 1.0); 
}    