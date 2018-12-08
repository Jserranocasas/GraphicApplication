#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

class PagAmbientLight : public PagLightApplication {
public:
	PagAmbientLight(glm::vec3 _Ia);
	virtual ~PagAmbientLight();

	virtual void execute(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	glm::vec3 Ia;
};
