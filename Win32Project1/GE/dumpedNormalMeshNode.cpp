#include "stdafx.h"
#include "dumpedNormalMeshNode.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"

dumpedNormalMeshNode::dumpedNormalMeshNode()
{
}


dumpedNormalMeshNode::~dumpedNormalMeshNode()
{
	if (m_pNormalMapTex != nullptr)
		delete m_pNormalMapTex;
}

void dumpedNormalMeshNode::draw()
{

	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texCoordLoc);
	glEnableVertexAttribArray(normalLoc);
	glEnableVertexAttribArray(tangentLoc);
	glEnable(GL_TEXTURE_2D);

	

	for (auto itr = m_Entries.begin(); itr != m_Entries.end(); ++itr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, itr->VB);
		
		m_Textures[itr->MaterialIndex-1]->bind();
		m_pNormalMapTex->bind();
		//glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

		const unsigned int VertexSize = sizeof(Vertex);
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, pos));
		glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, uv));
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, normal));
		glVertexAttribPointer(tangentLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, tangent));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, itr->IB);
		glDrawElements(GL_TRIANGLES, itr->NumIndices, GL_UNSIGNED_INT, 0);				
	}
	glDisableVertexAttribArray(positionLoc);
	glDisableVertexAttribArray(texCoordLoc);
	glDisableVertexAttribArray(normalLoc);
	glDisableVertexAttribArray(tangentLoc);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

bool dumpedNormalMeshNode::loadMesh(const char* filename, const char* normalMapFile)
{
	if (m_loaded)
		return false;
	m_loaded = true;

	clear();

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
		assert(0);
	}

	m_pNormalMapTex = new Texture(aiTextureType_NORMALS, normalMapFile);
	m_pNormalMapTex->load();

	return ret;
	
}

void dumpedNormalMeshNode::clear()
{

}

bool dumpedNormalMeshNode::initFromScene(const aiScene* scene, const char* filename)
{
	m_Entries.resize(scene->mNumMeshes);
	for (int n = 0; n < m_Entries.size(); ++n)
	{
		const aiMesh* m = scene->mMeshes[n];
		InitMesh(n, m);
	}

	return InitMaterials(scene, filename);
}

void dumpedNormalMeshNode::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (int n = 0; n < paiMesh->mNumVertices; ++n)
	{
		const aiVector3D& pos = paiMesh->mVertices[n];
		const aiVector3D& normal = paiMesh->HasNormals() ? paiMesh->mNormals[n] : zero;
		aiVector3D texCoords = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][n] : zero;

		vertices.push_back(Vertex(Vector3(pos.x, pos.y, pos.z), Vector2(texCoords.x, texCoords.y), Vector3(normal.x, normal.y, normal.z), Vector3()));
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

bool dumpedNormalMeshNode::InitMaterials(const aiScene* pScene, const char* filename)
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

dumpedNormalMeshNode::Texture::Texture(aiTextureType tp, const std::string& path_)
{
	type = tp;
	path = path_;	
}

void dumpedNormalMeshNode::Texture::load()
{

	
	if (std::string::npos != path.rfind(".bmp"))
	{
		id = TextureMgr->loadTextureBmp(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
	}
	else if (std::string::npos != path.rfind(".jpg"))
	{
		id = TextureMgr->loadTextureJpeg(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
	}	
	else
	{
		printf("cant find support texture load lib\r\n");
		assert(0);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void dumpedNormalMeshNode::Texture::bind(GLuint texId)
{
	// Set the filtering mode
	glActiveTexture(texId);
	TextureMgr->bindTexture(id);
}

dumpedNormalMeshNode::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

dumpedNormalMeshNode::MeshEntry::~MeshEntry()
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


void dumpedNormalMeshNode::MeshEntry::Init(std::vector<dumpedNormalMeshNode::Vertex>& Vertices, std::vector<unsigned int>& Indices)
{
	Vector2 uv1, uv2;
	Vector3 edge1, edge2, tangent;
	float f;
	for (unsigned int i = 0; i < Indices.size(); i+=3)
	{
		uv1 = Vertices[i + 1].uv - Vertices[i].uv;
		uv2 = Vertices[i + 2].uv - Vertices[i].uv;
		edge1 = Vertices[i + 1].pos - Vertices[i].pos;
		edge2 = Vertices[i + 2].pos - Vertices[i].pos;

		f = 1.0f / (uv1.x*uv2.y - uv2.x*uv1.y);
		tangent.x = f*(uv2.y*edge1.x - uv1.x*edge2.x);
		tangent.y = f*(uv2.y*edge1.y - uv1.x*edge2.y);
		tangent.z = f*(uv2.y*edge1.z - uv1.x*edge2.z);

		Vertices[i + 0].tangent += tangent;
		Vertices[i + 1].tangent += tangent;
		Vertices[i + 2].tangent += tangent;
	}

	for (unsigned int i = 0; i < Indices.size(); ++i)
	{
		Vertices[i].tangent.normalize();
	}



	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	std::vector<dumpedNormalMeshNode::Vertex>::iterator verItr = Vertices.begin();
	size_t verSize = sizeof(dumpedNormalMeshNode::Vertex)*Vertices.size();
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