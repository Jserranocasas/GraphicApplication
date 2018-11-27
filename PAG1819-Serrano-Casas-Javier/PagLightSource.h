#pragma once

#include "PagLightApplication.h"

class PagLightSource {
private:
	PagLightApplication *_strategy;

public:
	PagLightSource(PagLightApplication *strategy);
	PagLightSource& operator=(const PagLightSource& orig);
	~PagLightSource();

	void set_strategy(PagLightApplication *strategy);
	void execute(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
};