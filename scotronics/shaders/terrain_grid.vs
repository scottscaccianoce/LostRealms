
varying vec3 posW;

void main(void)
{
   posW = gl_Vertex.xyz;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}       