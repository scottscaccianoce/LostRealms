

#ifndef PARTICLE_SYSTEM__
#define PARTICLE_SYSTEM__

#include "math3d.h"
#include "shader.h"
#include <vector>
struct Particle {
	M3DVector3f starting_pos; //use vertex
	M3DVector3f dir; //use normal
	M3DVector4f color; //use texcoord
	
};

class Emitter {
	
public:
	virtual ~Emitter() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual bool IsActive();

	void SetEmitterPos(M3DVector3f emitter_loc);
	
	
protected:
	M3DVector3f emitter_pos;
	Shader *particleShader;
	float elapsed_time;
	float max_time;
	std::vector<Particle> particles;
	int number_of_particles;
	
	
};

#endif