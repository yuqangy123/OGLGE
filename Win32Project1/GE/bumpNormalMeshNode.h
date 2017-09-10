#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"

class bumpNormalMeshNode
{
public:
	bumpNormalMeshNode();
	~bumpNormalMeshNode();

	bool loadMesh(const char* filename, const char* normalMapFile);

	bool isLoaded(){ return m_loaded; }

	void clear();

	void draw();

	void setAttriPositionLoc(int loc) { positionLoc = loc; };
	void setAttriTexCoordLoc(int loc) { texCoordLoc = loc; };
	void setAttriNormalLoc(int loc) { normalLoc = loc; };
	void setAttriTangentLoc(int loc) { tangentLoc = loc; };
	void setUniformTextureLoc(int loc) { s_textureLoc = loc; };
	void setUniformNormalTextureLoc(int loc) { s_normalTextureLoc = loc; };
	
	
protected:
	bool initFromScene(const aiScene* scene, const char* filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const char* filename);

public:
	struct Texture
	{
		Texture(aiTextureType tp, const std::string& path_);
		void load();
		void bind(GLuint texId=GL_TEXTURE0);

		int id;
		aiTextureType type;
		std::string path;
	};
	struct Vertex
	{
		Vertex(const Vector3& pos_, const Vector2& uv_, const Vector3& normal_, const Vector3& tangent_)
		{
			pos = pos_;
			uv = uv_;
			normal = normal_;
			tangent = tangent_;
		}
		
		Vector3 pos;
		Vector2  uv;
		Vector3 normal;
		Vector3 tangent;
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
	int tangentLoc;

	int s_textureLoc;
	int s_normalTextureLoc;

	Texture* m_pNormalMapTex = nullptr;

	bool m_loaded = false;
};
