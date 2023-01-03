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
	Shader() :shaderIdx(-1){}
	Shader(int shaderIdx):shaderIdx(shaderIdx){}
	Shader(const std::string &shader_name, GLuint shader_program, bool compiled, bool update_list = false);
	explicit operator bool() const
	{
		return get_shader_program() != BAD_PROGRAM;
  }
	void use() const;
	GLuint get_shader_program() const;
	const std::string& get_name() const;
	const ShaderBuffer &get_instance_data() const;
	const std::vector<SamplerUniform> &get_samplers() const;

	int get_uniform_location(const char *name)
	{
		return glGetUniformLocation(get_shader_program(), name);
	}
	void set_mat3x3(const char*name, const mat3 &matrix, bool transpose = false) const
	{
		glUniformMatrix3fv(glGetUniformLocation(get_shader_program(), name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat3x3(int uniform_location, const mat3 &matrix, bool transpose = false) const
	{
		glUniformMatrix3fv(uniform_location, 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat4x4(const char*name, const mat4 matrix, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(get_shader_program(), name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat4x4(int uniform_location, const mat4 matrix, bool transpose = false) const
	{
		glUniformMatrix4fv(uniform_location, 1, transpose, glm::value_ptr(matrix));
	}

	void set_float(const char*name, const float &v) const
	{
		glUniform1fv(glGetUniformLocation(get_shader_program(), name), 1, &v);
  }
	void set_float(int uniform_location, const float &v) const
	{
		glUniform1fv(uniform_location, 1, &v);
  }
	void set_int(const char*name, int v) const
	{
		glUniform1i(glGetUniformLocation(get_shader_program(), name), v);
  }
	void set_int(int uniform_location, int v) const
	{
		glUniform1i(uniform_location, v);
  }

	void set_vec2(const char*name, const vec2 &v) const
	{
		glUniform2fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec2(int uniform_location, const vec2 &v) const
	{
		glUniform2fv(uniform_location, 1, glm::value_ptr(v));
  }

	void set_vec3(const char*name, const vec3 &v) const
	{
		glUniform3fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec3(const char*name, const std::vector<vec3> &v) const
	{
		if (!v.empty())
			glUniform3fv(glGetUniformLocation(get_shader_program(), name), v.size(), glm::value_ptr(v[0]));
  }
	void set_vec3(const char*name, size_t size, const vec3 *data) const
	{
		if (size > 0)
			glUniform3fv(glGetUniformLocation(get_shader_program(), name), size, (const float*)data);
  }
	void set_vec3(int uniform_location, const vec3 &v) const
	{
		glUniform3fv(uniform_location, 1, glm::value_ptr(v));
  }
	void set_vec3(int uniform_location, const std::vector<vec3> &v) const
	{
		if (!v.empty())
			glUniform3fv(uniform_location, v.size(), glm::value_ptr(v[0]));
  }

	void set_vec4(const char*name, const vec4 &v) const
	{
		glUniform4fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec4(int uniform_location, const vec4 &v) const
	{
		glUniform4fv(uniform_location, 1, glm::value_ptr(v));
  }
};

Shader get_shader(const std::string &shader_name, bool with_log = true);
const std::vector<const char*>get_shaders_names();
