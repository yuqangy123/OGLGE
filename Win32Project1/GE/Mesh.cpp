#include "stdafx.h"
#include "Mesh.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{

}

bool Mesh::loadMesh(const char* filename)
{
	clear();

	bool ret = false;

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);
	if (pScene)
	{
		ret = initFromScene(pScene, filename);
	}
	else
	{
		printf("error parsing %s : %s\r\n", filename, importer.GetErrorString());
	}

}

void Mesh::clear()
{

}

bool Mesh::initFromScene(const aiScene* scene, const char* filename)
{
	m_Entries.resize(scene->mNumMeshes);
	for (int n = 0; n < m_Entries.size(); ++n)
	{
		const aiMesh* m = scene->mMeshes[n];
		InitMesh(n, m);
	}

	return InitMaterials(scene, filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
	std::vector<Vertex> vertices;
	std::vector<float> indices;

	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (int n = 0; n < paiMesh->mNumVertices; ++n)
	{
		const aiVector3D& pos = paiMesh->mVertices[n];
		const aiVector3D& normal = paiMesh->HasNormals() ? paiMesh->mNormals[n] : zero;
		const aiVector3D& texCoords = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][n] : zero;

		vertices.push_back(Vertex(Vector3f(pos.x, pos.y, pos.z), Vector2f(texCoords.x, texCoords.y), Vector3f(normal.x, normal.y, normal.z)));
	}

	for (int n = 0; n < paiMesh->mNumFaces; ++n)
	{
		const aiFace& face = paiMesh->mFaces[n];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	
	m_Entries[Index].Init(vertices, indices);
}

bool Mesh::InitMaterials(const aiScene* pScene, const char* filename)
{
	if (!pScene->HasMaterials())
		return true;

	for (int n = 0; n < pScene->mNumMaterials; ++n)
	{
		aiMaterial* material = pScene->mMaterials[n];
		aiString path;
		if (aiReturn_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
		{
			m_Textures[n] = new Texture(aiTextureType_DIFFUSE, path.data);
			m_Textures[n]->load();
		}
	}	
}

Mesh::Texture::Texture(aiTextureType tp, const char* path_)
{
	type = tp;
	path.assign(path_);

	
}

void Mesh::Texture::load()
{
	
	//TextureMgr->loadTextureJpeg
}

void Mesh::Texture::bind(int bid)
{
	TextureMgr->bindTexture(id);
}

Mesh::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

Mesh::MeshEntry::~MeshEntry()
{
	/*
	if (VB != 0)
	{
		glDeleteBuffers(1, &VB);
	}
	if (IB != 0)
	{
		glDeleteBuffers(1, &IB);
	}
	*/
}


bool Mesh::MeshEntry::Init(std::vector<Mesh::Vertex>& Vertices, std::vector<float>& Indices)
{

	Vertices.at(0);
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	std::vector<Mesh::Vertex>::iterator verItr = Vertices.begin();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex)*Vertices.size(), &(*verItr), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	std::vector<float>::iterator indItr = Indices.begin();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*Indices.size(), &(*indItr), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	NumIndices = Indices.size();
}