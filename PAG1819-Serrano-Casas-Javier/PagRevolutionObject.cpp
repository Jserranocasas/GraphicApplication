#include "PagRevolutionObject.h"
#include "gtc/constants.hpp"
#include "gtc/epsilon.hpp"
#include <iostream>

// - Constructor de la clase PagRevolutionObject
PagRevolutionObject::PagRevolutionObject(std::vector<glm::vec2> points, 
		unsigned int subdivisions, unsigned int slices) : Pag3DElement() {
	profile = PagSubdivisionProfile(points);
	profile = profile.subdivide(subdivisions);

	if ( !isValid() ) {
		throw std::logic_error( "PagRevolutionObject::PagRevolutionObject: " 
													   "Perfil no válido" );
	}

	if ( has(PAG_BOTTOM_FAN) ) {
		bottom = new PagVAO();
		computePosNormTex(PAG_BOTTOM_FAN, slices);
		computeTangents(PAG_BOTTOM_FAN, slices);
		defineTopology(PAG_BOTTOM_FAN, slices);
		bottom->fill();
	}

	if ( has(PAG_TOP_FAN) ) {
		top = new PagVAO();
		computePosNormTex(PAG_TOP_FAN, slices);
		computeTangents(PAG_TOP_FAN, slices);
		defineTopology(PAG_TOP_FAN, slices);
		top->fill();
	}

	if ( has(PAG_BODY) ) {
		body = new PagVAO();
		computePosNormTex(PAG_BODY, slices);
		computeTangents(PAG_BODY, slices);
		defineTopology(PAG_BODY, slices);
		body->fill();
	}

}

PagRevolutionObject::PagRevolutionObject(const PagRevolutionObject &orig) {
	delete body;
	delete top;
	delete bottom;

	profile = orig.profile;
	top = orig.top;
	bottom = orig.bottom;
	body = orig.body;
}

// - Operador de asignación de la clase PagSubdivision.
PagRevolutionObject& PagRevolutionObject::operator=(const PagRevolutionObject &orig) {
	delete body;
	delete top;
	delete bottom;

	if ( this != &orig ) {
		profile = orig.profile;
		top = orig.top;
		bottom = orig.bottom;
		body = orig.body;
	}
	return *this;
}

// - Destructor de la clase PagRevolutionObject.
PagRevolutionObject::~PagRevolutionObject(){
	delete body;
	delete top;
	delete bottom;
}

// - Método que compueba si el perfil en el que se basa este objeto de 
//   revolución es válido. Devuelve true si lo es y false en caso contrario
bool PagRevolutionObject::isValid() {
	std::vector<glm::vec2> positions = profile.getPositions();

	for (unsigned int i = 0; i < positions.size(); i++) {
		// Comprueba que no existen puntos erróneos
		if ( positions[i].x < 0 )
			return false;
		// Comprueba que no existan puntos intermedios con x = 0
		if ( i != 0 && i != positions.size() - 1 && glm::epsilonEqual<double>(positions[i].x, 
																0.0, glm::epsilon<double>()))
			return false;
	}

	if ( profile.size() == 3 ) { // Comprueba si tiene 3 puntos pero con dos tapas
		if (glm::epsilonEqual<double>(positions[0].x, 0.0, glm::epsilon<double>()) && 
			glm::epsilonEqual<double>(positions[2].x, 0.0, glm::epsilon<double>())) {
			return false;
		}
	}

	if ( profile.size() == 2 ) { // Comprueba si existen unicamente dos puntos
		// Comprueba si dichos puntos se encuentra en el eje x
		if (glm::epsilonEqual<double>(positions[0].x, 0.0, glm::epsilon<double>()) &&
			glm::epsilonEqual<double>(positions[1].x, 0.0, glm::epsilon<double>())) {
			return false;
		}
	}

	if ( profile.size() < 2 ) { // Compruebo que existen mas de 1 punto
		return false;
	}

	return true;
}

// - Método que comprueba si el objeto tiene la parte pasada por parámetro 
//   en el perfil. Post: Se da por hecho que se ha validado el perfil antes
bool PagRevolutionObject::has(PagRevObjParts part) {
	if ( part < 0 || part > 2 ) {
		throw std::out_of_range("PagRevolutionObject::has: No se corresponde "
											  "con ninguna parte del objeto");
	}

	// Si tiene tapa inferior
	if ( part == PAG_BOTTOM_FAN ) {		
		return profile.existBottom();
	}

	// Si tiene tapa superior
	else if ( part == PAG_TOP_FAN ) {				
		return profile.existTop();
	}
	// Si tiene cuerpo
	else {								
		if ( !profile.existTop() && !profile.existBottom()) {
			return true;
		}
		else if ( (profile.existTop() && !profile.existBottom()) ||
				  (profile.existBottom() && !profile.existTop()) ) {
			if ( profile.size() >= 3 )
				return true;
			else
				return false;
		}
		else {
			if ( profile.size() >= 4 )
				return true;
			else
				return false;
		}
	}
}

