#pragma once

#include "PagMaterial.h"
#include <vector>

/*
 * @brief   Clase que encapsula todos los materiales en un singleton
 * @file	PagListMaterials.h
 * @author	Javier Serrano Casas
 * @date	25-11-2018
 */
class PagListMaterials {
public:
	~PagListMaterials();

	// - M�todo para acceder al singleton. 
	static PagListMaterials *getInstance();

	// - Este m�todo prepara los materiales
	void insertMaterial(PagMaterial *_materials);

	PagMaterial* getMaterial(int i);
private:
	// - Constructor es privado
	PagListMaterials();

	// - Singleton. �nica instancia que se tiene en la aplicaci�n.
	static PagListMaterials *instance;

	// - List de materiales.
	std::vector<PagMaterial> materials; // Lista de materiales
};