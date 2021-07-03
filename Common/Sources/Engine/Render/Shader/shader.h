#pragma once
#include "common.h"
#include "3dmath.h"
#include <vector>
#include "glad/glad.h"
#define BAD_PROGRAM 0
class Shader
{
private:
	int shaderIdx;
public:
	Shader(){}
	Shader(const std::string &shader_name, GLuint shader_program, bool update_list = false);
	explicit operator bool() const
	{ 
		return get_shader_program() != BAD_PROGRAM;
  }
	void use() const;
	GLuint get_shader_program() const;
	
	void set_mat3x3(const char*name, const mat3x3 &matrix, bool transpose = false) const
	{
		glUniformMatrix3fv(glGetUniformLocation(get_shader_program(), name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat3x3(const char*name, const vector<mat3x3> &matrices, bool transpose = false) const
	{
		glUniformMatrix3fv(glGetUniformLocation(get_shader_program(), name), matrices.size() , transpose, glm::value_ptr(matrices[0]));
	}
	void set_mat3x3(const char*name, vector<mat3x3>::iterator &begin, vector<mat3x3>::iterator &end, bool transpose = false) const
	{
		glUniformMatrix3fv(glGetUniformLocation(get_shader_program(), name), end - begin , transpose, glm::value_ptr(*begin));
	}

	void set_mat4x4(const char*name, const mat4x4 matrix, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(get_shader_program(), name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat4x4(const char*name, const vector<mat4x4> &matrices, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(get_shader_program(), name), matrices.size() , transpose, glm::value_ptr(matrices[0]));
	}
	void set_mat4x4(const char*name, vector<mat4x4>::iterator begin, vector<mat4x4>::iterator end, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(get_shader_program(), name), end - begin , transpose, glm::value_ptr(*begin));
	}
	void set_mat4x4(const char*name, vector<mat4x4>::const_iterator begin, vector<mat4x4>::const_iterator end, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(get_shader_program(), name), end - begin , transpose, glm::value_ptr(*begin));
	}

	void set_float(const char*name, const float &v) const
	{
		glUniform1fv(glGetUniformLocation(get_shader_program(), name), 1, &v);
  }
	void set_float(const char*name, const vector<float> &matrices) const
	{
		glUniform1fv(glGetUniformLocation(get_shader_program(), name), matrices.size(), matrices.data());
	}
	void set_float(const char*name, vector<float>::iterator &begin, vector<float>::iterator &end) const
	{
		glUniform1fv(glGetUniformLocation(get_shader_program(), name), end - begin, &begin[0]);
	}

	void set_vec2(const char*name, const vec2 &v) const
	{
		glUniform2fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec2(const char*name, const vector<vec2> &matrices) const
	{
		glUniform2fv(glGetUniformLocation(get_shader_program(), name), matrices.size(), glm::value_ptr(matrices[0]));
	}
	void set_vec2(const char*name, vector<vec2>::iterator &begin, vector<vec2>::iterator &end) const
	{
		glUniform2fv(glGetUniformLocation(get_shader_program(), name), end - begin, glm::value_ptr(*begin));
	}

	void set_vec3(const char*name, const vec3 &v) const
	{
		glUniform3fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec3(const char*name, const vector<vec3> &matrices) const
	{
		glUniform3fv(glGetUniformLocation(get_shader_program(), name), matrices.size(), glm::value_ptr(matrices[0]));
	}
	void set_vec3(const char*name, vector<vec3>::iterator &begin, vector<vec3>::iterator &end) const
	{
		glUniform3fv(glGetUniformLocation(get_shader_program(), name), end - begin, glm::value_ptr(*begin));
	}
	void set_vec3(const char*name, vector<vec3>::const_iterator &begin, vector<vec3>::const_iterator &end) const
	{
		glUniform3fv(glGetUniformLocation(get_shader_program(), name), end - begin, glm::value_ptr(*begin));
	}

	void set_vec4(const char*name, const vec4 &v) const
	{
		glUniform4fv(glGetUniformLocation(get_shader_program(), name), 1, glm::value_ptr(v));
  }
	void set_vec4(const char*name, const vector<vec4> &matrices) const
	{
		glUniform4fv(glGetUniformLocation(get_shader_program(), name), matrices.size(), glm::value_ptr(matrices[0]));
	}
	void set_vec4(const char*name, vector<vec4>::iterator &begin, vector<vec4>::iterator &end) const
	{
		glUniform4fv(glGetUniformLocation(get_shader_program(), name), end - begin, glm::value_ptr(*begin));
	}
};

Shader get_shader(std::string shader_name);