// - Método que inicializa las posiciones, normales y texturas de la parte pasada por 
//   parámetro. Creo la geometría para el perfil de revolución. Tanto tapas como cuerpo.
void PagRevolutionObject::computePosNormTex(PagRevObjParts part, unsigned int slices) {
	std::vector<glm::vec3> normals = profile.getNormals();
	double deltaRad = (2 * glm::pi<double>()) / slices;
	struct PagPosNorm aux;

	// Para la tapa inferior
	if ( part == PAG_BOTTOM_FAN ) { 	
		aux.position = glm::vec3(0.0, profile.getPosition(0).y, 0.0);
		aux.normal = glm::normalize(normals[0]);

		bottom->addPosNorm(aux);  // Añado coordenadas de posicion y normal
		bottom->addTexture(glm::vec2(0.5, 0.5)); // Añado coordenada de textura

		for (unsigned int s = 0; s <= slices; s++) {	 // Revoluciono segundo punto
			glm::vec3 pI(profile.getPosition(1).x * cos(s*deltaRad), profile.getPosition(1).y,
						-profile.getPosition(1).x * sin(s*deltaRad));
			
			glm::vec3 nI(normals[1].x * cos(s*deltaRad), normals[1].y,
						 -normals[1].x * sin(s*deltaRad));

			aux.position = pI;
			aux.normal = glm::normalize(nI);

			bottom->addPosNorm(aux); // Añado coordenadas de posicion y normal
			bottom->addTexture(glm::vec2( (float) (cos(s*deltaRad) / 2) + 0.5,
							(float) (-sin(s*deltaRad) / 2) + 0.5)); // Añado coordenada de textura
		}
	}
	// Para la tapa superior	
	else if ( part == PAG_TOP_FAN ) {					
		for (unsigned int s = 0; s <= slices; s++) {	// Revoluciono segundo punto
			glm::vec3 pI(profile.getPosition(profile.size() - 2).x * cos(s*deltaRad),
						 profile.getPosition(profile.size() - 2).y,
						-profile.getPosition(profile.size() - 2).x * sin(s*deltaRad));
			
			glm::vec3 nI(normals[profile.size() - 2].x * cos(s*deltaRad),
						 normals[profile.size() - 2].y,
						 -normals[profile.size() - 2].x * sin(s*deltaRad));

			aux.position = pI;
			aux.normal = glm::normalize(nI);

			top->addPosNorm(aux);  // Añado coordenadas de posicion y normal
			top->addTexture(glm::vec2( (float) cos(s*deltaRad) / 2 + 0.5,
				(float) -sin(s*deltaRad) / 2 + 0.5)); // Añado coordenada de textura
		}

		aux.position = glm::vec3(0.0, profile.getPosition(profile.size()-1).y, 0.0);;
		aux.normal = glm::normalize(normals[normals.size()-1]);

		top->addPosNorm(aux);	// Añado coordenadas de posicion y normal
		top->addTexture(glm::vec2(0.5, 0.5));	// Añado coordenada de textura
	}
	// Para el cuerpo
	else {										
		int start = 0, end = profile.size();

		if ( profile.existTop() )
			end = profile.size() - 1;

		if ( profile.existBottom() )
			start = 1;

		std::vector<float> s, vi;
		float x, y, distance, summation_si = 0;

		for (int i = start+1; i < end; i++) { // Calculo las distancias entre los puntos
			x = abs(profile.getPosition(i).x - profile.getPosition(i-1).x);
			y = abs(profile.getPosition(i).y - profile.getPosition(i-1).y);
			distance = glm::sqrt((x*x) + (y*y));
			summation_si += distance;
			s.push_back(distance);
		}

		float summation_sa = 0;
		for (int i = 0; i < end-start-1; i++) { // Calculo los vi para la textura
			summation_sa += s[i];
			vi.push_back(summation_sa / summation_si);
		}

		for (int i = start; i < end; i++) { // Revoluciono los puntos
			for (unsigned int s = 0; s <= slices; s++) {
				glm::vec3 pI(profile.getPosition(i).x * cos(s*deltaRad), profile.getPosition(i).y,
							-profile.getPosition(i).x * sin(s * deltaRad));
				
				glm::vec3 nI(normals[i].x * cos(s * deltaRad), normals[i].y,
							 -normals[i].x * sin(s * deltaRad));
				
				aux.position = pI;
				aux.normal = glm::normalize(nI);

				body->addPosNorm(aux); // Añado coordenadas de posicion y normal

				glm::vec2 texture;

				if (i == start) {
					texture = glm::vec2 ((float)s / slices, 0.0f);
				} else {
					texture = glm::vec2 ((float)s / slices, vi[i-start-1]);
				}

				body->addTexture(texture);	 // Añado coordenada de textura
			}
		}
	}
}

