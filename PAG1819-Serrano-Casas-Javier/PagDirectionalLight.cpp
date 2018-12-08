#include "PagDirectionalLight.h"

PagDirectionalLight::PagDirectionalLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is,
	glm::vec3 _direction, float _c1, float _c2, float _c3)
	: Id(_Id), Is(_Is), direction(_direction), c1(_c1), c2(_c2), c3(_c3) {
}

PagDirectionalLight::~PagDirectionalLight() {
}

void PagDirectionalLight::execute(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 directionView = glm::vec3(camera.getMatrixView() * glm::vec4(direction, 0.0));
	ads.setUniform("lightDirection", glm::normalize(directionView));
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);

	// Estas variables no las utilizare pero necesito pasarlas por las subrutinas
	ads.setUniform("lightPosition", glm::vec3());
	ads.setUniform("Ia", glm::vec3());
	ads.setUniform("cosGamma", 0.0f);
	ads.setUniform("exponent", 0.0f);
}

LightType PagDirectionalLight::getType() {
	return PAG_DIRECTIONAL;
}