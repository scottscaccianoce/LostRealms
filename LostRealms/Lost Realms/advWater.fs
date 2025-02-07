

uniform sampler2D water_normalmap;
uniform sampler2D water_reflection;
uniform sampler2D water_refraction;
uniform sampler2D water_dudvmap;
uniform sampler2D water_depthmap;
uniform vec4 waterColor, waterDepth;



varying vec4 waterTex0; //lightpos
varying vec4 waterTex1; //moving texcoords
varying vec4 waterTex2; //moving texcoords
varying vec4 waterTex3; //for projection
varying vec4 waterTex4; //viewts



//unit 0 = water_reflection
//unit 1 = water_refraction
//unit 2 = water_normalmap
//unit 3 = water_dudvmap
//unit 4 = water_depthmap

varying vec3 ReflectionMapSamplingPos;
varying vec2 BumpMapSamplingPos;
varying vec2 RefractionMapSamplingPos;
varying vec2 texin;

varying float fog;
varying vec4 mpos;





void main(void)

{
/*
     const vec4 sca = vec4(0.005, 0.005, 0.005, 0.005);
     const vec4 sca2 = vec4(0.02, 0.02, 0.02, 0.02);
     const vec4 tscale = vec4(0.25, 0.25, 0.25, 0.25);
     const vec4 two = vec4(2.0, 2.0, 2.0, 1.0);
     const vec4 mone = vec4(-1.0, -1.0, -1.0, 1.0);
     const vec4 ofive = vec4(0.5,0.5,0.5,1.0);
     const float exponent = 64.0;
     vec4 lightTS = normalize(waterTex0);
     vec4 viewt = normalize(waterTex4);
     vec4 disdis = texture2D(water_dudvmap, vec2(waterTex2 * tscale));
     vec4 dist = texture2D(water_dudvmap, vec2(waterTex1 + disdis*sca2));
     vec4 fdist = dist;
     fdist = fdist * two + mone; 
     fdist = normalize(fdist);
     fdist *= sca;

     //load normalmap
     vec4 nmap = texture2D(water_normalmap, vec2(waterTex1 + disdis*sca2));
     nmap = (nmap-ofive) * two;
     vec4 vNorm = nmap;
	vNorm = normalize(nmap);

     //get projective texcoords
     vec4 tmp = vec4(1.0 / waterTex3.w);
     vec4 temp = tmp;

     vec4 projCoord = waterTex3 * tmp;
     projCoord += vec4(1.0);
     projCoord *= vec4(0.5);
     tmp = projCoord + fdist;
     tmp = clamp(tmp, 0.001, 0.999);

     //load reflection,refraction and depth texture
     vec4 refTex = texture2D(water_reflection, vec2(tmp));
     vec4 refl = refTex;
     vec4 refr = texture2D(water_refraction, vec2(tmp));
     vec4 wdepth = texture2D(water_depthmap, vec2(tmp));

     wdepth = vec4(pow(wdepth.x, 4.0));
     vec4 invdepth = 1.0 - wdepth;

     //calculate specular highlight
     vec4 vRef = normalize(reflect(-lightTS, vNorm));
     float stemp =max(0.0, dot(viewt, vRef) );
     stemp = pow(stemp, exponent); 
     vec4 specular = vec4(stemp);

     //calculate fresnel and inverted fresnel  
     vec4 invfres = vec4( dot(vNorm, viewt) );
     vec4 fres = vec4(1.0) -invfres ;

     //calculate reflection and refraction
     refr *= invfres;
     refr *= invdepth;
     temp = waterColor * wdepth * invfres;
     refr += temp;
     refl *= fres;

     //add reflection and refraction
     tmp = refr + refl;
     vec4 tfinal = tmp + specular;   
	  gl_FragColor = tfinal;
	  
	 	  */
	  
	  
	  /*
	  vec2 projcoord = ReflectionMapSamplingPos.xy / ReflectionMapSamplingPos.z;
	  vec4 bumpColor = texture2D(water_normalmap, BumpMapSamplingPos);
	  vec4 bumpColor2 = texture2D(water_normalmap, RefractionMapSamplingPos);
	  vec4 depthColor = texture2D(water_depthmap,texin);
	  //vec4 depthColor = vec4(0.0,0.2,0.2,0.2);
		
	  vec2 perturbation = (bumpColor.rg - 0.5);
	  vec2 perturbation2 = bumpColor2.rg - 0.5;
	
	 
	  vec2 perturbatedTexCoords = projcoord + perturbation / 5.0 + perturbation2 / 5.0;
	  perturbatedTexCoords = clamp(perturbatedTexCoords, 0.001, 0.999);
	  
	 
	  vec3 reflectionColor = texture2D(water_reflection, perturbatedTexCoords).rgb;
	  reflectionColor = clamp(reflectionColor, 0.0, 1.0);
	  
	  
	  vec3 dullColor = vec3(0.1, 0.4, 0.4);
	  float dullBlendFactor = (0.5 - depthColor.r);// / 2.0;
	  
	 // vec3 finalColor = dullBlendFactor * dullColor + (1.0 - dullBlendFactor) * (reflectionColor.rgb);
	   vec3 finalColor = dullBlendFactor * dullColor + (1.0 - dullBlendFactor) * (reflectionColor.rgb);
	   
	  float z = gl_FragCoord.z / gl_FragCoord.w;
	  
	  float fogFactor = (gl_Fog.end - z ) * (gl_Fog.end/ gl_Fog.start);
	  fogFactor = clamp(fogFactor, 0.0, 1.0);
	  vec4 finalWithFog = mix(gl_Fog.color, vec4(finalColor, 1.0), fogFactor);

	  //vec3 finalWithFog = finalColor * fog  + (gl_Fog.color.rgb) * (1.0 - fog);
	  gl_FragColor = vec4(finalWithFog.rgb ,1.0 - depthColor.r);
	  
	  */
	  
	  
	  //WATER ATTEMPT 3===================================
		
	
	  vec2 projcoord = ReflectionMapSamplingPos.xy / ReflectionMapSamplingPos.z;
	  vec4 bumpColor = texture2D(water_normalmap, BumpMapSamplingPos);
	  vec4 bumpColor2 = texture2D(water_normalmap, RefractionMapSamplingPos);
	  
	  
	  vec4 reflect = texture2D(water_reflection, projcoord);
	  vec4 depthColor = texture2D(water_depthmap,texin);
	  reflect = clamp(reflect, 0.0, 1.0);
	  
	  vec2 perturbation = (bumpColor.rg - 0.5);
	  vec2 perturbation2 = bumpColor2.rg - 0.5;
	
	  float perturbFactor = 10.0;
	  vec2 perturbatedTexCoords = projcoord + perturbation / perturbFactor + perturbation2 / perturbFactor;
	  perturbatedTexCoords = clamp(perturbatedTexCoords, 0.001, 0.999);
	
	  vec3 reflectionColor = texture2D(water_reflection, perturbatedTexCoords).rgb;
	  vec3 finalColor = (reflectionColor.rgb + reflect.rgb);
	  
	  
		  
	  
	  
	  gl_FragColor = vec4(reflectionColor.rgb, 1.0 - depthColor.r);
}
