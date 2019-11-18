#version 400

layout (location = 0) in vec3 vPosition;

uniform float minHeight;
uniform float maxHeight;
uniform vec3 minColor;
uniform vec3 maxColor;
uniform float pointSize;
uniform mat4 mModelViewProj;

out vec4 destinationColor;

void main(){

	if(vPosition.y < minHeight){
		destinationColor = vec4(minColor, 1.0);
	}
	else if (vPosition.y > maxHeight) {
		destinationColor = vec4(maxColor, 1.0);
	}
	else {
		float factor = (maxHeight - vPosition.y) / (maxHeight - minHeight);
		destinationColor = vec4(minColor * factor + (1-factor) * maxColor, 1.0);		
	}
	
	gl_PointSize = pointSize;
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);

}