#include "PagVAO.h"
#include <iostream>

PagVAO::PagVAO() : vao(0) {
	glGenVertexArrays(1, &vao);

	// - Siempre que se quiere usar un VAO, 
	//   hay que activarlo con esta orden.
	glBindVertexArray(vao);

	// - Se generan los VBOs y se activan
	glGenBuffers(3, vbo);

	// - Se genera el VBO y se activa
	// - Se activa el VBO que se quiere rellenar
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// - Se describe cómo es la geometría que hay en ese array
	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	// layout (location=0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a las
	// posiciones
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat),
		GL_FLOAT, GL_FALSE, sizeof(PagPosNorm),
		((GLubyte *)NULL + (0)));

	// - Como es un array entrelazado, hay que repetir el proceso para los demás elementos,
	// en este caso para la normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat),
		GL_FLOAT, GL_FALSE, sizeof(PagPosNorm),
		((GLubyte *)NULL + (sizeof(glm::vec3))));


	// - Se activa el VBO que se quiere rellenar
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// - Se describe cómo es la geometría que hay en ese array
	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	// layout (location=2) en el shader, en concreto la posición
	glEnableVertexAttribArray(2);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a las
	// tangentes
	glVertexAttribPointer(2, sizeof(glm::vec3) / sizeof(GLfloat),
		GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
		((GLubyte *)NULL + (0)));

	// - Se activa el VBO que se quiere rellenar
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	// - Se describe cómo es la geometría que hay en ese array
	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	// layout (location=3) en el shader, en concreto la posición
	glEnableVertexAttribArray(3);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a las
	// texturas
	glVertexAttribPointer(3, sizeof(glm::vec2) / sizeof(GLfloat),
		GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
		((GLubyte *)NULL + (0)));

	// - Se generan los IBOs y se activan
	glGenBuffers(3, ibo);
}

PagVAO::~PagVAO() {}

/* --------------------- Métodos getter ---------------------*/
std::vector<PagPosNorm> PagVAO::getPosNorm() const {
	return posNorm;
}

std::vector<glm::vec2> PagVAO::getTextures() const {
	return textures;
}

std::vector<glm::vec3> PagVAO::getTangents() const {
	return tangents;
}

std::vector<GLuint> PagVAO::getIndexesTMesh() const {
	return indexesTMesh;
}

std::vector<GLuint> PagVAO::getIndexesCloud() const {
	return indexesCloud;
}

std::vector<GLuint> PagVAO::getIndexesWireframe() const {
	return indexesWireframe;
}

// ------------ Métodos para añadir a los vectores -----------
void PagVAO::addIndexWireFrame(GLuint index) {
	indexesWireframe.push_back(index);
}

void PagVAO::addIndexTMesh(GLuint index) {
	indexesTMesh.push_back(index);
}

void PagVAO::addIndexCloud(GLuint index) {
	indexesCloud.push_back(index);
}

void PagVAO::addPosNorm(PagPosNorm ppn) {
	posNorm.push_back(ppn);
}

void PagVAO::addTangent(glm::vec3 tang) {
	tangents.push_back(tang);
}

void PagVAO::addTexture(glm::vec2 txt) {
	textures.push_back(txt);
}

/* ---------------- Métodos públicos -----------------*/
// - Rellena los vbos e ibos con los vectores de geometría y topología respectivamente
void PagVAO::fill() {
	// - Se activa el VAO que contiene al IBO que se quiere rellenar con su array de topología
	glBindVertexArray(vao);

	// - Rellenamos los VBOs
	// - Se activa el VBO de posición y normal que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	// - Se le pasa el array que contiene los datos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, posNorm.size() * sizeof(PagPosNorm),
		posNorm.data(), GL_STATIC_DRAW);

	// - Se activa el VBO de tangentes que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	// - Se le pasa el array que contiene los datos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3),
		tangents.data(), GL_STATIC_DRAW);

	// - Se activa el VBO de textura que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	// - Se le pasa el array que contiene los datos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2),
		textures.data(), GL_STATIC_DRAW);

	// - Rellenamos los IBOs
	// - Se activa el IBO que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	// - Se le pasa el array que contiene los índices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesCloud.size() * sizeof(GLuint),
		indexesCloud.data(), GL_STATIC_DRAW);

	// - Se activa el IBO que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	// - Se le pasa el array que contiene los índices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesWireframe.size() * sizeof(GLuint),
		indexesWireframe.data(), GL_STATIC_DRAW);

	// - Se activa el IBO que se quiere rellenar
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
	// - Se le pasa el array que contiene los índices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesTMesh.size() * sizeof(GLuint),
		indexesTMesh.data(), GL_STATIC_DRAW);
}

// - Dibuja como puntos
void PagVAO::drawPoints() {
	// - Se activa el VAO
	glBindVertexArray(vao);
	// - Se activa el IBO que contiene los índices adecuados al modo de dibujo que se quiera
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	// - Se dibuja la malla
	glDrawElements(GL_POINTS, indexesCloud.size(), GL_UNSIGNED_INT, NULL);
}

// - Dibuja como lineas
void PagVAO::drawLines() {
	// - Se activa el VAO
	glBindVertexArray(vao);
	// - Se activa el IBO que contiene los índices adecuados al modo de dibujo que se quiera
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	// - Se dibuja la malla
	glDrawElements(GL_LINE_STRIP, indexesWireframe.size(), GL_UNSIGNED_INT, NULL);
}

// - Dibuja como triangulos
void PagVAO::drawTriangles(PagRevObjParts part) {
	// - Se activa el VAO
	glBindVertexArray(vao);
	// - Se activa el IBO que contiene los índices adecuados al modo de dibujo que se quiera
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);

	// - Se dibuja la malla ----------- 
	if (part == PAG_TOP_FAN || part == PAG_BOTTOM_FAN) {
		glDrawElements(GL_TRIANGLE_FAN, indexesTMesh.size(), GL_UNSIGNED_INT, NULL);
	}
	else {
		glDrawElements(GL_TRIANGLE_STRIP, indexesTMesh.size(), GL_UNSIGNED_INT, NULL);
	}
}