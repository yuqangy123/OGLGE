#pragma once
#include "MeshNode.h"
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "Importer.hpp"

class ModelMesh : public MeshNode
{
public:
	ModelMesh();
	~ModelMesh();

	bool loadMesh(const char* filename);

	bool isLoaded(){ return m_loaded; }

	void clear();

	void draw();

	void setAttriPositionLoc(int loc) { positionLoc = loc; };
	void setAttriTexCoordLoc(int loc) { texCoordLoc = loc; };
	void setAttriNormalLoc(int loc) { normalLoc = loc; };

	void setDrawBoundbox(bool bl);

protected:
	bool initFromScene(const aiScene* scene, const char* filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const char* filename);

public:
	struct Texture
	{
		Texture(aiTextureType tp, const std::string& path_);
		void load();
		void bind();

		int id;
		aiTextureType type;
		std::string path;
	};
	struct Vertex
	{
		Vertex(const Vector3f& pos_, const Vector3f& uv_, const Vector3f& normal_)
		{
			pos = pos_;
			uv = uv_;
			normal = normal_;
		}
		
		Vector3f pos;
		Vector3f uv;
		Vector3f normal;
	};
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();
		void Init(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices, GLenum pel);
		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
		GLenum drawPel;
	};

public:
	Vector3 position;
	Vector3 scale;

protected:
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	std::string m_meshFileName;
	
	int positionLoc;
	int texCoordLoc;
	int normalLoc;

	bool m_loaded;

	Assimp::Importer m_importer;

	bool m_draw_framebox;
	boundBoxData* m_boundboxData;
};