#include "util.h"




/*sVector3 sVector3 ( GLfloat x, GLfloat y, GLfloat z )
{
	sVector3 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return temp;
}
*/

sVector3 AddsVectors3 ( sVector3 * u, sVector3 * v){
	sVector3 result;
	result.x = u->x + v->x;
	result.y = u->y + v->y;
	result.z = u->z + v->z;
	return result;
	
}

void AddVector3ToVector ( sVector3 * Dst, sVector3 * V2){
	Dst->x += V2->x;
	Dst->y += V2->y;
	Dst->z += V2->z;
	
	
}

float Dot( sVector3 *u, sVector3 * v)
{
	return u->x * v->x + u->y * v->y + u->z * v->z;
	
}
sVector3 Cross( sVector3 *u, sVector3 * v)
{
	sVector3 result;
	result.x = u->y * v->z - u->z * v->y;
	result.y = u->z * v->x - u->x * v->z;
	result.z = u->x * v->y - u->y * v->x;
	return result;
}

sVector3 SubsVectors3 ( sVector3 * v, sVector3 * u)
{
	sVector3 result;
	result.x = v->x - u->x;
	result.y = v->y - u->y;
	result.z = v->z - u->z;
	return result;
	
	
}
void Normalize(sVector3 &v)
{
	float x,y,z, mag;
	
	x = v.x;
	y = v.y;
	z = v.z;
	
	mag = sqrt(x*x + y*y + z * z);
	
	x/= mag;
	y/= mag;
	z/= mag;
	
	v.x = x;
	v.y = y;
	v.z = z;
	
	
	
}

bool CheckLineTri( sVector3 TP1, sVector3 TP2, sVector3 TP3, sVector3 LP1, sVector3 LP2, sVector3 & HitPos)
{
	sVector3 Normal, IntersectPos;
	
	//Find triangle normal
	sVector3 line1 = SubsVectors3(&TP2,&TP1);
	sVector3 line2 = SubsVectors3(&TP3,&TP1);
	
	Normal = Cross(&line1, &line2);
	Normalize(Normal);
	//find distance from LP1 and LP2 to the plane
	sVector3 L1T1 = SubsVectors3(&LP1, &TP1);
	sVector3 L2T1 = SubsVectors3(&LP2, &TP1);
	
	
	float Dist1 = Dot(&L1T1,&Normal);
	float Dist2 = Dot(&L2T1,&Normal);
	if( (Dist1 * Dist2) >= 0.0f) return false; //line doen't cross
	if( Dist1 == Dist2) return false; //parallel
	
	
	//IntersectPos = LP1 + (LP2 - LP1) * (-Dist1/(Dist2-Dist1));
	
	sVector3 diff = SubsVectors3(&LP2, &LP1);
	float mult = -Dist1/(Dist2-Dist1);
	diff.x *= mult;
	diff.y *= mult;
	diff.z *= mult;
	sVector3 sum = AddsVectors3(&LP1, &diff);
	
	IntersectPos.x = sum.x;
	IntersectPos.y = sum.y;
	IntersectPos.z = sum.z;
	
	//check if intersect
	sVector3 vtest;
	sVector3 temp,temp2;
	
	temp = SubsVectors3(&TP2, &TP1);
	temp2 = SubsVectors3(&IntersectPos, &TP1);
	vtest = Cross(&Normal,&temp);
	if(Dot(&vtest,&temp2) < 0.0f) return false;
	
	temp = SubsVectors3(&TP3, &TP2);
	temp2 = SubsVectors3(&IntersectPos, &TP2);
	vtest = Cross(&Normal,&temp);
	if(Dot(&vtest,&temp2) < 0.0f) return false;
	
	temp = SubsVectors3(&TP1, &TP3);
	temp2 = SubsVectors3(&IntersectPos, &TP1);
	vtest = Cross(&Normal,&temp);
	if(Dot(&vtest,&temp2) < 0.0f) return false;
	
	HitPos = IntersectPos;
	return true;
	
	
	


}

void GetMouseRay(sVector3 &pos1, sVector3  &pos2, sVector3 &dir) {
	int myX, myY;
	glfwGetMousePos(&myX,&myY);
	
	gCamera.Render();
	
	double mvmatrix[16];
	double projmatrix[16];
	int viewport[4];
	double dx,dy,dz;
	
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	
	double mouseY = viewport[3] - myY;
	
	gluUnProject((double)myX, (double)mouseY, 0.0, mvmatrix, projmatrix, viewport, &dx, &dy, &dz);
	pos1 = sVector3((float)dx,(float)dy,(float)dz);
	
	gluUnProject((double)myX, (double)mouseY, 1.0, mvmatrix, projmatrix, viewport, &dx, &dy, &dz);
	pos2  = sVector3((float)dx,(float)dy,(float)dz);
	
	float x,y,z, mag;
	
	x = pos2.x - pos1.x;
	y = pos2.y - pos1.y;
	z = pos2.z - pos1.z;
	
	mag = sqrt(x*x + y*y + z * z);
	
	x/= mag;
	y/= mag;
	z/= mag;
	dir = sVector3(x,y,z);
	
	
}

bool RaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius)
{
	//m3dNormalizeVectorf(ray);	// Make sure ray is unit length
	
	
	
	M3DVector3f rayToCenter;	// Ray to center of sphere
	rayToCenter[0] =  sphereCenter[0] - point[0];	
	rayToCenter[1] =  sphereCenter[1] - point[1];
	rayToCenter[2] =  sphereCenter[2] - point[2];
	
	// Project rayToCenter on ray to test
	float a = m3dDotProduct3(rayToCenter, ray);
	
	// Distance to center of sphere
	float distance2 = m3dDotProduct3(rayToCenter, rayToCenter);	// Or length
	
	float dRet = (sphereRadius * sphereRadius) - distance2 + (a*a);
	
	if(dRet > 0.0)			// Return distance to intersection
		return true;
		dRet = a - sqrtf(dRet);
		
	
	
	return false;
}


void report_gl_error(const char* file, int line_number)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		//log("ERROR: OGL %s %d %s\n", file, line_number, gluErrorString(error));
	}
}

std::string IntToString(int number) {
	return  static_cast<std::ostringstream*>( &(std::ostringstream() << number) )->str();
}

std::string FloatToString(float number) {
	return  static_cast<std::ostringstream*>( &(std::ostringstream() << number) )->str();
}


// Generate a random number between 0 and 1
// return a uniform number in [0,1].
float unifRand()
{
    return rand() / float(RAND_MAX);
}
//
// Generate a random number in a real interval.
// param a one end point of the interval
// param b the other end of the interval
// return a inform rand numberin [a,b].
float unifRand(float a, float b)
{
    return (b-a)*unifRand() + a;
}

// Reset the random number generator with the system clock.
void seed()
{
    srand(time(0));
}

#define GLE report_gl_error(__FILE__, __LINE__);