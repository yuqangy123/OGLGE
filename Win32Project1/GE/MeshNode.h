#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"

class MeshNode
{
public:
	MeshNode();
	~MeshNode();

	bool loadMesh(const char* filename);

	void clear();

	void draw();

	void setAttriPositionLoc(int loc) { positionLoc = loc; };
	void setAttriTexCoordLoc(int loc) { texCoordLoc = loc; };
	void setAttriNormalLoc(int loc) { normalLoc = loc; };

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
		void Init(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);
		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

public:
	Vector3 position;
	Vector3 scale;

protected:
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	
	int positionLoc;
	int texCoordLoc;
	int normalLoc;
};