// - Método que da valores a las tangentes de la parte pasada por parámetro
//   Creo la geometría para el perfil de revolución. Tanto tapas como cuerpo.
void PagRevolutionObject::computeTangents(PagRevObjParts part, unsigned int slices) {
	double deltaRad = (2 * glm::pi<double>()) / slices;

	// Para la tapa inferior
	if (part == PAG_BOTTOM_FAN) {
		for (unsigned int s = 0; s <= slices; s++) {
			bottom->addTangent(glm::vec3(-sin(s*deltaRad), 0.0, -cos(s*deltaRad)));
		}
	}
	// Para la tapa superior	
	else if (part == PAG_TOP_FAN) {
		for (unsigned int s = 0; s <= slices; s++) {
			top->addTangent(glm::vec3(-sin(s*deltaRad), 0.0, -cos(s*deltaRad)));
		}
	}
	// Para el cuerpo
	else {
		int start = 0, end = profile.size();

		if (profile.existTop())
			end = profile.size() - 1;

		if (profile.existBottom())
			start = 1;

		for (int i = start; i < end; i++) { // Revoluciono los puntos
			for (unsigned int s = 0; s <= slices; s++) {
				body->addTangent(glm::vec3(-sin(s*deltaRad), 0, -cos(s*deltaRad)));
			}
		}
	}
}

// - Método para insertar la relación entre vértices, dando lugar así a la topología
void PagRevolutionObject::defineTopology(PagRevObjParts part, unsigned int slices) {
	// Para la tapa inferior
	if (part == PAG_BOTTOM_FAN) {
		defineTMesh(PAG_BOTTOM_FAN, slices);
		defineCloud(PAG_BOTTOM_FAN, slices);
		defineWireFrame(PAG_BOTTOM_FAN, slices);
	}
	// Para la tapa superior	
	else if (part == PAG_TOP_FAN) {
		defineTMesh(PAG_TOP_FAN, slices);
		defineCloud(PAG_TOP_FAN, slices);
		defineWireFrame(PAG_TOP_FAN, slices);
	}
	// Para el cuerpo
	else {
		defineTMesh(PAG_BODY, slices);
		defineCloud(PAG_BODY, slices);
		defineWireFrame(PAG_BODY, slices);
	}
}

// - Método para definir la topologia en modo malla de triángulos
void PagRevolutionObject::defineTMesh(PagRevObjParts part, unsigned int slices) {
	// Para la tapa inferior
	if (part == PAG_BOTTOM_FAN) {
		for (unsigned int s = 1; s <= slices; s++) {
			bottom->addIndexTMesh(0);
			bottom->addIndexTMesh(s + 1);
			bottom->addIndexTMesh(s);
			bottom->addIndexTMesh(0xFFFFFFFF);
		}
	}
	// Para la tapa superior	
	else if (part == PAG_TOP_FAN) {
		for (unsigned int s = 0; s < slices; s++) {
			top->addIndexTMesh(slices + 1);
			top->addIndexTMesh(s);
			top->addIndexTMesh(s + 1);
			top->addIndexTMesh(0xFFFFFFFF);
		}
	}
	// Para el cuerpo
	else {
		int height = body->getPosNorm().size() / (slices + 1);

		for (unsigned int s = 0; s < slices; s++) {
			for ( int i = 0; i < height - 1; i++ ) {
				body->addIndexTMesh((i * (slices + 1)) + s);
				body->addIndexTMesh((i * (slices + 1)) + (s + 1));
				body->addIndexTMesh(((i + 1) * (slices + 1)) + (s));
				body->addIndexTMesh(0xFFFFFFFF);

				body->addIndexTMesh((i * (slices + 1)) + s + 1);
				body->addIndexTMesh(((i + 1) * (slices + 1)) + (s) + 1);
				body->addIndexTMesh(((i + 1) * (slices + 1)) + (s));
				body->addIndexTMesh(0xFFFFFFFF);
			}
		}
	}
}

