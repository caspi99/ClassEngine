#include "ModuleProgram.h"
#include "GL/glew.h"

ModuleProgram::ModuleProgram() {

}

ModuleProgram::~ModuleProgram() {

}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name) {
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	else {
		LOG("Error: Unable to open shader file %s", shader_file_name);
	}
	return data;
}

unsigned int ModuleProgram::CompileShader(unsigned int type, const char* shader_file_name)
{
	char* data = LoadShaderSource(shader_file_name);
	if (!data) {
		LOG("Error: Shader source is empty for file %s", shader_file_name);
		return 0;
	}

	unsigned int shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &data, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)	
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
		glDeleteShader(shader_id);
		shader_id = 0;
	}
	free(data);
	return shader_id;
}

unsigned int ModuleProgram::CreateProgram(const char* vertex_shader_file_name, const char* fragment_shader_file_name)
{
	unsigned int vtx_shader = CompileShader(GL_VERTEX_SHADER, vertex_shader_file_name);
	unsigned int frg_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_file_name);

	if (vtx_shader == 0 || frg_shader == 0) {
		LOG("Error: Shader compilation failed.");
		return 0;
	}

	unsigned int program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
		glDeleteProgram(program_id);
		program_id = 0;
	}

	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}
