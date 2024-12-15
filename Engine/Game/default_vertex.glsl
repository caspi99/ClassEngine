#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 pos;
out vec3 normal;
out vec2 uv0;


void main()
{
	gl_Position = proj * view * model * vec4(vertex_position, 1.0); 
	pos = vec3(model * vec4(vertex_position, 1.0));
	uv0 = vertex_uv0;
	normal = vertex_normal;
}
