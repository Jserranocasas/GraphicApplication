#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

/*
 * @brief	Clase que representa una fuente luminosa ambiental
 * @file	PagAmbientLight.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagAmbientLight : public PagLightApplication {
public:
	PagAmbientLight(glm::vec3 _Ia);
	virtual ~PagAmbientLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	glm::vec3 Ia;	// Iluminación ambiental
};
