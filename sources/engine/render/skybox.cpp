#include <vector>
#include "skybox.h"
#include <type_registration.h>

ECS_REGISTER_TYPE(SkyBox, SkyBox, false, false);

SkyBox::SkyBox()
{
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
	skyboxMesh = Mesh(indices, vertecs);
}

void SkyBox::render() const
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	material->get_shader().use();
	material->bind_textures_to_shader();
	skyboxMesh.render();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}