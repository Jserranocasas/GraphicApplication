#include <glm.hpp>
#include <fstream>
#include <iostream>
#include <GL\glew.h>

#include "PagPlane.h"
#include "PagRenderer.h"
#include "Pag3DGroup.h"

// - Inicializa el singleton.
PagRenderer * PagRenderer::instance = NULL;

// -----------------------  Métodos constructores  ---------------------
PagRenderer::PagRenderer() : debug(false), lines(false), tmesh(true), points(false), debugOption (0) {
}

PagRenderer::~PagRenderer() {
	delete camera;
	delete objects;
	delete pointShader;
	delete linesShader;
	delete ADSShader;
	delete debugShader;
}

// - Acceder al singleton.
// Si es la primera vez que se llama a este método de clase
// instance es NULL y se construye. Es lo que se llama inicialización
// perezosa. Sólo se inicializa cuando se necesita.
PagRenderer * PagRenderer::getInstance() {
	if (!instance) {
		instance = new PagRenderer();
	}
	return instance;
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void PagRenderer::refreshCallback() {
	std::cout << "Refresh callback called" << std::endl;

	// - Borra los buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (points && lines && tmesh) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.8f, 0.5f);
	}

	if (points) {	// - Si dibujas como puntos activado

		glDisable(GLenum(GL_BLEND));
		glDepthFunc(GLenum(GL_LESS));

		// - Activamos el shader program que se va a usar.
		pointShader->use();

		// - Asignamos los parámetros de tipo uniform del shader program.
		// - Para cada objeto de la escena.
		pointShader->setUniform("pointSize", 5.0f);
		pointShader->setUniform("vColor", glm::vec3(1.0f, 0.0f, 0.0f));

		// - Dibujo el objeto como puntos
		objects->drawAsPoints(pointShader, camera->getMatrixViewProject());
	}

	if (lines) {	// - Si dibujas como lineas activado

		glDisable(GLenum(GL_BLEND));
		glDepthFunc(GLenum(GL_LESS));

		// - Activamos el shader program que se va a usar.
		linesShader->use();

		// - Asignamos los parámetros de tipo uniform del shader program.
		linesShader->setUniform("vColor", glm::vec3(0.0f, 0.0f, 1.0f));

		// - Dibujo el objeto como lineas.
		objects->drawAsLines(linesShader, camera->getMatrixViewProject());
	}

	if (tmesh) {	// - Si dibujar como triángulos sombreados activado

		glEnable(GLenum(GL_BLEND));
		glDepthFunc(GLenum(GL_LEQUAL));

		// - Activamos el shader program que se va a usar.
		ADSShader->use();

		// - Buscamos el identificador de la subrutina para ads y textura
		GLuint adsId1 = glGetSubroutineIndex(idAds, GL_FRAGMENT_SHADER, "adsDirectional");
		GLuint adsId2 = glGetSubroutineIndex(idAds, GL_FRAGMENT_SHADER, "adsSpot");
		GLuint adsId3 = glGetSubroutineIndex(idAds, GL_FRAGMENT_SHADER, "adsPoint");
		GLuint adsId4 = glGetSubroutineIndex(idAds, GL_FRAGMENT_SHADER, "adsAmbient");

		bool firstlight = true;

		// - Realizamos multipass rendering
		for (unsigned int i = 0; i < light.size(); i++) {
			if (firstlight) {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstlight = false;
			}
			else {
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			// - Activamos las subrutinas
			if (light[i]->getType() == PAG_DIRECTIONAL) {
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &adsId1);
			}

			if (light[i]->getType() == PAG_SPOT) {
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &adsId2);
			}

			if (light[i]->getType() == PAG_POINT) {
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &adsId3);
			}

			if (light[i]->getType() == PAG_AMBIENT) {
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &adsId4);
			}

			// - Dibujar la escena completa
			light[i]->execute(*ADSShader, *camera);

			// - Dibujo el objeto como triángulos sombreados.
			objects->drawAsTriangles(ADSShader, camera->getMatrixViewProject(), camera->getMatrixView());
		}
	}

	if (debug) {	// - Si dibujar modo debug

		glDisable(GLenum(GL_BLEND));
		glDepthFunc(GLenum(GL_LESS));

		// - Activamos el shader program que se va a usar.
		debugShader->use();
	
		// - Asignamos los parámetros de tipo uniform del shader program. 
		debugShader->setUniform("debugOption", debugOption);

		// - Dibujo el objeto como debug.
		objects->drawAsTriangles(debugShader, camera->getMatrixViewProject(), camera->getMatrixView());
	}

}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void PagRenderer::framebufferSizeCallback(int width, int height) {
	glViewport(0, 0, width, height);
	std::cout << "Resize callback called:" << width << " width, " << height << " height." << std::endl;
	camera->setViewport((float)width, (float)height);
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void PagRenderer::keyCallback(int key, int scancode, int action, int mods) {
	std::cout << "Key callback called: " << (char)key << std::endl;

	switch (key) {
	case 'P':
		points = true;
		lines = false;
		tmesh = false;
		debug = false;
		break;
	case 'W':
		points = false;
		lines = true;
		tmesh = false;
		debug = false;
		break;
	case 'T':
		points = false;
		lines = false;
		tmesh = true;
		debug = false;
		break;
	case 'A':
		points = true;
		lines = true;
		tmesh = true;
		debug = false;
		break;
	case 'D':
		points = false;
		lines = false;
		tmesh = false;
		debug = true;
		break;
	case 'Z':
		debugOption = 0;
		break;
	case 'X':
		debugOption = 1;
		break;
	case 'C':
		debugOption = 2;
		break;
	case 'V':
		debugOption = 3;
		break;
	case '1':
		camera->setMode(PAG_WITHOUT_MOVE);
		break;
	case '2':
		camera->setMode(PAG_ORBIT);
		break;
	case '3':
		camera->setMode(PAG_PAN);
		break;
	case '4':
		camera->setMode(PAG_TRUCK);
		break;
	case '5':
		camera->setMode(PAG_DOLLY);
		break;
	case '6':
		camera->setMode(PAG_CRANE);
		break;
	case '7':
		camera->setView(glm::vec3(0.0, 50.0, 60.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		break;
	case '8':
		camera->setView(glm::vec3(0.0, 80.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
		break;
	case '9':
		camera->setView(glm::vec3(60.0, 50.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		break;
	default:
		break;
	}

}

// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void PagRenderer::mouseButtonCallback(int button, int action, int mods, const int press, const int release) {
	if (action == press) {
		std::cout << "Pulsado el boton: " << button << std::endl;
	}

	else if (action == release) {
		std::cout << "Soltado el boton: " << button << std::endl;
	}
}

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
void PagRenderer::scrollCallback(double xoffset, double yoffset) {
	std::cout << "Movida la rueda del raton " << xoffset <<
		" Unidades en horizontal y " << yoffset << " unidades en vertical" << std::endl;

	if (yoffset == 1) {
		camera->setFovY(camera->getFovY() - 1);
	}
	if (yoffset == -1) {
		camera->setFovY(camera->getFovY() + 1);
	}
}

// - Método para gestionar el callback del movimiento de ratón
void PagRenderer::cursorPosCallback(double xpos, double ypos) {
	std::cout << "Desplazamiento raton: [x=" << xpos << ", y=" << ypos << "]" << std::endl;
	bool left = (xpos < cursorX);
	bool up = (ypos > cursorY);

	switch (camera->getMode()) {
	case PAG_ORBIT:
		camera->executeOrbit(xpos, cursorX, ypos, cursorY);
		break;
	case PAG_PAN:
		camera->executePan(xpos, cursorX, ypos, cursorY);
		break;
	case PAG_TRUCK:
		camera->executeTruck(xpos, cursorX, ypos, cursorY);
		break;
	case PAG_DOLLY:
		camera->executeDolly(xpos, cursorX, ypos, cursorY);
		break;
	case PAG_CRANE:
		camera->executeCrane(xpos, cursorX, ypos, cursorY);
		break;
	}

	cursorX = xpos;
	cursorY = ypos;
}

// - Método para inicializar las posiciones del cursor
double* PagRenderer::getCursorX() {
	return &cursorX;
}

double* PagRenderer::getCursorY() {
	return &cursorY;
}

// - Método que prepara la escena para su visualización 
void PagRenderer::prepareOpenGL() {

	// -------------------- Defino la camara de visión -----------------------

	camera = new PagCamera(glm::vec3(0.0, 50.0, 60.0), glm::vec3(0.0, 0.0, 0.0),
								glm::vec3(0.0, 1.0, 0.0), 60.0f, 0.1f, 200.0f);

	objects = new Pag3DGroup();

	// ------------------ Defino los objetos de revolución -------------------
	// ---------------------------------- Peon -------------------------------
	std::vector<glm::vec2> points;

	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(2.49, 0.0));
	points.push_back(glm::vec2(2.5, 0.0));
	points.push_back(glm::vec2(2.5, 0.025));
	points.push_back(glm::vec2(2.4, 0.1));
	points.push_back(glm::vec2(2.3, 0.25));
	points.push_back(glm::vec2(2.4, 0.75));
	points.push_back(glm::vec2(1.15, 2.0));
	points.push_back(glm::vec2(1.175, 2.0));
	points.push_back(glm::vec2(1.1, 2.0));
	points.push_back(glm::vec2(0.95, 2.0));
	points.push_back(glm::vec2(0.95, 2.05));
	points.push_back(glm::vec2(0.5, 3.5));
	points.push_back(glm::vec2(1.0, 5.5));
	points.push_back(glm::vec2(1.025, 5.5));
	points.push_back(glm::vec2(1.05, 5.5));
	points.push_back(glm::vec2(1.125, 5.65));
	points.push_back(glm::vec2(1.125, 5.7));
	points.push_back(glm::vec2(0.825, 5.8));
	points.push_back(glm::vec2(1.275, 6.7));
	points.push_back(glm::vec2(1.0, 7.5));
	points.push_back(glm::vec2(0.0, 7.75));

	PagRevolutionObject *pawn = new PagRevolutionObject(points, 2, 20);
	PagRevolutionObject *pawn2 = new PagRevolutionObject(*pawn);
	PagRevolutionObject *pawn3 = new PagRevolutionObject(*pawn);
	PagRevolutionObject *pawn4 = new PagRevolutionObject(*pawn);

	pawn->setMaterial(glm::vec3(0.5, 0.2, 0.6), glm::vec3(0.4, 0.4, 0.5), 30.0f);
	pawn->translateModel(glm::vec3(5.0, 0.0, 5.0));
	pawn2->setMaterial(glm::vec3(0.8, 0.2, 0.2), glm::vec3(0.6, 0.3, 0.3), 30.0f);
	pawn2->translateModel(glm::vec3(-5.0, 0.0, 5.0));
	pawn3->setMaterial(glm::vec3(0.5, 0.5, 0.2), glm::vec3(0.5, 0.5, 0.3), 30.0f);
	pawn3->translateModel(glm::vec3(5.0, 0.0, -5.0));
	pawn4->setMaterial(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.3, 0.4, 0.3), 30.0f);
	pawn4->translateModel(glm::vec3(-5.0, 0.0, -5.0));

	objects->insert3DElement(pawn);
	objects->insert3DElement(pawn2);
	objects->insert3DElement(pawn3);
	objects->insert3DElement(pawn4);

	// ---------------------------------------------------------------------------
	// ---------------------------------- Batidora -------------------------------

	Pag3DGroup *beater = new Pag3DGroup();

	points.clear();

	points.push_back(glm::vec2(3.49, 0.0));
	points.push_back(glm::vec2(3.5, 0.0));
	points.push_back(glm::vec2(3.5, 0.01));
	points.push_back(glm::vec2(3.3, 2.5));
	points.push_back(glm::vec2(1.2, 5.0));
	points.push_back(glm::vec2(1.2, 5.1));
	points.push_back(glm::vec2(1.2, 22.0));
	points.push_back(glm::vec2(1.2, 22.1));
	points.push_back(glm::vec2(1.19, 22.1));
	points.push_back(glm::vec2(0, 22.1));

	PagRevolutionObject *beaterDown = new PagRevolutionObject(points, 2, 20);

	beaterDown->setMaterial(glm::vec3(0.9, 0.9, 0.9), glm::vec3(0.8, 0.8, 0.8), 5.5);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(1.5, 0.4));
	points.push_back(glm::vec2(1.99, 0.6));
	points.push_back(glm::vec2(2.0, 0.6));
	points.push_back(glm::vec2(2.0, 0.61));
	points.push_back(glm::vec2(2.6, 3.0));
	points.push_back(glm::vec2(3.5, 10.0));
	points.push_back(glm::vec2(3.4, 16.0));
	points.push_back(glm::vec2(1.5, 22.0));
	points.push_back(glm::vec2(1.0, 22.2));
	points.push_back(glm::vec2(0.0, 22.0));

	PagRevolutionObject *beaterUp = new PagRevolutionObject(points, 2, 20);

	beaterUp->translateModel(glm::vec3(0.0, 21.1, 0.0));

	beaterUp->setMaterial(glm::vec3(0.5, 0.5, 1.0), glm::vec3(0.2, 0.2, 0.3), 105.0);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(0.5, 1.5));
	points.push_back(glm::vec2(2.4, 3.0));
	points.push_back(glm::vec2(2.5, 3.0));
	points.push_back(glm::vec2(2.5, 3.1));
	points.push_back(glm::vec2(2.2, 3.8));
	points.push_back(glm::vec2(2.1, 3.8));
	points.push_back(glm::vec2(2.1, 3.7));
	points.push_back(glm::vec2(0.0, 3.6));

	PagRevolutionObject *beaterHandle = new PagRevolutionObject(points, 2, 20);

	beaterHandle->translateModel(glm::vec3(1.2, 40.2, 0.0));

	beaterHandle->setMaterial(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 90.0);

	beater->insert3DElement(beaterDown);
	beater->insert3DElement(beaterUp);
	beater->insert3DElement(beaterHandle);

	beater->translateModel(glm::vec3(-30.0, 3.5, -30.0));
	beater->rotateModel(glm::vec3(1.0, 0.0, 0.0), 90);

	objects->insert3DElement(beater);

	// --------------------------------------------------------------------------
	// ---------------------------------- Lampara -------------------------------

	Pag3DGroup *bodyRotor = new Pag3DGroup();
	Pag3DGroup *bodyRotor2 = new Pag3DGroup();
	Pag3DGroup *bodyRotor3 = new Pag3DGroup();
	Pag3DGroup *flexo = new Pag3DGroup();

	points.clear();
	points.push_back(glm::vec2(0.0, 9.0));
	points.push_back(glm::vec2(2.0, 8.5));
	points.push_back(glm::vec2(2.5, 7.8));
	points.push_back(glm::vec2(2.2, 6.0));
	points.push_back(glm::vec2(4.0, 3.0));
	points.push_back(glm::vec2(4.5, 2.0));
	points.push_back(glm::vec2(4.8, 0.0));
	points.push_back(glm::vec2(5.0, 0.0)); 
	points.push_back(glm::vec2(5.2, 0.0));
	points.push_back(glm::vec2(4.9, 2.0));
	points.push_back(glm::vec2(3.9, 3.0));
	points.push_back(glm::vec2(2.6, 6.0));
	points.push_back(glm::vec2(2.9, 7.8));
	points.push_back(glm::vec2(2.4, 8.8));
	points.push_back(glm::vec2(0.0, 9.1));

	PagRevolutionObject *flexoHead = new PagRevolutionObject(points, 2, 30);
	
	flexoHead->translateModel(glm::vec3(0.0, 0.0, 0.0));
	flexoHead->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0,0.8, 1.0), 12.0);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(1.0, 1.0));
	points.push_back(glm::vec2(1.5, 2.5));
	points.push_back(glm::vec2(3.0, 5.0));
	points.push_back(glm::vec2(1.5, 7.0));
	points.push_back(glm::vec2(0.8, 7.5));
	points.push_back(glm::vec2(0.1, 7.5));
	points.push_back(glm::vec2(0.0, 7.5));

	PagRevolutionObject *bulb = new PagRevolutionObject(points, 2, 20);

	bulb->translateModel(glm::vec3(0.0, 8.0, 0.0));
	bulb->rotateModel(glm::vec3(1.0, 0.0, 0.0), 180);
	bulb->setMaterial(glm::vec3(1.0, 1.0, 0.9), glm::vec3(1.0, 1.0, 0.9), 15.0f);

	points.clear();
	points.push_back(glm::vec2(1.75, 0.0));
	points.push_back(glm::vec2(1.7, 0.2));
	points.push_back(glm::vec2(1.5, 0.2));
	points.push_back(glm::vec2(1.51, 0.2));
	points.push_back(glm::vec2(1.5, 1.5));
	points.push_back(glm::vec2(1.5, 1.55));
	points.push_back(glm::vec2(1.1, 1.56));
	points.push_back(glm::vec2(1.1, 3.0));
	points.push_back(glm::vec2(1.1, 3.1));
	points.push_back(glm::vec2(0.0, 3.2));

	PagRevolutionObject *haft = new PagRevolutionObject(points, 2, 20);
	
	haft->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);
	
	haft->translateModel(glm::vec3(0.0, 2.0, 4.4));
	haft->rotateModel(glm::vec3(1.0, 0.0, 0.0), 65);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(0.4, 0.0));
	points.push_back(glm::vec2(0.4, 0.32));
	points.push_back(glm::vec2(0.41, 0.32));
	points.push_back(glm::vec2(1.0, 0.32));
	points.push_back(glm::vec2(1.1, 0.32));
	points.push_back(glm::vec2(1.1, 0.64));
	points.push_back(glm::vec2(1.2, 0.64));
	points.push_back(glm::vec2(1.4, 0.8));
	points.push_back(glm::vec2(1.5, 1.1));
	points.push_back(glm::vec2(1.4, 1.4));
	points.push_back(glm::vec2(1.2, 1.56));
	points.push_back(glm::vec2(1.1, 1.56));
	points.push_back(glm::vec2(1.1, 1.88));
	points.push_back(glm::vec2(1.0, 1.88));
	points.push_back(glm::vec2(0.41, 1.88));
	points.push_back(glm::vec2(0.4, 1.88));
	points.push_back(glm::vec2(0.4, 2.2));
	points.push_back(glm::vec2(0.0, 2.2));

	PagRevolutionObject *rotor = new PagRevolutionObject(points, 2, 40);
	PagRevolutionObject *rotor2 = new PagRevolutionObject(*rotor);
	PagRevolutionObject *rotor3 = new PagRevolutionObject(*rotor);

	rotor->translateModel(glm::vec3(1.1, 4.2, 9.5));
	rotor->rotateModel(glm::vec3(0.0, 0.0, 1.0), 90);
	rotor->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(1.1, 0.0));
	points.push_back(glm::vec2(1.12, 0.0));
	points.push_back(glm::vec2(1.12, 0.01));
	points.push_back(glm::vec2(1.1, 0.25));
	points.push_back(glm::vec2(0.9, 0.25));
	points.push_back(glm::vec2(0.85, 0.5));
	points.push_back(glm::vec2(0.9, 0.75));
	points.push_back(glm::vec2(1.05, 1.3));
	points.push_back(glm::vec2(0.0, 1.0));

	PagRevolutionObject *connexion = new PagRevolutionObject(points, 2, 20);
	PagRevolutionObject *connexion2 = new PagRevolutionObject(*connexion);
	PagRevolutionObject *connexion3 = new PagRevolutionObject(*connexion);
	PagRevolutionObject *connexion4 = new PagRevolutionObject(*connexion);
	PagRevolutionObject *connexion5 = new PagRevolutionObject(*connexion);

	connexion->translateModel(glm::vec3(0.0, 3.25, 7.25));
	connexion->rotateModel(glm::vec3(1.0, 0.0, 0.0), 65.0);
	connexion->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);

	bodyRotor->insert3DElement(flexoHead);
	bodyRotor->insert3DElement(bulb);
	bodyRotor->insert3DElement(haft);
	bodyRotor->insert3DElement(connexion);
	bodyRotor->insert3DElement(rotor);

	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(1.0, 0.0));
	points.push_back(glm::vec2(1.1, 0.0));
	points.push_back(glm::vec2(1.1, 0.1));
	points.push_back(glm::vec2(1.0, 0.4));
	points.push_back(glm::vec2(0.85, 0.4));
	points.push_back(glm::vec2(0.84, 0.4));
	points.push_back(glm::vec2(0.85, 0.41));
	points.push_back(glm::vec2(0.85, 10.59));
	points.push_back(glm::vec2(0.84, 10.6));
	points.push_back(glm::vec2(0.85, 10.6));
	points.push_back(glm::vec2(1.0, 10.6));
	points.push_back(glm::vec2(1.1, 10.9));
	points.push_back(glm::vec2(1.1, 11.0));
	points.push_back(glm::vec2(1.0, 11.0));
	points.push_back(glm::vec2(0.0, 11.0));

	PagRevolutionObject *tube = new PagRevolutionObject(points, 2, 20);
	PagRevolutionObject *tube2 = new PagRevolutionObject(*tube);

	tube->translateModel(glm::vec3(0.0, 2.9, 11.4));
	tube->rotateModel(glm::vec3(1.0, 0.0, 0.0), 130);
	tube->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);

	connexion2->translateModel(glm::vec3(0.0, 2.85, 11.4));
	connexion2->rotateModel(glm::vec3(-1.0, 0.0, 0.0), 50.0);
	connexion2->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);	
	
	connexion3->translateModel(glm::vec3(0.0, -4.1, 19.8));
	connexion3->rotateModel(glm::vec3(1.0, 0.0, 0.0), 130.0);
	connexion3->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);
	
	rotor2->translateModel(glm::vec3(1.1, -5.5, 21.8));
	rotor2->rotateModel(glm::vec3(0.0, 0.0, 1.0), 90);
	rotor2->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);

	bodyRotor->translateModel(glm::vec3(0.0, 5.2, -1.0));
	bodyRotor->rotateModel(glm::vec3(1.0, 0.0, 0.0), 30);

	bodyRotor2->insert3DElement(bodyRotor);
	bodyRotor2->insert3DElement(connexion2);
	bodyRotor2->insert3DElement(tube);
	bodyRotor2->insert3DElement(connexion3);
	bodyRotor2->insert3DElement(rotor2);

	connexion4->translateModel(glm::vec3(0.0, -7.4, 20.3));
	connexion4->rotateModel(glm::vec3(1.0, 0.0, 0.0), 40.0);
	connexion4->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);

	tube2->translateModel(glm::vec3(0.0, -15.8, 13.15));
	tube2->rotateModel(glm::vec3(1.0, 0.0, 0.0), 40);
	tube2->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);

	connexion5->translateModel(glm::vec3(0.0, -15.75, 13.15));
	connexion5->rotateModel(glm::vec3(1.0, 0.0, 0.0), 220.0);
	connexion5->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);

	rotor3->translateModel(glm::vec3(1.1, -17.6, 11.45));
	rotor3->rotateModel(glm::vec3(0.0, 0.0, 1.0), 90);
	rotor3->setMaterial(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.7, 0.7, 0.7), 0.1f);

	bodyRotor3->insert3DElement(bodyRotor2);
	bodyRotor3->insert3DElement(connexion4);
	bodyRotor3->insert3DElement(tube2);
	bodyRotor3->insert3DElement(connexion5);
	bodyRotor3->insert3DElement(rotor3);

	bodyRotor3->translateModel(glm::vec3(0.0, 28.3, -11.45));
	
	points.clear();
	points.push_back(glm::vec2(0.0, 0.0));
	points.push_back(glm::vec2(7.0, 0.0));
	points.push_back(glm::vec2(7.1, 0.0));
	points.push_back(glm::vec2(7.1, 0.1));
	points.push_back(glm::vec2(7.1, 2.8));
	points.push_back(glm::vec2(7.1, 3.0));
	points.push_back(glm::vec2(7.3, 3.0));
	points.push_back(glm::vec2(5.0, 3.0));
	points.push_back(glm::vec2(5.0, 5.0));
	points.push_back(glm::vec2(4.8, 5.2));
	points.push_back(glm::vec2(3.0, 5.5));
	points.push_back(glm::vec2(2.0, 8.0));
	points.push_back(glm::vec2(2.3, 8.2));
	points.push_back(glm::vec2(1.5, 9.0));
	points.push_back(glm::vec2(1.8, 10.0));
	points.push_back(glm::vec2(1.6, 10.0));
	points.push_back(glm::vec2(0.0, 9.0));

	PagRevolutionObject *base = new PagRevolutionObject(points, 2, 20);
	
	base->setMaterial(glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.0, 0.8, 1.0), 12.0);
	base->translateModel(glm::vec3());
	flexo->insert3DElement(bodyRotor3);
	flexo->insert3DElement(base);

	flexo->translateModel(glm::vec3(20.0, 0.0, 20.0));
	flexo->rotateModel(glm::vec3(0.0, 1.0, 0.0), 45);

	objects->insert3DElement(flexo);

	// --------------------------------------------------------------------------
	// ---------------------------------- Mesa ----------------------------------

	PagPlane *board = new PagPlane(100.0, 80.0, 10.0, 8, 8);
	board->translateModel(glm::vec3(-50.0, 0.0, -40.0));
	board->setMaterial(glm::vec3(0.5, 0.2, 0.0), glm::vec3(0.2, 0.1, 0.0), 10.0);

	objects->insert3DElement(board);

	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------


	// - Crea el shader program que dibujará la geometría en forma de nube de puntos.
	pointShader = new PagShaderProgram();
	pointShader->createShaderProgram("pointShader");  

	// - Crea el shader program que dibujará la geometría en modo alambre.
	linesShader = new PagShaderProgram();
	linesShader->createShaderProgram("linesShader");

	// - Crea el shader program que dibujará la geometría en triangulos sombreados.
	ADSShader = new PagShaderProgram();
	idAds = ADSShader->createShaderProgram("ADSShader");

	// - Crea el shader program que dibujará la geometría en modo debug.
	debugShader = new PagShaderProgram();
	debugShader->createShaderProgram("debugShader");

	// - Defino 3 luces, una de tipo puntual, otra de tipo direccional y otra tipo foco
	PagPointLight *point = new PagPointLight(glm::vec3(0.3, 0.3, 0.1), glm::vec3(0.3, 0.3, 0.2), 
		glm::vec3(13.0, 13.0, 13.0), 1.0f, 0.0f, 0.0f);

	PagSpotLight *spot = new PagSpotLight(glm::vec3(0.6, 0.6, 0.4), glm::vec3(0.4, 0.4, 0.3),
		glm::vec3(-1.0, -1.0, -1.0), glm::vec3(15.0, 15.0, 15.0), 20.0f, 60.0f, 0.1f, 0.05f, 0.0f);
	
	PagSpotLight *spot2 = new PagSpotLight(glm::vec3(0.8, 0.8, 0.5), glm::vec3(0.6, 0.6, 0.4),
		glm::vec3(5.0, 6.0, 5.0), glm::vec3(4.0, 28.0, 4.0), 10.0f, 60.0f, 1.0f, 0.0f, 0.0f);

	PagDirectionalLight *directional = new PagDirectionalLight(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.3, 0.3, 0.3), 
		glm::vec3(0.3, 0.3, 0.2), glm::vec3(10.0, -5.0, -10.0), 0.1f, 0.1f, 0.0f);

	PagAmbientLight *ambient = new PagAmbientLight(glm::vec3(0.0, 0.0, 0.05));

	light.push_back(new PagLightSource(directional));
	light.push_back(new PagLightSource(point));
	light.push_back(new PagLightSource(spot));
	light.push_back(new PagLightSource(spot2));
	light.push_back(new PagLightSource(ambient));
};