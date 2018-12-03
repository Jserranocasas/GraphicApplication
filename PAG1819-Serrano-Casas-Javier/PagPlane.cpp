#include "PagPlane.h"

// - Constructor parametrizado para la clase PagPlane
PagPlane::PagPlane(float width, float height, float depth, int tilingH, int tilingV) {

	for (int i = 0; i < 6; i++) {
		sides[i] = new PagVAO();
	}

	addPointsLevel(width, height, depth, tilingH, tilingV, 0);
	addPointsLevel(width, height, depth, tilingH, tilingV, 1);
	addTexturesCoordenates(width, height, depth, tilingH, tilingV);
	addTangentsCoordenates(width, height, depth, tilingH, tilingV);
	defineTopology(tilingH, tilingV);
	fillVectorVAO();
}

// - Destructor de la clase PagPlane
PagPlane::~PagPlane(){
	delete []sides;
}

// - Dibuja los 6 lados del tablero como triangulos sombreados
void PagPlane::drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v) {
	for (int i = 0; i < 6; i++) {
		sides[i]->drawTriangles(PAG_BODY);
	}
}

// - Dibuja los 6 lados del tablero como nube de puntos
void PagPlane::drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject) {
	for (int i = 0; i < 6; i++) {
		sides[i]->drawPoints();
	}
}

// - Dibuja los 6 lados del tablero en modo alambre
void PagPlane::drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject) {
	for (int i = 0; i < 6; i++) {
		sides[i]->drawLines();
	}
}

/**
 * Añade los puntos de la geometría a distintas alturas segun su nivel
 * @param width float. Ancho en el eje x del plano xz del tablero
 * @param height float. Alto en el eje z del plano xz del tablero
 * @param depth float. Profundidad en el eje y del plano xy e yz del tablero
 * @param tilingH int. Número de divisiones horizontales del plano
 * @param tilingV int. Número de divisiones verticales del plano
 * @param level int. Nivel para la adicion de puntos
 */
void PagPlane::addPointsLevel(float width, float height, float depth, int tilingH, int tilingV, int level) {

	float incrementH = width / tilingH;
	float incrementV = height / tilingV;

	for (int i = 0; i <= tilingV; i++) {
		for (int j = 0; j <= tilingH; j++) {
			PagPosNorm aux;

			aux.position = glm::vec3( j * incrementH, -depth * level, i * incrementV);

			if (level == 0) {  // Normal de la cara superior del tablero
				aux.normal = glm::vec3(0.0, 1.0, 0.0);
			}

			if (level == 1) {  // Normal de la cara inferior del tablero
				aux.normal = glm::vec3(0.0, -1.0, 0.0);
			}

			sides[level]->addPosNorm(aux); //Para las caras superior e inferior

			if (i == 0) { //Para la cara lejana del tablero
				aux.normal = glm::vec3(0.0, 0.0, -1.0);
				sides[2]->addPosNorm(aux);
			}
			 
			if (i == tilingV) { //Para la cara cercana del tablero
				aux.normal = glm::vec3(0.0, 0.0, 1.0);
				sides[3]->addPosNorm(aux);
			}

			if (j == 0) { //Para la cara izquierda del tablero
				aux.normal = glm::vec3(-1.0, 0.0, 0.0);
				sides[4]->addPosNorm(aux);
			}

			if (j == tilingH) { //Para la cara derecha del tablero
				aux.normal = glm::vec3(1.0, 0.0, 0.0);
				sides[5]->addPosNorm(aux);
			}

		}
	}
}

/**
 * Añade las coordenadas de textura del objeto
 * @param width float. Ancho en el eje x del plano xz del tablero
 * @param height float. Alto en el eje z del plano xz del tablero
 * @param depth float. Profundidad en el eje y del plano xy e yz del tablero
 * @param tilingH int. Número de divisiones horizontales del plano
 * @param tilingV int. Número de divisiones verticales del plano
 */
void PagPlane::addTexturesCoordenates(float width, float height, float depth, int tilingH, int tilingV) {
	for (int s = 0; s < 2; s++) {
		for (int i = 0; i <= tilingV; i++) { //Para la cara superior e inferior del tablero
			for (unsigned int j = 0; j <= tilingH; j++) {
				sides[s]->addTexture(glm::vec2(j / (1.0 * tilingH), i / (1.0 * tilingV)));
			}
		}
	}
	
	for (int s = 2; s < 4; s++) { //Para la cara cercana y lejana del tablero
		for (int i = 0; i < 2; i++) {
			for (unsigned int j = 0; j <= tilingH; j++) {
				sides[s]->addTexture(glm::vec2(j / (1.0 * tilingH), i));
			}
		}
	}

	for (int s = 4; s < 6; s++) { //Para la cara izquierda y derecha del tablero
		for (unsigned int i = 0; i < 2; i++) {
			for (int j = 0; j <= tilingV; j++) {
				sides[s]->addTexture(glm::vec2(j / (1.0 * tilingH), i));
			}
		}
	}

}

