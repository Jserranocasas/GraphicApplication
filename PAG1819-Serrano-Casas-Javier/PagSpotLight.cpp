#include "PagSpotLight.h"


/* - Constructor parametrizado para PagSpotLight*/
PagSpotLight::PagSpotLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _direction, glm::vec3 _position, 
									float _exponent, float _angle, float _c1, float _c2, float _c3, LightType l)
	: Ia(_Ia), Id(_Id), Is(_Is), direction(_direction), position(_position), exponent(_exponent), angle(_angle),
	c1(_c1), c2(_c2), c3(_c3) {
	type = l;
}

PagSpotLight::~PagSpotLight() {
}

void PagSpotLight::execute(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 positionView = glm::vec3(camera.getMatrixView() * glm::vec4(position, 1.0));
	glm::vec3 directionView = glm::vec3(camera.getMatrixView() * glm::vec4(direction, 0.0));

	ads.setUniform("lightPosition", positionView);
	ads.setUniform("direction", directionView);
	ads.setUniform("gamma", angle);
	ads.setUniform("s", exponent);
	ads.setUniform("Ia", Ia);
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);
}

LightType PagSpotLight::getType() {
	return type;
}