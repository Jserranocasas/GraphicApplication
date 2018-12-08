#include "PagAmbientLight.h"


PagAmbientLight::PagAmbientLight(glm::vec3 _Ia) : Ia(_Ia){
}

PagAmbientLight::~PagAmbientLight()
{
}

// - Aplica los parámetros de la fuente luminosa enviando los uniform correspondientes
void PagAmbientLight::apply(PagShaderProgram& ads, PagCamera camera) {
	ads.setUniform("Ia", Ia);

	// Estas variables no las utilizare pero necesito pasarlas por las subrutinas
	ads.setUniform("lightDirection", glm::vec3());
	ads.setUniform("lightPosition", glm::vec3());
	ads.setUniform("cosGamma", 0.0f);
	ads.setUniform("exponent", 0.0f);
	ads.setUniform("Id", glm::vec3());
	ads.setUniform("Is", glm::vec3());
	ads.setUniform("c1", 1.0f);
	ads.setUniform("c2", 0.0f);
	ads.setUniform("c3", 0.0f);
}

LightType PagAmbientLight::getType() {
	return PAG_AMBIENT;
}