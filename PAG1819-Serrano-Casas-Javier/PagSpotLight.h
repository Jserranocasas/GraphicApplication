#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

class PagSpotLight : public PagLightApplication {
public:
	PagSpotLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _direction, glm::vec3 _position, 
							float _exponent, float _angle, float _c1, float _c2, float _c3, LightType l);
	virtual ~PagSpotLight();

	virtual void execute(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
private:
	glm::vec3 Ia, Id, Is;
	glm::vec3 direction;
	glm::vec3 position;
	float exponent, angle;
	float c1, c2, c3;
	LightType type;
};