#include "mesh.h"

Mesh::Mesh(VertexArrayObject vao):
vertexArrayObject(vao)
{

}
Mesh::Mesh(const aiMesh *mesh)
{
  numVert = mesh->mNumVertices;
  numFaces = mesh->mNumFaces;
  if (mesh->HasPositions())
  {
    positions.resize(numVert);
    for (int i = 0; i < numVert; i++)
      positions[i] = to_vec3(mesh->mVertices[i]);
  }
  if (mesh->HasNormals())
  {
    normals.resize(numVert);
    for (int i = 0; i < numVert; i++)
      normals[i] = to_vec3(mesh->mNormals[i]);
  }
  if (mesh->HasFaces())
  {
    indices.resize(numFaces * 3);
    for (int i = 0; i < numFaces; i++)
    { 
      assert(mesh->mFaces[i].mNumIndices == 3);
      for (int j = 0; j < 3; j++)
        indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
    }
  }
  if (mesh->HasTextureCoords(0))
  {
    uvs.resize(numVert);
    for (int i = 0; i < numVert; i++)
      uvs[i] = to_vec2(mesh->mTextureCoords[0][i]);
  }
  if (mesh->HasBones())
  {
    weights.resize(numVert);
    weightsIndex.resize(numVert);
    int numBones = mesh->mNumBones;
    vector<int> weightsOffset(numVert, 0);
    for (int i = 0; i < numBones; i++)
    {
      const aiBone * bone = mesh->mBones[i];
      bonesMap[string(bone->mName.C_Str())] = i;

      for (uint j = 0; j < bone->mNumWeights; j++)
      {
        int vertex = bone->mWeights[j].mVertexId;
        int offset = weightsOffset[vertex]++;
        weights[vertex][offset] = bone->mWeights[j].mWeight;
        weightsIndex[vertex][offset] = i;
      }
    }
  }
  if (indices.size() && positions.size())
  {
    vertexArrayObject = VertexArrayObject(indices, positions, normals, uvs, weights, weightsIndex);
  }
}
void Mesh::render(bool wire_frame)
{
  vertexArrayObject.render(wire_frame);
}

MeshPtr make_mesh(VertexArrayObject vao)
{
  return make_shared<Mesh>(vao);
}
MeshPtr make_mesh(const aiMesh *mesh)
{
  return make_shared<Mesh>(mesh);
}