/**
 * Añade las coordenadas de las tangentes del objeto
 * @param width float. Ancho en el eje x del plano xz del tablero
 * @param height float. Alto en el eje z del plano xz del tablero
 * @param depth float. Profundidad en el eje y del plano xy e yz del tablero
 * @param tilingH int. Número de divisiones horizontales del plano
 * @param tilingV int. Número de divisiones verticales del plano
 */
void PagPlane::addTangentsCoordenates(float width, float height, float depth, int tilingH, int tilingV) {
	for (int i = 0; i <= tilingV; i++) { //Para la cara superior del tablero
		for (unsigned int j = 0; j <= tilingH; j++) {
			sides[0]->addTangent(glm::vec3(0.0, 0.0, -1.0));
		}
	}
	
	for (int i = 0; i <= tilingV; i++) { //Para la cara inferior del tablero
		for (unsigned int j = 0; j <= tilingH; j++) {
			sides[1]->addTangent(glm::vec3(0.0, 0.0, 1.0));
		}
	}

	for (int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j <= tilingH; j++) { //Para la cara lejana del tablero
			sides[2]->addTangent(glm::vec3(-1.0, 0.0, 0.0));
		}
	}
	
	for (int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j <= tilingH; j++) { //Para la cara cercana del tablero
			sides[3]->addTangent(glm::vec3(1.0, 0.0, 0.0));
		}
	}

	for (unsigned int i = 0; i < 2; i++) {  //Para la cara izquierda del tablero
		for (int j = 0; j <= tilingV; j++) {
			sides[4]->addTangent(glm::vec3(0.0, 0.0, 1.0));
		}
	}

	for (unsigned int i = 0; i < 2; i++) {  //Para la cara derecha del tablero
		for (int j = 0; j <= tilingV; j++) {
			sides[5]->addTangent(glm::vec3(0.0, 0.0, -1.0));
		}
	}
}

// - Define la topología del objeto para modo alambre, puntos y triángulos
void PagPlane::defineTopology(int tilingH, int tilingV) {
	definePointCloud(tilingH, tilingV);
	defineWireFrame(tilingH, tilingV);
	defineMeshTriangles(tilingH, tilingV);
}

// - Rellena los vectores de geometría y topología para los shaders
void PagPlane::fillVectorVAO() {
	for (int i = 0; i < 6; i++) {
		sides[i]->fill();
	}
}

// - Define la topologia para la nuble de puntos
void PagPlane::definePointCloud(int tilingH, int tilingV) {
	for (int i = 0; i < 6; i++) {
		for (unsigned int j = 0; j < sides[i]->getPosNorm().size(); j++) {
			sides[i]->addIndexCloud(j);
		}
	}
}

