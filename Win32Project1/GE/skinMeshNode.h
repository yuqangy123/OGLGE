#pragma once
#include <vector>
#include <map>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "Importer.hpp"

class skinMeshNode
{
public:
#define NUM_BONES_PER_VEREX 4
	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];
		VertexBoneData()
		{
			for (int n = 0; n < NUM_BONES_PER_VEREX; ++n)
			{
				IDs[n] = 0;
				Weights[n] = 0.0f;
			}
		}
	};
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
		Vertex() {};
		Vertex(const Vector3f& pos_, const Vector3f& uv_, const Vector3f& normal_)
		{
			pos = pos_;
			uv = uv_;
			normal = normal_;
		}

		Vector3f pos;
		Vector3f uv;
		Vector3f normal;
		VertexBoneData vertexBone;

	};
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();
		void Init(unsigned int index, std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);
		GLuint VB;
		GLuint IB;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int NumVertex;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};
	struct BoneInfo {
		std::string name;
		Matrix4f finalTransformationMat4;
		Matrix4f boneOffsetMat4;
	};


public:
	skinMeshNode();
	~skinMeshNode();

	bool loadMesh(const char* filename);

	bool isLoaded(){ return m_loaded; }

	void clear();

	void update(float ft);

	void draw();

	void playAnimation(const char* anim, long long time);

	//获取骨骼的转换矩阵
	const std::vector<Matrix4f>& getBonesTransformMt4();

protected:
	bool initFromScene(const aiScene* scene, const char* filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const char* filename);
	void BoneTransform(double TimeInSeconds, std::vector<Matrix4f>& Transforms);
	void loadBone(const aiMesh* pMesh, std::vector<skinMeshNode::Vertex>& vertexs);
	void ReadNodeHeirarchy(long long timeTicks, aiNode* nd, Matrix4f& parentTransform);

public:
	Vector3 position;
	Vector3 scale;
	int positionLoc = 0;
	int texCoordLoc = 0;
	int weightsLoc = 0;
	int boneIDsLoc = 0;

	std::vector<Matrix4f> m_bonesTransforms;

protected:
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	std::vector<BoneInfo> m_bones;//保存所有骨骼信息
	std::map<std::string, unsigned int> m_bonesMapping;
	
	
	bool m_loaded = false;

	const aiScene* m_aiScene;
	Assimp::Importer m_importer;

	float m_animTime = 0.0f;

	std::string m_playAnimName;

	Matrix4f m_rootNodeMat4;

	bool m_playing = false;
};
