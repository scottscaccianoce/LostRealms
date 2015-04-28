uniform vec4 skycolor;
uniform float skyMin;
uniform float skyMax;
varying float vertheight;


void main (void)  
{     
   float vertexHeight = clamp(vertheight, skyMin, skyMax);
   float blendBasedOnHeight = 1.6 - (vertexHeight / skyMax);
   
   vec3 final = skycolor.rgb * blendBasedOnHeight;
   gl_FragColor = vec4(final,1.0);  
 
}    