// - Define la topologia para los triangulos sombreados
void PagPlane::defineMeshTriangles(int tilingH, int tilingV) {
	
	// Cara superior del plano
	for (unsigned int j = 0; j < tilingV; j++) {
		for (unsigned int m = 0; m < tilingH; m++) {
			sides[0]->addIndexTMesh(j * (tilingH + 1) + m);
			sides[0]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1) + 1);
			sides[0]->addIndexTMesh(j * (tilingH + 1) + m + 1);
			sides[0]->addIndexTMesh(0xFFFFFFFF);

			sides[0]->addIndexTMesh(j * (tilingH + 1) + m);
			sides[0]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1));
			sides[0]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1) + 1);
			sides[0]->addIndexTMesh(0xFFFFFFFF);
		}
	}
	
	// Cara inferior del plano
	for (unsigned int j = 0; j < tilingV; j++) {
		for (unsigned int m = 0; m < tilingH; m++) {
			sides[1]->addIndexTMesh(j * (tilingH + 1) + m);
			sides[1]->addIndexTMesh(j * (tilingH + 1) + m + 1);
			sides[1]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1) + 1);
			sides[1]->addIndexTMesh(0xFFFFFFFF);

			sides[1]->addIndexTMesh(j * (tilingH + 1) + m);
			sides[1]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1) + 1);
			sides[1]->addIndexTMesh(j * (tilingH + 1) + m + (tilingH + 1));
			sides[1]->addIndexTMesh(0xFFFFFFFF);
		}
	}

	// Cara lejana del plano
	for (unsigned int m = 0; m < tilingH; m++) {
		sides[2]->addIndexTMesh(m);
		sides[2]->addIndexTMesh(m + (tilingH + 1) + 1);
		sides[2]->addIndexTMesh(m + 1);
		sides[2]->addIndexTMesh(0xFFFFFFFF);

		sides[2]->addIndexTMesh(m);
		sides[2]->addIndexTMesh(m + (tilingH + 1));
		sides[2]->addIndexTMesh(m + (tilingH + 1) + 1);
		sides[2]->addIndexTMesh(0xFFFFFFFF);
	}

	// Cara cercana del plano
	for (unsigned int m = 0; m < tilingH; m++) {
		sides[3]->addIndexTMesh(m);
		sides[3]->addIndexTMesh(m + (tilingH + 1) + 1);
		sides[3]->addIndexTMesh(m + 1);
		sides[3]->addIndexTMesh(0xFFFFFFFF);

		sides[3]->addIndexTMesh(m);
		sides[3]->addIndexTMesh(m + (tilingH + 1));
		sides[3]->addIndexTMesh(m + (tilingH + 1) + 1);
		sides[3]->addIndexTMesh(0xFFFFFFFF);
	}

	// Cara lateral izquierda del plano
	for (unsigned int m = 0; m < tilingV; m++) {
		sides[4]->addIndexTMesh(m);
		sides[4]->addIndexTMesh(m + (tilingV + 1));
		sides[4]->addIndexTMesh(m + 1);
		sides[4]->addIndexTMesh(0xFFFFFFFF);

		sides[4]->addIndexTMesh(m + 1);
		sides[4]->addIndexTMesh(m + (tilingV + 1));
		sides[4]->addIndexTMesh(m + (tilingV + 1) + 1);
		sides[4]->addIndexTMesh(0xFFFFFFFF);
	}
	
	// Cara lateral derecha del plano
	for (unsigned int m = 0; m < tilingV; m++) {
		sides[5]->addIndexTMesh(m);
		sides[5]->addIndexTMesh(m + 1);
		sides[5]->addIndexTMesh(m + (tilingV + 1) + 1);
		sides[5]->addIndexTMesh(0xFFFFFFFF);

		sides[5]->addIndexTMesh(m);
		sides[5]->addIndexTMesh(m + (tilingV + 1) + 1);
		sides[5]->addIndexTMesh(m + (tilingV + 1));
		sides[5]->addIndexTMesh(0xFFFFFFFF);
	}
}


// - Define la topologia para el modo de lineas
void PagPlane::defineWireFrame(int tilingH, int tilingV) {
	for (int i = 0; i < 2; i++) {
		for (unsigned int j = 0; j <= tilingV; j++) {
			for (unsigned int m = 0; m < tilingH; m++) {
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + m);
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + m + 1);
				sides[i]->addIndexWireFrame(0xFFFFFFFF);
			}
		}

		for (int j = 0; j < tilingV; j++) {
			for (unsigned int m = 0; m <= tilingH; m++) {
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + m);
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + (tilingH + 1) + m);
				sides[i]->addIndexWireFrame(0xFFFFFFFF);
			}
		}
	}

	for (int i = 2; i < 4; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			for (unsigned int m = 0; m < tilingH; m++) {
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + m);
				sides[i]->addIndexWireFrame(j * (tilingH + 1) + m + 1);
				sides[i]->addIndexWireFrame(0xFFFFFFFF);
			}
		}

		for (unsigned int m = 0; m <= tilingH; m++) {
			sides[i]->addIndexWireFrame(m);
			sides[i]->addIndexWireFrame((tilingH + 1) + m);
			sides[i]->addIndexWireFrame(0xFFFFFFFF);
		}
	}

	for (int i = 4; i < 6; i++) {
		for (unsigned int j = 0; j < 2; j++) {
			for (unsigned int m = 0; m < tilingV; m++) {
				sides[i]->addIndexWireFrame(j * (tilingV + 1) + m);
				sides[i]->addIndexWireFrame(j * (tilingV + 1) + m + 1);
				sides[i]->addIndexWireFrame(0xFFFFFFFF);
			}
		}

		for (unsigned int m = 0; m <= tilingV; m++) {
			sides[i]->addIndexWireFrame(m);
			sides[i]->addIndexWireFrame((tilingV + 1) + m);
			sides[i]->addIndexWireFrame(0xFFFFFFFF);
		}
	}
}