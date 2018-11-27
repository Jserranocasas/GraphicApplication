#version 400

in vec4 destinationColor;

out vec4 fColor;

void main() {
	vec2 coord = vec2(0.5);

	if( length (coord - gl_PointCoord) > 0.5){
		discard;
	}

	fColor = destinationColor;
}