#include "particle_system.h"


// EMITTER

void Emitter::SetEmitterPos(M3DVector3f emitter_loc) {
	m3dCopyVector3(emitter_pos, emitter_loc);
}

bool Emitter::IsActive() {
	return elapsed_time < max_time;
}

Emitter::~Emitter() {}