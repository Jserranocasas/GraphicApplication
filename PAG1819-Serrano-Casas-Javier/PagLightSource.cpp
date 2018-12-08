#include "PagLightSource.h"

PagLightSource::PagLightSource(PagLightApplication *strategy) :_strategy(strategy) {
}

PagLightSource& PagLightSource::operator=(const PagLightSource& orig) {
	if (this != &orig) {
		_strategy = orig._strategy;
	}
	return *this;
}

PagLightSource::~PagLightSource() {
}

void PagLightSource::set_strategy(PagLightApplication *strategy) {
	_strategy = strategy;
}

void PagLightSource::apply(PagShaderProgram& ads, PagCamera camera) {
	_strategy->apply(ads, camera);
}

LightType PagLightSource::getType() {
	return _strategy->getType();
}