#version 400

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float cosGamma;

uniform float c1;
uniform float c2;
uniform float c3;

uniform vec3 Ks;
uniform float Shininess;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float exponent;

uniform sampler2D TexSamplerColor;

layout (location = 0) out vec4 FragColor;

#define M_PI 3.1415926535897932384626433832795

float attenuation(float dis) {
	return min( 1.0 / (c1 + c2*dis + c3*dis*dis), 1.0 );
}

subroutine vec4 selectAdsType();
subroutine uniform selectAdsType selectAdsUniform;

subroutine(selectAdsType)
vec4 adsAmbient() {
	vec4 texColor = texture(TexSamplerColor, texCoord);
	vec3 Kad = texColor.rgb;

	return vec4(Ia * Kad, 1.0);
}

subroutine(selectAdsType)
vec4 adsPoint() {
	vec4 texColor = texture(TexSamplerColor, texCoord);
	vec3 Kad = texColor.rgb;

	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = normalize( lightPosition - position );
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );
		
	vec3 diffuse = (Id * Kad * max( dot(l,n), 0.0));
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}
	
	float a = attenuation( distance(position, lightPosition) );

	return vec4( a * (diffuse + specular), 1.0);
}

subroutine(selectAdsType)
vec4 adsDirectional() {
	vec4 texColor = texture(TexSamplerColor, texCoord);
	vec3 Kad = texColor.rgb;

	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = - lightDirection;
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );

	vec3 diffuse = (Id * Kad * max( dot(l,n), 0.0) );
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}

	vec3 nPlano = lightDirection;
	float num = abs((nPlano.x * position.x) + (nPlano.y * position.y) + (nPlano.z * position.z) + 1);
	float dem = sqrt( pow(nPlano.x,2.0) + pow(nPlano.y,2.0) + pow(nPlano.z,2.0) );
	float dis = num / dem;

	float a = attenuation(dis);

	return vec4(a * (diffuse + specular), 1.0);
}

subroutine(selectAdsType)
vec4 adsSpot() {
	vec4 texColor = texture(TexSamplerColor, texCoord);
	vec3 Kad = texColor.rgb;

	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = normalize( lightPosition - position );
	vec3 d = lightDirection;
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );

	float spotFactor = 0.0;

	if (cos(dot(-l, d)) < cosGamma) { 
		spotFactor = pow(dot(-l, d), exponent) ; 
	}

	vec3 diffuse = (Id * Kad * max( dot(l,n), 0.0));
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}

	float a = attenuation( distance(position, lightPosition) );

	return vec4(spotFactor * a * (diffuse + specular), 1.0);
}

void main() {
	FragColor = selectAdsUniform();
}