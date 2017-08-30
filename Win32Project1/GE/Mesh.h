#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"



class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadMesh(const char* filename);

	void clear();

protected:
	bool initFromScene(const aiScene* scene, const char* filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const char* filename);

public:
	struct Texture
	{
		Texture(aiTextureType tp, const char* path_);
		void load();
		void bind(int bid);

		int id;
		aiTextureType type;
		std::string path;
	};
	struct Vertex
	{
		Vertex(const Vector3f& pos_, const Vector2f& uv_, const Vector3f& normal_)
		{
			pos = pos_;
			uv = uv_;
			normal = normal_;
		}
		
		Vector3f pos;
		Vector2f uv;
		Vector3f normal;
	};
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();
		bool Init(std::vector<Vertex>& Vertices, std::vector<float>& Indices);
		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

protected:
	

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
};
