
#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

class PagDirectionalLight : public PagLightApplication {
public:
	PagDirectionalLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is,
		glm::vec3 _direction, float _c1, float _c2, float _c3);
	virtual ~PagDirectionalLight();

	virtual void execute(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	float c1, c2, c3;
	glm::vec3 Ia, Id, Is;
	glm::vec3 direction;
};