#include "PagListMaterials.h"

// - Inicializa el singleton.
PagListMaterials * PagListMaterials::instance = nullptr;

// M�todos constructores -----------------------------------------
PagListMaterials::PagListMaterials() {}

// M�todos -------------------------------------------------------
PagListMaterials::~PagListMaterials() {
}

// - Acceder al singleton.
PagListMaterials * PagListMaterials::getInstance() {
	if (!instance) {
		instance = new PagListMaterials();
	}
	return instance;
}

// - M�todo que prepara los materiales 
void PagListMaterials::insertMaterial(PagMaterial * _materials) {
	materials.push_back(*_materials);
}

// - Obtiene un material correspondiente a un index
PagMaterial* PagListMaterials::getMaterial(int i) {
	return &materials[i];
}