

varying vec2 tiledTexCord;
varying vec2 tiledTexCord2;
varying vec2 tiledTexCord3;
varying vec2 alphaMapCord;

varying float shade; //lighting
varying vec3 posW;

uniform int draw_circle;
uniform float radius;
uniform vec3 circle_pos;

uniform sampler2D grassTex;
uniform sampler2D dirtTex;
uniform sampler2D rockTex;
uniform sampler2D snowTex;

uniform sampler2D AlphaMapTex;

varying float fog;
varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;
void main (void)  
{     
	vec3 c0 = texture2D(grassTex , tiledTexCord * 1.0).rgb;
	vec3 tc0 = texture2D(grassTex, tiledTexCord3).rgb;
	vec3 c1 = texture2D(dirtTex, tiledTexCord).rgb;
	vec3 c2 = texture2D(rockTex, tiledTexCord).rgb;
	vec3 tc2 = texture2D(rockTex, tiledTexCord2).rgb;
	
	vec3 c3 = texture2D(snowTex, tiledTexCord).rgb;
	vec4 blendmap = texture2D(AlphaMapTex, alphaMapCord);
	
	
	// Find the inverse of all the blend weights so that we can
	// scale the total color to the range [0, 1].
    float totalInverse = 1.0 / (blendmap.r + blendmap.g + blendmap.b + blendmap.a);
   

    // Scale the colors by each layer by its corresponding weight
    // stored in the blendmap.  
    c0 *= blendmap.r  * totalInverse *0.8;
    c1 *= blendmap.g  * totalInverse;
    c2 *= blendmap.b  * totalInverse * 0.5;
    c3 *= blendmap.a  * totalInverse ;
	tc2 *= blendmap.b * totalInverse * 0.5;
	tc0 *= blendmap.r * totalInverse * 0.5;

	//Lighting
	float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir))) + 0.7;
	

	vec4 final = vec4((c0 + c1 + c2 + c3 + tc2 ), 1.0) * diff;
		
	
	
	// CIRCLE CODE--------------------------------------------------
	if (draw_circle == 1 ) {
		
       float dist = sqrt((posW.x - circle_pos.x) * (posW.x - circle_pos.x) + (posW.z - circle_pos.z) * (posW.z - circle_pos.z));
       if( dist <= radius && dist > (radius * 0.95))
	   {
			final = vec4(1.0,1.0,1.0,1.0);
	   }
	}
	
	vec3 color = final.rgb * fog + (gl_Fog.color.rgb) * (1.0 - fog);
	gl_FragColor = vec4(color, 1.0);
}    