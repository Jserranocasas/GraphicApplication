#pragma once

#include <vector>
#include <glm.hpp>

/*
* @file		PagSubdivisionProfile.h
* @brief	Clase que representa un perfil de subdivisión
* @author	Javier Serrano Casas
* @date		16-10-2018
*/
class PagSubdivisionProfile {
private:
	PagSubdivisionProfile subdivision(PagSubdivisionProfile profile);
	std::vector<glm::vec2> points;	// Vector con los puntos del perfil
	bool top, bottom;				// Tapas superior e inferior del perfil
public:
	PagSubdivisionProfile();
	PagSubdivisionProfile(std::vector<glm::vec2> _points);
	~PagSubdivisionProfile();

	PagSubdivisionProfile& operator=(const PagSubdivisionProfile &orig);
	PagSubdivisionProfile subdivide(unsigned int times);

	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec2> getPositions();
	glm::vec2 getPosition(int index);
	
	bool existBottom();
	bool existTop();
	int size();
};