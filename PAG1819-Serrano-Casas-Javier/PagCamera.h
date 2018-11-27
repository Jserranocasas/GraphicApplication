#pragma once

#include "glm.hpp"
#include "PagEnumerations.h"

/*
* @brief Clase que representa una cámara virtual
* @file	PagCamera.h
* @author Javier Serrano Casas
* @date	14-11-2018
*/
class PagCamera {
public:
	PagCamera();
	PagCamera(glm::vec3 _position, glm::vec3 _interest, glm::vec3 _up, float _fovY, float _zNear, float _zFar);
	~PagCamera(); 

	// - Métodos públicos
	glm::mat4x4 getMatrixView();
	glm::mat4x4 getMatrixViewProject();
	void executePan(double xpos, double cursorX, double ypos, double cursorY);
	void executeOrbit(double xpos, double cursorX, double ypos, double cursorY);
	void executeTruck(double xpos, double cursorX, double ypos, double cursorY);
	void executeDolly(double xpos, double cursorX, double ypos, double cursorY);
	void executeCrane(double xpos, double cursorX, double ypos, double cursorY);
	
	// - Métodos Getters y Setters
	void setView(glm::vec3 _pos, glm::vec3 _r, glm::vec3 _vUp);
	void setViewport(float _width, float _height);
	void setMode(CameraMovement _mode);
	void setFovY(float _fovY);
	CameraMovement getMode();
	float getFovY();

private:
	void calculateNVU();

	float width, height;		// Altura y anchura del viewport
	float fovY, zNear, zFar;	// Ángulo de visión, z cercano y z lejano
	CameraMovement mode;		// Modo de la API

	glm::vec3 interest;			// Punto que situa la posición del punto de interés
	glm::vec3 position;			// Punto que situa la posición de la cámara
	glm::vec3 up;				// Vector hacia arriba

	glm::vec3 u;				// Vector u de la cámara
	glm::vec3 v;				// Vector v de la cámara
	glm::vec3 n;				// Vector n de la cámara
};
