#pragma once

#include "PagLightApplication.h"

/*
 * @brief	Clase que representa un patron de diseño strategy
 * @file	PagLightSource.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagLightSource {
private:
	PagLightApplication *_strategy;

public:
	PagLightSource(PagLightApplication *strategy);
	PagLightSource& operator=(const PagLightSource& orig);
	~PagLightSource();

	void set_strategy(PagLightApplication *strategy);
	void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
};