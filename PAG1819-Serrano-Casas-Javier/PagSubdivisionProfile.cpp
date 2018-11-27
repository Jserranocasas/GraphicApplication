#include "PagSubdivisionProfile.h"
#include "gtc/epsilon.hpp"

PagSubdivisionProfile::PagSubdivisionProfile() : top(false), bottom(false) {}

// - Constructor parametrizado de la clase PagSudivisionProfile. Se le pasa
//	 un vector de puntos y comprueba si el perfil contiene puntos erróneos, 
//   en cuyo caso los descarta. También examina si ademas tiene tapas o no.
PagSubdivisionProfile::PagSubdivisionProfile(std::vector<glm::vec2> _points) {
	for ( glm::vec2 point : _points ) {
		if ( point.x >= 0 )	 // Descarta puntos erróneos
			points.push_back(point);
	}

	//Comprueba si tiene tapa inferior	
	bottom = (glm::epsilonEqual<double>(points[0].x, 0.0, glm::epsilon<double>())) ? true : false;

	//Comprueba si tiene tapa superior
	top = (glm::epsilonEqual<double>(points[points.size()-1].x, 0.0, glm::epsilon<double>())) ? true : false;
}


// - Operador de asignación de la clase PagSubdivision.
PagSubdivisionProfile& PagSubdivisionProfile::operator=(const PagSubdivisionProfile &orig) {
	points.clear();

	if ( this != &orig ) {
		points = orig.points;
		top = orig.top;
		bottom = orig.bottom;
	}

	return *this;
}

// - Destructor de la clase PagSubdivisionProfile.
PagSubdivisionProfile::~PagSubdivisionProfile(){
}

// ---------------------------- Métodos Getters -----------------------------
bool PagSubdivisionProfile::existBottom() {
	return bottom;
}

bool PagSubdivisionProfile::existTop() {
	return top;
}

glm::vec2 PagSubdivisionProfile::getPosition(int index) {
	return points[index];
}

std::vector<glm::vec2> PagSubdivisionProfile::getPositions() {
	return points;
}
// --------------------------------------------------------------------------

// - Método genera un nuevo perfil de subdivisión, aplicando times veces el
//	 algoritmo de subdivisión.
PagSubdivisionProfile PagSubdivisionProfile::subdivide(unsigned int times) {
	PagSubdivisionProfile profile(points);
	for (unsigned int i = 0; i < times; i++) {
		profile = subdivision(profile);
	}
	return profile;
}

// - Método de subdivisión que devuelve un array con los nuevos puntos subdivididos.
PagSubdivisionProfile PagSubdivisionProfile::subdivision(PagSubdivisionProfile p) {
	PagSubdivisionProfile subProfile(p);
	auto it = subProfile.points.begin();

	glm::vec2 hiPrevius( (p.points[0].x + p.points[1].x) / 2.0,
						 (p.points[0].y + p.points[1].y) / 2.0 );
	
	subProfile.points.insert(++it, hiPrevius);

	for (unsigned int i = 1; i < p.points.size() - 1; i++) {
		glm::vec2 hi( (p.points[i].x + p.points[i + 1].x) / 2.0,
					  (p.points[i].y + p.points[i + 1].y) / 2.0 );

		glm::vec2 piPrime( (p.points[i].x * 6 + p.points[i - 1].x + p.points[i + 1].x) / 8.0,
						   (p.points[i].y * 6 + p.points[i - 1].y + p.points[i + 1].y) / 8.0 );

		it = subProfile.points.begin();
		subProfile.points.insert(it + i * 2 + 1, hi);
		subProfile.points[i * 2] = piPrime;
	}

	return subProfile;
}

// - Calcula las normales de los puntos del perfil.
std::vector<glm::vec3> PagSubdivisionProfile::getNormals() {
	std::vector<glm::vec3> normals;
	glm::vec3 nI; // Normal en i

	if (bottom) {
		nI = glm::vec3(0.0, -1.0, 0.0);
	}
	else {
		nI = glm::vec3((points[1].y - points[0].y), -(points[1].x - points[0].x), 0.0);
	}

	glm::normalize(nI);
	normals.push_back(nI);

	for (unsigned int i = 1; i < points.size() - 1; i++) {
		glm::vec3 nsI((points[i + 1].y - points[i].y), -(points[i + 1].x - points[i].x), 0.0);
		glm::vec3 nsIPrevius((points[i].y - points[i - 1].y), -(points[i].x - points[i - 1].x), 0.0);
		
		// Defino la normal para el punto i
		nI = glm::vec3((nsIPrevius.x + nsI.x) / 2.0, (nsIPrevius.y + nsI.y) / 2.0, 0.0);
		glm::normalize(nI);
		normals.push_back(nI);
	}

	if (top) {
		nI = glm::vec3(0.0, 1.0, 0.0);
	}
	else {
		nI = glm::vec3((points[points.size() - 1].y - points[points.size() - 2].y),
			-(points[points.size() - 1].x - points[points.size() - 2].x), 0.0);
	}

	glm::normalize(nI);
	normals.push_back(nI);

	return normals;
}

// - Método que devuelve el tamaño del vector de puntos.
int PagSubdivisionProfile::size() {
	return points.size();
}