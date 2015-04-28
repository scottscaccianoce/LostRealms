
varying vec2 texCord;


uniform sampler2D billTex;
uniform float alphaFactor;
uniform float useAlpha;

void main (void)  
{     
	vec4 color = texture2D(billTex , texCord ).rgba;
	float alpha = alphaFactor;
	if (useAlpha == 1.0)
		alpha = color.a;
		
	if ( color.a == 0.0 )
		discard;

	gl_FragColor = vec4(color.rgb, alphaFactor); 

}    