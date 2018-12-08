#include "PagPointLight.h"


PagPointLight::PagPointLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _position, float _c1, float _c2, float _c3)
	: Id(_Id), Is(_Is), position(_position), c1(_c1), c2(_c2), c3(_c3) {
}

PagPointLight::~PagPointLight() {
}

// - Aplica los parámetros de la fuente luminosa enviando los uniform correspondientes
void PagPointLight::apply(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 positionView = glm::vec3(camera.getMatrixView() * glm::vec4(position, 1.0));
	ads.setUniform("lightPosition", positionView);
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);

	// Estas variables no las utilizare pero necesito pasarlas por las subrutinas
	ads.setUniform("lightDirection", glm::vec3());
	ads.setUniform("cosGamma", 0.0f);
	ads.setUniform("Ia", glm::vec3());
	ads.setUniform("exponent", 0.0f);
}

LightType PagPointLight::getType() {
	return PAG_POINT;
}