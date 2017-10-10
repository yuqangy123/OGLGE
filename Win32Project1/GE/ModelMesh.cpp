#include "stdafx.h"
#include "ModelMesh.h"
#include "anim.h"
#include "postprocess.h"
#include "TextureManager.h"
#include <math.h>
#include <cfloat>

ModelMesh::ModelMesh()
{
	m_loaded = false;
	m_draw_framebox = false;
	m_boundboxData = nullptr;
}


ModelMesh::~ModelMesh()
{

}

void ModelMesh::draw()
{
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texCoordLoc);
	glEnableVertexAttribArray(normalLoc);
	glEnable(GL_TEXTURE_2D);

	for (auto itr = m_Entries.begin(); itr != m_Entries.end(); ++itr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, itr->VB);
		
		if (m_Textures.size() >= itr->MaterialIndex && itr->MaterialIndex > 0)
			m_Textures[itr->MaterialIndex-1]->bind();
		//glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

		const unsigned int VertexSize = sizeof(Vertex);
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, pos));
		glVertexAttribPointer(texCoordLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, uv));
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, itr->IB);
		glDrawElements(itr->drawPel, itr->NumIndices, GL_UNSIGNED_INT, 0);			
	}
	glDisableVertexAttribArray(positionLoc);
	glDisableVertexAttribArray(texCoordLoc);
	glDisableVertexAttribArray(normalLoc);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


bool ModelMesh::loadMesh(const char* filename)
{
	if (m_loaded)
		return false;

	m_loaded = true;
	
	m_meshFileName.assign(filename);

	clear();

	bool ret = false;

	
	const aiScene* pScene = m_importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (pScene)
	{
		ret = initFromScene(pScene, filename);
	}
	else
	{
		printf("error parsing %s : %s\r\n", filename, m_importer.GetErrorString());
		assert(0);
	}

	return ret;
	
}

void ModelMesh::clear()
{

}

bool ModelMesh::initFromScene(const aiScene* scene, const char* filename)
{
	m_Entries.resize(scene->mNumMeshes);
	for (int n = 0; n < m_Entries.size(); ++n)
	{
		const aiMesh* m = scene->mMeshes[n];
		InitMesh(n, m);
	}

	return InitMaterials(scene, filename);
}

void ModelMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	float maxz = FLT_MIN, minz = FLT_MAX, maxy = FLT_MIN, miny = FLT_MAX, maxx = FLT_MIN, minx = FLT_MAX;

	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (int n = 0; n < paiMesh->mNumVertices; ++n)
	{
		const aiVector3D& pos = paiMesh->mVertices[n];
		const aiVector3D& normal = paiMesh->HasNormals() ? paiMesh->mNormals[n] : zero;
		aiVector3D texCoords = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][n] : zero;

		vertices.push_back(Vertex(Vector3f(pos.x, pos.y, pos.z), Vector3f(texCoords.x, texCoords.y, texCoords.z), Vector3f(normal.x, normal.y, normal.z)));

		if (pos.x>maxx)maxx = pos.x;
		if (pos.x<minx)minx = pos.x;
		if (pos.y>maxy)maxy = pos.y;
		if (pos.y<miny)miny = pos.y;
		if (pos.z>maxz)maxz = pos.z;
		if (pos.z<minz)minz = pos.z;
	}
	m_boundboxData.init(maxx, minx, maxy, miny, maxz, minz);

	for (int n = 0; n < paiMesh->mNumFaces; ++n)
	{
		const aiFace& face = paiMesh->mFaces[n];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	


	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
	m_Entries[Index].Init(vertices, indices, GL_TRIANGLES);

	
}

bool ModelMesh::InitMaterials(const aiScene* pScene, const char* filename)
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

ModelMesh::Texture::Texture(aiTextureType tp, const std::string& path_)
{
	type = tp;
	path = path_;	
}

void ModelMesh::Texture::load()
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

void ModelMesh::Texture::bind()
{
	// Set the filtering mode
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(id);
}

ModelMesh::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

ModelMesh::MeshEntry::~MeshEntry()
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


void ModelMesh::MeshEntry::Init(std::vector<ModelMesh::Vertex>& Vertices, std::vector<unsigned int>& Indices, GLenum pel)
{
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	std::vector<ModelMesh::Vertex>::iterator verItr = Vertices.begin();
	size_t verSize = sizeof(ModelMesh::Vertex)*Vertices.size();
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
	drawPel = pel;
}

void ModelMesh::setDrawBoundbox(bool bl)
{
	if(bl == m_draw_framebox)
		return;
	m_draw_framebox = bl;
	GLenum drawPel = GL_LINES;

	if (!bl)
	{
		for (auto itr = m_Entries.begin(); itr != m_Entries.end(); ++itr)
		{
			if (itr->drawPel == drawPel)
			{
				glDeleteBuffers(1, &itr->VB);
				glDeleteBuffers(1, &itr->IB);
				m_Entries.erase(itr);
				break;
			}
		}
		return ;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.push_back(Vertex(m_boundboxData.left_top_front, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.right_top_front, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.right_bottom_front, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.left_bottom_front, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.left_top_back, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.right_top_back, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.right_bottom_back, Vector3f(0.5f,0.5f,0.5f), Vector3f()));
	vertices.push_back(Vertex(m_boundboxData.left_bottom_back, Vector3f(0.5f,0.5f,0.5f), Vector3f()));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(4);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(6);

	MeshEntry entry;
	entry.MaterialIndex = 1;
	entry.Init(vertices, indices, drawPel);

	m_Entries.push_back(entry);
}

