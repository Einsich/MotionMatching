#pragma once
#include <iostream>
#include <string>
#include "Glad/include/glad/glad.h"
#include <memory>
using namespace std;

#define log_error(format, args...) fprintf(stderr, format "\n", ##args)

#define log_debug(format, args...) fprintf(stdout, format "\n", ##args)

#define F4FMT "[%f, %f, %f, %f]\n"
#define F4ARG(v) v[0], v[1], v[2], v[3]

#define log_mat4x4(mat) \
{ mat4x4 _m_ = (mat); fprintf(stdout, "%s\n" F4FMT F4FMT F4FMT F4FMT, #mat, F4ARG(_m_[0]), F4ARG(_m_[1]), F4ARG(_m_[2]), F4ARG(_m_[3])); }
const static string projectPath = "/home/yan/Documents/MotionMatching";
const static string shadersFolderPath = "/home/yan/Documents/MotionMatching/Shaders";
const static string resourcesFolderPath = "/home/yan/Documents/MotionMatching/Resources";

#define join_path(path1, path2) ((path1) + "/" + (path2))

#define  join_recources_path(local_path) (resourcesFolderPath + "/" + (local_path))

