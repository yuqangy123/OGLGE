#include "stdafx.h"
#include "Mesh.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{

}

void Mesh::draw()
{
	m_tech->enable();
	

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);
	glEnableVertexAttribArray(m_tech->normalLoc);

	for (auto itr = m_Entries.begin(); itr != m_Entries.end(); ++itr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, itr->VB);
		
		
		
		m_Textures[itr->MaterialIndex]->bind(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

		glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);
		const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
		glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
		glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
		glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
		glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
		glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);
		
		const unsigned int VertexSize = sizeof(Vertex);
		glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, pos));
		glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, uv));
		glVertexAttribPointer(m_tech->normalLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, itr->IB);
		glDrawElements(GL_TRIANGLES, itr->NumIndices, GL_UNSIGNED_INT, 0);

		
	}
	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);
	glDisableVertexAttribArray(m_tech->normalLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
}

void Mesh::update(float ft)
{
	m_MVPMt4 = *m_pipe.GetTrans();
}

bool Mesh::loadMesh(const char* filename)
{
	clear();

	m_tech = new lightTechnique();
	m_tech->init(lightType::diffuseLight);

	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);
	m_pipe.WorldPos(0, 0, -5.0);

	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(1.0f, 0.0f, 0.0f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 3.0f;

	
	bool ret = false;

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (pScene)
	{
		ret = initFromScene(pScene, filename);
	}
	else
	{
		printf("error parsing %s : %s\r\n", filename, importer.GetErrorString());
	}

	return InitMaterials(pScene, filename);
	
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
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (int n = 0; n < paiMesh->mNumVertices; ++n)
	{
		const aiVector3D& pos = paiMesh->mVertices[n];
		const aiVector3D& normal = paiMesh->HasNormals() ? paiMesh->mNormals[n] : zero;
		aiVector3D texCoords = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][n] : zero;

		vertices.push_back(Vertex(Vector3f(pos.x, pos.y, pos.z), Vector3f(texCoords.x, texCoords.y, texCoords.z), Vector3f(normal.x, normal.y, normal.z)));
	}

	for (int n = 0; n < paiMesh->mNumFaces; ++n)
	{
		const aiFace& face = paiMesh->mFaces[n];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	


	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
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
			char texName[256] = { 0 };
			sprintf(texName, "content/%s", path.data);
			auto tex = new Texture(aiTextureType_DIFFUSE, texName);
			tex->load();
			m_Textures.push_back(tex);
		}
	}

	return true;
}

Mesh::Texture::Texture(aiTextureType tp, const std::string& path_)
{
	type = tp;
	path = path_;	
}

void Mesh::Texture::load()
{
	id = TextureMgr->loadTextureJpeg(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
	
	
}

void Mesh::Texture::bind(int bid)
{
	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	glActiveTexture(GL_TEXTURE0);
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


void Mesh::MeshEntry::Init(std::vector<Mesh::Vertex>& Vertices, std::vector<unsigned int>& Indices)
{
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	std::vector<Mesh::Vertex>::iterator verItr = Vertices.begin();
	size_t verSize = sizeof(Mesh::Vertex)*Vertices.size();
	void* verPtr = &(*verItr);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Verticess), Verticess, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, verSize, verPtr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	std::vector<unsigned int>::iterator indItr = Indices.begin();
	size_t indSize = sizeof(unsigned int)*Indices.size();
	void* indPtr = &(*indItr);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, indPtr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	NumIndices = Indices.size();
}