// - Método para definir la topologia en modo nube de puntos
void PagRevolutionObject::defineCloud(PagRevObjParts part, unsigned int slices) {
	// Para la tapa inferior
	if (part == PAG_BOTTOM_FAN) {
		for (unsigned int i = 0; i < bottom->getPosNorm().size(); i++) {
			bottom->addIndexCloud(i);
		}
	}
	// Para la tapa superior	
	else if (part == PAG_TOP_FAN) {
		for (unsigned int i = 0; i < top->getPosNorm().size(); i++) {
			top->addIndexCloud(i);
		}
	}
	// Para el cuerpo
	else {
		for (unsigned int i = 0; i < body->getPosNorm().size(); i++) {
			body->addIndexCloud(i);
		}
	}
}

// - Método para definir la topologia en modo alambre
void PagRevolutionObject::defineWireFrame(PagRevObjParts part, unsigned int slices) {
	// Para la tapa inferior
	if (part == PAG_BOTTOM_FAN) {
		for (unsigned int s = 0; s <= slices; s++) {
			bottom->addIndexWireFrame(s + 1);
		}

		bottom->addIndexWireFrame(0xFFFFFFFF);

		for (unsigned int s = 0; s <= slices; s++) {
			bottom->addIndexWireFrame(0);
			bottom->addIndexWireFrame(s + 1);
			bottom->addIndexWireFrame(0xFFFFFFFF);
		}
	}
	// Para la tapa superior	
	else if (part == PAG_TOP_FAN) {
		for (unsigned int s = 0; s <= slices; s++) {
			top->addIndexWireFrame(s);
		}

		top->addIndexWireFrame(0xFFFFFFFF);

		for (unsigned int s = 0; s <= slices; s++) {
			top->addIndexWireFrame(top->getPosNorm().size() - 1);
			top->addIndexWireFrame(s);
			top->addIndexWireFrame(0xFFFFFFFF);
		}
	}
	 //Para el cuerpo
	else {
		int height = body->getPosNorm().size() / (slices + 1);
		
		for (int i = 0; i < height; i++) {
			for (unsigned int s = 0; s < slices; s++) {
				body->addIndexWireFrame(i*slices + s + i);
				body->addIndexWireFrame(i*slices + s + i + 1);
				body->addIndexWireFrame(0xFFFFFFFF);
			}
		}

		for (int s = 0; s < slices; s++) {
			for (unsigned int i = 0; i < height - 1; i++) {
				body->addIndexWireFrame(i * (slices + 1) + s);
				body->addIndexWireFrame((i+1) * (slices + 1) + s);
				body->addIndexWireFrame(0xFFFFFFFF);
			}
		}
	}
}


/* - Método para dibujar la geometria y topologia como nube de puntos. */
void PagRevolutionObject::drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject) {
	if (has(PAG_TOP_FAN))
		top->drawPoints();

	if (has(PAG_BODY))
		body->drawPoints();

	if (has(PAG_BOTTOM_FAN))
		bottom->drawPoints();
}

/* - Método para dibujar la geometria y topologia como lineas. */
void PagRevolutionObject::drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject) {
	if (has(PAG_TOP_FAN))
		top->drawLines();

	if (has(PAG_BODY))
		body->drawLines();

	if (has(PAG_BOTTOM_FAN))
		bottom->drawLines();
}

/* - Método para dibujar la geometria y topologia como triangulos sombreados. */
void PagRevolutionObject::drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v) {
	if (has(PAG_TOP_FAN))
		top->drawTriangles(PAG_TOP_FAN);

	if (has(PAG_BODY))
		body->drawTriangles(PAG_BODY);

	if (has(PAG_BOTTOM_FAN))
		bottom->drawTriangles(PAG_BOTTOM_FAN);
}

void PagRevolutionObject::drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject) {
	if (has(PAG_TOP_FAN))
		top->drawTriangles(PAG_TOP_FAN);

	if (has(PAG_BODY))
		body->drawTriangles(PAG_BODY);

	if (has(PAG_BOTTOM_FAN))
		bottom->drawTriangles(PAG_BOTTOM_FAN);
}