#include "PagDirectionalLight.h"

PagDirectionalLight::PagDirectionalLight(glm::vec3 _Ia, glm::vec3 _Id, glm::vec3 _Is,
	glm::vec3 _direction, float _c1, float _c2, float _c3, LightType l)
	: Ia(_Ia), Id(_Id), Is(_Is), direction(_direction), c1(_c1), c2(_c2), c3(_c3) {
	type = l;
}

PagDirectionalLight::~PagDirectionalLight() {
}

void PagDirectionalLight::execute(PagShaderProgram& ads, PagCamera camera) {
	glm::vec3 directionView = glm::vec3(camera.getMatrixView() * glm::vec4(direction, 0.0));
	ads.setUniform("lightPosition", glm::vec3(0.0, 0.0, 0.0));
	ads.setUniform("direction", directionView);
	ads.setUniform("gamma", 0.0f);
	ads.setUniform("s", 0.0f);
	ads.setUniform("Ia", Ia);
	ads.setUniform("Id", Id);
	ads.setUniform("Is", Is);
	ads.setUniform("c1", c1);
	ads.setUniform("c2", c2);
	ads.setUniform("c3", c3);
}

LightType PagDirectionalLight::getType() {
	return type;
}