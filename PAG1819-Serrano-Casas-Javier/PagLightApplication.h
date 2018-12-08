#pragma once

#include "glm.hpp"
#include "PagCamera.h"
#include "PagEnumerations.h"
#include "PagShaderProgram.h"

/*
 * @brief	Clase abstracta para cada tipo de fuente luminosa
 * @file	PagLightApplication.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagLightApplication {
public:
	virtual void apply(PagShaderProgram& ads, PagCamera camera) = 0;
	virtual LightType getType() = 0;
};