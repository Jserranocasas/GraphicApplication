#pragma once

#include <vector>
#include <stdlib.h>
#include "PagCamera.h"
#include "Pag3DGroup.h"
#include "PagSpotLight.h"
#include "PagPointLight.h"
#include "PagStructures.h"
#include "PagLightSource.h"
#include "PagEnumerations.h"
#include "PagShaderProgram.h"
#include "PagRevolutionObject.h"
#include "PagDirectionalLight.h"

/*
 * @file	PagRenderer.h
 * @brief	Clase PagRenderer que encapsula toda la gestión del área de dibujo
 *			OpenGL, eventos, etc. Se implementa como un singleton para que pueda 
 *			ser llamada desde las funciones callback del main.cpp
 * @author	Javier Serrano Casas
 * @date	03-10-2018
 */
class PagRenderer {
public:
	~PagRenderer();

	// - Será llamado cada vez que haya que redibujar la escena.
	void refreshCallback();

	// - Métodos de instancia para responder a los demás eventos
	void mouseButtonCallback(int button, int action, int mods, const int press, const int release);
	void keyCallback(int key, int scancode, int action, int mods);
	void framebufferSizeCallback(int width, int height);
	void scrollCallback(double xoffset, double yoffset);
	void cursorPosCallback(double xpos, double ypos);

	double* getCursorX();
	double* getCursorY();

	// - Método para acceder al singleton. Cada vez que se necesite llamar 
	// al renderer se hará a través de este método.
	static PagRenderer *getInstance();

	// - Este método prepara la escena para su visualización
	void prepareOpenGL();
private:
	// - Constructor es privado para impedir que se puedan construir nuevos
	// renderers aparte del singleton
	PagRenderer();

	// - Singleton. Única instancia de la clase PagRenderer que se tiene 
	// en la aplicación.
	static PagRenderer *instance;

	// - Camara virtual con las coordenadas de modelado, vision y proyeccion
	PagCamera *camera;

	// - Objetos de revolución de la escena
	Pag3DGroup *objects; 

	// - Shader program que se usa para dibujar la geometría
	PagShaderProgram *pointShader;
	PagShaderProgram *linesShader;
	PagShaderProgram *ADSShader;
	PagShaderProgram *debugShader;

	// - Booleanos para conocer el modo de pintar
	bool points, lines, tmesh, debug;

	// - Entero con el modo de debug
	int debugOption;

	// - Posición del cursor para el movimiento de la cámara
	double cursorX, cursorY;

	// - Identificador del ShaderAds
	int idAds;

	// - Luces de la escena
	std::vector<PagLightSource*> light;
};