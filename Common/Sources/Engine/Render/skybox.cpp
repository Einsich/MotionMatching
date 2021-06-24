#include <vector>
#include "skybox.h"
#include "../camera.h"
SkyBox::SkyBox(string path_to_folder):path(path_to_folder)
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
				indices.push_back(ind); indices.push_back(ind + 1);indices.push_back(ind + 2);
				indices.push_back(ind); indices.push_back(ind + 2); indices.push_back(ind + 3);
		}
	}
	skyboxVAO = VertexArrayObject(indices, vertecs);
}

void SkyBox::render(const mat4 view_projection, const vec3 &camera_position, bool wire_frame) 
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	skyboxShader.use();
	set_camera_to_shader(skyboxShader, view_projection, camera_position);

	skybox.bind(skyboxShader, "skybox");
	skyboxVAO.render(wire_frame);
	skybox.unbind();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	
}
template<>
void edit_component(SkyBox &component, const char *name)
{
  ImGui::Text("%s : %s", name, component.path.c_str());
  ImGui::Spacing();
}