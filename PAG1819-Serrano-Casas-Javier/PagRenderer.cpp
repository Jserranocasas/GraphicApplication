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
PagRenderer::PagRenderer() : debug(false), lines(false), tmesh(false), points(true), debugOption (0) {
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

	pawn->setMaterial(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.3, 0.4, 0.3), 0.2f);

	objects->insert3DElement(pawn);

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

	beater->translateModel(glm::vec3(20.0, 0.0, 0.0));
	
	objects->insert3DElement(beater);

	// --------------------------------------------------------------------------
	// ---------------------------------- Lampara -------------------------------

	Pag3DGroup *flexo = new Pag3DGroup();

	points.clear();
	points.push_back(glm::vec2(0.0, 9.0));
	points.push_back(glm::vec2(2.0, 8.5));
	points.push_back(glm::vec2(2.5, 7.8));
	points.push_back(glm::vec2(2.2, 6.0));
	points.push_back(glm::vec2(4.0, 3.0));
	points.push_back(glm::vec2(4.5, 2.0));
	points.push_back(glm::vec2(4.8, 0.0));
	points.push_back(glm::vec2(5.0, 0.0)); // Mitad
	points.push_back(glm::vec2(5.2, 0.0));
	points.push_back(glm::vec2(4.9, 2.0));
	points.push_back(glm::vec2(3.9, 3.0));
	points.push_back(glm::vec2(2.6, 6.0));
	points.push_back(glm::vec2(2.9, 7.8));
	points.push_back(glm::vec2(2.4, 8.8));
	points.push_back(glm::vec2(0.0, 9.1));

	PagRevolutionObject *flexoHead = new PagRevolutionObject(points, 2, 20);
	
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
	bulb->setMaterial(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 0.1f);

	flexo->insert3DElement(flexoHead);
	flexo->insert3DElement(bulb);

	flexo->translateModel(glm::vec3(-20.0, 20.0, 0.0));
	flexo->rotateModel(glm::vec3(1.0, 1.0, 0.0), 90);

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
	PagPointLight *point = new PagPointLight(glm::vec3(0.35, 0.3, 0.3), glm::vec3(0.2, 0.1, 0.1), 
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(30.0, 15.0, -30.0), 1.0f, 0.0f, 0.0f, PAG_POINT);

	PagSpotLight *spot = new PagSpotLight(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.1, 0.1, 0.3),
		glm::vec3(0.0, -1.0, 0.0), glm::vec3(-5.0, 1.0, 0.0), 0.0f, 60.0f, 0.7f, 0.4f, 0.0f, PAG_SPOT);

	PagDirectionalLight *directional = new PagDirectionalLight(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.3, 0.3, 0.3), 
		glm::vec3(0.3, 0.3, 0.2), glm::vec3(-10.0, -5.0, 10.0), 0.3f, 0.5f, 0.0f, PAG_DIRECTIONAL);

	//light.push_back(new PagLightSource(directional));
	light.push_back(new PagLightSource(point));
	//light.push_back(new PagLightSource(spot));
};