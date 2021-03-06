#include <vector>
#include <string>
#include "skybox.h"
#include "CommonCode/Shader/shader.h"
#include "CommonCode/Texture/textures.h"
#include "CommonCode/Camera/camera.h"
#include "CommonCode/Mesh/vertex_array_object.h"
static CubeMap skybox;
static Shader skyboxShader;
static VertexArrayObject skyboxVAO;
void create_sky_box(string path_to_folder)
{
	skybox = CubeMap(path_to_folder);
	skyboxShader = get_shader("sky_box");
	vector<unsigned int> indices;
	vector<vec3> vertecs;
  for (int face = 0; face < 3; face++)
	{
		for (int d = -1; d <= 1; d += 2)
		{
				int ind = vertecs.size();
				float a = -1, b = -1, ta, tb;
				for (int i = 0; i < 4; i++)
				{
						vec3 v;
						v[face] = d;
						v[(face + 1) % 3] = a;
						v[(face + 2) % 3] = b;

						ta = -b * d;
						tb = a * d;
						a = ta;
						b = tb;
						vertecs.push_back(v);
				}
				indices.push_back(ind); indices.push_back(ind + 2);indices.push_back(ind + 1);
				indices.push_back(ind); indices.push_back(ind + 3); indices.push_back(ind + 2);
		}
	}
	skyboxVAO = VertexArrayObject(indices, vertecs);
}

void render_sky_box()
{
	CameraPtr camera = main_camera();
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	skyboxShader.use();
	camera->set_to_shader(skyboxShader, true);

	skybox.bind(skyboxShader, "skybox");
	skyboxVAO.render();
	skybox.unbind();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	
}
