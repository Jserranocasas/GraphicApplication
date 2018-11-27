#version 400

in vec3 position;
in vec3 normal;

uniform vec3 lightPosition;
uniform vec3 direction;
uniform float gamma;

uniform float c1;
uniform float c2;
uniform float c3;

uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float s;

layout (location = 0) out vec4 FragColor;

float sf(){
	vec3 l = normalize( lightPosition - position );
	vec3 d = normalize( direction );

	float delta = max( dot(-l,d), 0.0 );

	if( delta <= gamma ){
		return min( pow(delta, s), 1.0 );
	} else {
		return 0.0;
	}
}

float attenuation() {
	float dis = distance( position, lightPosition );
	return min( 1.0 / (c1+c2*dis+c3*dis), 1.0 );
}

float attenuationDir() {
	vec3 nPlano = normalize( direction );
	float num = abs((nPlano.x * position.x) + (nPlano.y * position.y) + (nPlano.z * position.z) + 1);
	float dem = sqrt( pow(nPlano.x,2.0) + pow(nPlano.y,2.0) + pow(nPlano.z,2.0) );
	float dis = num / dem;

	return min( 1.0 / (c1 + c2 * dis + c3 * dis), 1.0 );
}

subroutine vec4 selectAdsType();
subroutine uniform selectAdsType selectAdsUniform;

subroutine(selectAdsType)
vec4 adsPoint() {
	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = normalize( lightPosition - position );
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );

	vec3 ambient = Ia;
	vec3 diffuse = (Id * Kd * max( dot(l,n), 0.0));
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}

	float a = attenuationDir();
	a = attenuation();
	float sf = sf();
	sf = 1.0;
	
	return vec4(ambient + sf * a * (diffuse + specular), 1.0);
}

subroutine(selectAdsType)
vec4 adsDirectional() {
	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = normalize( -direction );
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );

	vec3 ambient = Ia;
	vec3 diffuse = (Id * Kd * max( dot(l,n), 0.0) );
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}

	float a = attenuation();
	a = attenuationDir();
	float sf = sf();
	sf = 1.0;

	return vec4(ambient + sf * a * (diffuse + specular), 1.0);
}

subroutine(selectAdsType)
vec4 adsSpot() {
	vec3 n;
	
	if (gl_FrontFacing) {
		n = normalize( normal );
	} else {
		n = normalize( -normal );
	}

	vec3 l = normalize( lightPosition - position );
	vec3 v = normalize( -position );
	vec3 r = reflect( -l, n );

	vec3 ambient = Ia;
	vec3 diffuse = (Id * Kd * max( dot(l,n), 0.0));
	vec3 specular;

	if (dot(l, n) < 0.0) {
		specular = vec3(0.0);
	} else {
		specular = (Is * Ks * pow( max( dot(r,v), 0.0), Shininess ));
	}

	float a = attenuationDir();
	a = attenuation();
	float sf = sf();

	return vec4(ambient + sf * a * (diffuse + specular), 1.0);
}

void main() {
	FragColor = selectAdsUniform();
}