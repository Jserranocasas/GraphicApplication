#include "PagListMaterials.h"

// - Inicializa el singleton.
PagListMaterials * PagListMaterials::instance = nullptr;

// Métodos constructores -----------------------------------------
PagListMaterials::PagListMaterials() {}

// Métodos -------------------------------------------------------
PagListMaterials::~PagListMaterials() {
}

// - Acceder al singleton.
PagListMaterials * PagListMaterials::getInstance() {
	if (!instance) {
		instance = new PagListMaterials();
	}
	return instance;
}

// - Método que prepara los materiales 
void PagListMaterials::insertMaterial(PagMaterial * _materials) {
	materials.push_back(*_materials);
}

// - Obtiene un material correspondiente a un index
PagMaterial* PagListMaterials::getMaterial(int i) {
	return &materials[i];
}