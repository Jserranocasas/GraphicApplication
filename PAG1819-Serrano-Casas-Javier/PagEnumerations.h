#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

/* Enumerador para las partes de un objeto de revolución*/
enum PagRevObjParts : int {
	PAG_BODY = 0, 
	PAG_TOP_FAN = 1,
	PAG_BOTTOM_FAN = 2 
};

/* Enumerador para la API de movimientos de camara*/
enum CameraMovement : int {
	PAG_WITHOUT_MOVE = 1,
	PAG_ORBIT = 2,
	PAG_PAN = 3,
	PAG_TRUCK = 4,
	PAG_DOLLY = 5,
	PAG_CRANE = 6
};

/* Enumerador para el tamaño inicial del viewport */
enum SizeViewport {
	PAG_INIT_WIDTH = 1024,
	PAG_INIT_HEIGHT = 576
};

/* Enumerado para el tipo de luz */
enum LightType : int {
	PAG_DIRECTIONAL = 0,
	PAG_SPOT = 1,
	PAG_POINT = 2,
	PAG_AMBIENT = 3
};