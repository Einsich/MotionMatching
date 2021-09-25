#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include "glad/glad.h"
#include "shader_buffer.h"

#define BAD_PROGRAM 0
struct StorageBuffer;

class Shader
{
private:
	int shaderIdx;
public:
	Shader() = default;
	Shader(int shaderIdx):shaderIdx(shaderIdx){}
	Shader(const std::string &shader_name, GLuint shader_program, bool compiled, bool update_list = false);
	explicit operator bool() const
	{ 
		return get_shader_program() != BAD_PROGRAM;
  }
	void use() const;
	GLuint get_shader_program() const;
	const ShaderBuffer &get_instance_data() const;
	const vector<SamplerUniform> &get_samplers() const;
};

Shader get_shader(const std::string &shader_name, bool with_log = true);
const vector<const char*>get_shaders_names();
