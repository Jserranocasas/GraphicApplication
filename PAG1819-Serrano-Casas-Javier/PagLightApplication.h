#pragma once

#include "glm.hpp"
#include "PagCamera.h"
#include "PagEnumerations.h"
#include "PagShaderProgram.h"

class PagLightApplication {
public:
	virtual void execute(PagShaderProgram& ads, PagCamera camera) = 0;
	virtual LightType getType() = 0;
};