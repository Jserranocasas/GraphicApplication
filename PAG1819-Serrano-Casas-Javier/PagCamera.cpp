#include <iostream>
#include "PagCamera.h"
#include <glm.hpp>
#include "gtc/epsilon.hpp"
#include <gtc\matrix_transform.hpp>

// - Constructor por defecto
PagCamera::PagCamera() : position(glm::vec3(30.0,30.0,0.0)), interest(glm::vec3(0.0, 0.0, 0.0)), 
						up(glm::vec3(0.0, 1.0, 0.0)), fovY(50), zNear(0.1), zFar(200)
{
	calculateNVU();

	width = PAG_INIT_WIDTH;
	height = PAG_INIT_HEIGHT;
	mode = PAG_WITHOUT_MOVE;
}

PagCamera::PagCamera(glm::vec3 _position, glm::vec3 _interest, glm::vec3 _up, float _fovY, float _zNear, float _zFar) 
	: position(_position), interest(_interest), up(_up), fovY(_fovY), zNear(_zNear), zFar(_zFar)
{
	calculateNVU();

	width = PAG_INIT_WIDTH;
	height = PAG_INIT_HEIGHT;
	mode = PAG_WITHOUT_MOVE;
}

PagCamera::~PagCamera() {
}

/* --------------------------------------------------------------- */
/* ------------------- Métodos getters y setters ----------------- */
/* --------------------------------------------------------------- */
void PagCamera::setView(glm::vec3 _pos, glm::vec3 _r, glm::vec3 _vUp) {
	position = _pos;
	interest = _r;
	up = _vUp;

	calculateNVU();
}

void PagCamera::setViewport(float _width, float _height) {
	width = _width;
	height = _height;
}

void PagCamera::setMode(CameraMovement _mode) {
	mode = _mode;
}

void PagCamera::setFovY(float _fovY) {
	if (_fovY > 0.1 && _fovY < 180.0) {
		fovY = _fovY;
	}
}

CameraMovement PagCamera::getMode() {
	return this->mode;
}

float PagCamera::getFovY() {
	return this->fovY;
}

/* ------------------------------------------------------------------- */
/* ------------------------ Métodos públicos ------------------------- */
/* ------------------------------------------------------------------- */

// - Devuelvo la matriz de modelado vision y proyección de la camara virtual.
glm::mat4x4 PagCamera::getMatrixViewProject() {
	glm::mat4 projection = glm::perspective(glm::radians(fovY), width / height, zNear, zFar);;

	return projection * getMatrixView();
}

// - Devuelvo la matriz de modelado vision
glm::mat4x4 PagCamera::getMatrixView() {
	calculateNVU();

	glm::mat4x4 view = glm::lookAt(position, interest, up);

	return view;
}

// - Método para representar un movimiento de cámara orbital alrededor de un objeto 
void PagCamera::executeOrbit(double xpos, double cursorX, double ypos, double cursorY) {
	glm::mat4x4 translateOrigin = glm::translate(glm::mat4(1.0f), -interest);
	glm::mat4x4 translate = glm::translate(glm::mat4(1.0f), interest);
	glm::mat4x4 rotate;

	if (xpos < cursorX) {
		rotate = glm::rotate(glm::mat4(1.0f), 0.01f, v);
	} else {
		rotate = glm::rotate(glm::mat4(1.0f), -0.01f, v);
	}

	glm::mat4x4 M = translate * rotate * translateOrigin;

	position = glm::vec3(M * glm::vec4(position, 1.0));
}

// - Método para representar un movimiento de la cámara donde la cámara cambia en el eje x el punto donde mira 
void PagCamera::executePan(double xpos, double cursorX, double ypos, double cursorY) {

	glm::mat4x4 translateOrigin = glm::translate(glm::mat4(1.0f), -position);
	glm::mat4x4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4x4 rotate;

	if (xpos < cursorX) {
		rotate = glm::rotate(glm::mat4(1.0f), 0.005f, v);
	}
	else {
		rotate = glm::rotate(glm::mat4(1.0f), -0.005f, v);
	}

	glm::mat4x4 M = translate * rotate * translateOrigin;

	position = glm::vec3(M*glm::vec4(position, 1.0));
	interest = glm::vec3(M*glm::vec4(interest, 1.0));
}

// - Método para representar un movimiento de la cámara donde la cámara se mueve hacia adelante o hacia detrás
void PagCamera::executeTruck(double xpos, double cursorX, double ypos, double cursorY) {
	
	if (xpos < cursorX) {
		position = position + (u * 0.5f);
		interest = interest + (u * 0.5f);
	}
	else {
		position = position - (u * 0.5f);
		interest = interest - (u * 0.5f);
	}
}

// - Método para representar un movimiento de la cámara donde la cámara se mueve lateralmente
void PagCamera::executeDolly(double xpos, double cursorX, double ypos, double cursorY) {

	if (ypos > cursorY) {
		position = position + (n * 0.5f);
		interest = interest + (n * 0.5f);
	}
	else {
		position = position - (n * 0.5f);
		interest = interest - (n * 0.5f);
	}
}

// - Método para representar un movimiento de la cámara donde la cámara se mueve hacia arriba o abajo
void PagCamera::executeCrane(double xpos, double cursorX, double ypos, double cursorY) {

	if (ypos > cursorY) {
		position = position + (v * 0.5f);
		interest = interest + (v * 0.5f);
	}
	else {
		position = position - (v * 0.5f);
		interest = interest - (v * 0.5f);
	}
}

// - Calcula los vectores unitarios n, v, u
void PagCamera::calculateNVU() {
	n = glm::normalize(position - interest);
	
	if (glm::epsilonEqual<double>(glm::abs(n.y), up.y, 0.009)) {		
		if (up.y < 0.0) {
			u = glm::cross(glm::vec3(0.0, 0.0, -1.0), n);
		} 
		else {
			u = glm::cross(glm::vec3(0.0, 0.0, 1.0), n);
		}
	} 
	else {
		u = glm::cross(up, n);
	}

	u = glm::normalize(u);
	v = glm::cross(n, u);
}