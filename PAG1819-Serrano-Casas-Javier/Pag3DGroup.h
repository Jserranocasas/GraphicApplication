#pragma once

#include <vector>
#include "PagPlane.h"
#include "Pag3DElement.h"
#include "PagRevolutionObject.h"

class Pag3DGroup : public Pag3DElement {
public:
	Pag3DGroup();
	~Pag3DGroup();

	Pag3DGroup& operator=(const Pag3DGroup& orig);
	
	void insert3DElement(PagPlane *element);
	void insert3DElement(Pag3DGroup *element);
	void insert3DElement(PagRevolutionObject *element);

	void drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v);
	void drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject);

private:
	std::vector<Pag3DElement*> elements;
};