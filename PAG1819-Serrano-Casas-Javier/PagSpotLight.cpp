#include "PagSpotLight.h"
#include "gtc/constants.hpp"

/* - Constructor parametrizado para PagSpotLight*/
PagSpotLight::PagSpotLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _direction, glm::vec3 _position, 
						float _exponent, float _angle, float _c1, float _c2, float _c3)
	: Id(_Id), Is(_Is), direction(_direction), position(_position), exponent(_exponent), angle(_angle),
	c1(_c1), c2(_c2), c3(_c3) {
}

PagSpotLight::~PagSpotLight() {
}

// - Aplica los parámetros de la fuente luminosa enviando los uniform correspondientes
void PagSpotLight::apply(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 positionView = glm::vec3(camera.getMatrixView() * glm::vec4(position, 1.0));
	glm::vec3 directionView = glm::vec3(camera.getMatrixView() * glm::vec4(direction, 0.0));

	float gamma = cos(angle * 180.0 / glm::pi<float>());

	ads.setUniform("lightPosition", positionView);
	ads.setUniform("lightDirection", glm::normalize(directionView));
	ads.setUniform("exponent", exponent);
	ads.setUniform("cosGamma", gamma);
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);

	// La Iluminacion ambiental no la utilizare pero necesito pasarla por las subrutinas
	ads.setUniform("Ia", glm::vec3(0.0, 0.0, 0.0));
}

LightType PagSpotLight::getType() {
	return PAG_SPOT;
}