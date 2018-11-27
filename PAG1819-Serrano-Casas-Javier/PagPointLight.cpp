#include "PagPointLight.h"


PagPointLight::PagPointLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _position,
					float _c1, float _c2, float _c3, LightType l)
	: Ia(_Ia), Id(_Id), Is(_Is), position(_position), c1(_c1), c2(_c2), c3(_c3) {
	type = l;
}

PagPointLight::~PagPointLight() {
}

void PagPointLight::execute(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 positionView = glm::vec3(camera.getMatrixView() * glm::vec4(position, 1.0));
	ads.setUniform("lightPosition", positionView);
	ads.setUniform("direction", glm::vec3(0.0, 0.0, 0.0));
	ads.setUniform("gamma", 0.0f);
	ads.setUniform("s", 0.0f);
	ads.setUniform("Ia", Ia);
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);
}

LightType PagPointLight::getType() {
	return type;
}