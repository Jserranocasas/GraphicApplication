#version 400

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

uniform int debugOption;

layout (location = 0) out vec4 fColor;

#define M_PI 3.1415926535897932384626433832795

void main(){
	if(debugOption == 0){ // Modo Normales
		fColor = vec4(normal, 1.0);
	}

	if(debugOption == 1){ // Modo Tangentes
		fColor = vec4(tangent, 1.0);
	}

	if(debugOption == 2){ // Modo Textura
		fColor = vec4(vec3(texCoord, 1.0), 1.0);
	}

	if(debugOption == 3){ // Modo Magico
		fColor = vec4( cos(normal.z * 5 * M_PI), cos(position.y * 0.6 * M_PI), cos(normal.x * 5 * M_PI), 1.0);
	}
}