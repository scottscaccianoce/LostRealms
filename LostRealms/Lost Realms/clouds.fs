uniform sampler2D cloudTex; 
uniform float cloudDensity;

varying vec2 uv;
varying float blend;

void main (void)  
{     

	vec4 cloudColor = texture2D(cloudTex , uv);
	cloudColor = cloudColor - (1.0 - cloudDensity);
	clamp(cloudColor, 0.0, 1.0);
	
	cloudColor.a = cloudColor.r  * blend;
	cloudColor.rgb = cloudColor.rgb + 0.79;
	gl_FragColor = cloudColor;  
}    