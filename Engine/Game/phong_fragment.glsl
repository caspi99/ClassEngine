#version 460

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform vec3 kd;
uniform vec3 ks;
uniform float n;
uniform sampler2D mytexture;

in vec3 pos;
in vec2 uv0;
in vec3 normal;

out vec4 color;

void main()
{
	vec3 result;

	vec3 texColor = texture(mytexture, uv0).rgb;
	vec3 ambient = 0.1 * texColor;

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	float NL = dot(-N, L);

	if(NL > 0){
		vec3 diffuse = kd * lightColor * texColor * NL;

		vec3 R = reflect(L, N);
		vec3 V = normalize(cameraPos - pos);
		float spec = pow(max(dot(V, R), 0.0), n);
		vec3 specular = ks * lightColor * spec;
    
    	result = ambient + diffuse + specular;
	}
	else {
		result = ambient;
	}
    color = vec4(result, 1.0);
}
