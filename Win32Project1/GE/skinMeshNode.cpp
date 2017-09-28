#include "stdafx.h"
#include "skinMeshNode.h"
#include "anim.h"
#include "postprocess.h"
#include "TextureManager.h"

#define aiMatrix4x4_Matrix4f(xx4, x4)\
{\
x4.m[0][0] = xx4.a1; x4.m[0][1] = xx4.a2; x4.m[0][2] = xx4.a3; x4.m[0][3] = xx4.a4;\
x4.m[1][0] = xx4.b1; x4.m[1][1] = xx4.b2; x4.m[1][2] = xx4.b3; x4.m[1][3] = xx4.b4;\
x4.m[2][0] = xx4.c1; x4.m[2][1] = xx4.c2; x4.m[2][2] = xx4.c3; x4.m[2][3] = xx4.c4;\
x4.m[3][0] = xx4.d1; x4.m[3][1] = xx4.d2; x4.m[3][2] = xx4.d3; x4.m[3][3] = xx4.d4;\
}

skinMeshNode::skinMeshNode()
{
}


skinMeshNode::~skinMeshNode()
{

}

void skinMeshNode::draw()
{

	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texCoordLoc);
	glEnableVertexAttribArray(weightsLoc);
	glEnableVertexAttribArray(boneIDsLoc);
	glEnable(GL_TEXTURE_2D);


	for (auto itr = m_Entries.begin(); itr != m_Entries.end(); ++itr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, itr->VB);
		
		if (m_Textures.size() > itr->MaterialIndex && itr->MaterialIndex >= 0)
			m_Textures[itr->MaterialIndex]->bind();
		//glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

		const unsigned int VertexSize = sizeof(Vertex);
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, pos));
		glVertexAttribPointer(texCoordLoc, 3, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, uv));
		glVertexAttribPointer(weightsLoc, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)offsetof(Vertex, vertexBone.Weights));
		glVertexAttribIPointer(boneIDsLoc, NUM_BONES_PER_VEREX, GL_UNSIGNED_INT, VertexSize, (GLvoid*)offsetof(Vertex, vertexBone.IDs));
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, itr->IB);
		glDrawElements(GL_TRIANGLES, itr->NumIndices, GL_UNSIGNED_INT, 0);				
	}
	glDisableVertexAttribArray(positionLoc);
	glDisableVertexAttribArray(texCoordLoc);
	glDisableVertexAttribArray(weightsLoc);
	glDisableVertexAttribArray(boneIDsLoc);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}


bool skinMeshNode::loadMesh(const char* filename)
{
	if (m_loaded)
		return false;
	m_loaded = true;

	clear();

	bool ret = false;

	m_aiScene = m_importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (m_aiScene)
	{
		ret = initFromScene(m_aiScene, filename);
	}
	else
	{
		printf("error parsing %s : %s\r\n", filename, m_importer.GetErrorString());
		assert(0);
	}

	return ret;
	
}

void skinMeshNode::clear()
{

}

bool skinMeshNode::initFromScene(const aiScene* scene, const char* filename)
{
	m_Entries.resize(scene->mNumMeshes);
	for (int n = 0; n < m_Entries.size(); ++n)
	{
		const aiMesh* m = scene->mMeshes[n];
		InitMesh(n, m);
	}
	

	return InitMaterials(scene, filename);
}

void skinMeshNode::InitMesh(unsigned int Index, const aiMesh* paiMesh)
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
	
	loadBone(paiMesh, vertices);

	
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
	m_Entries[Index].Init(Index, vertices, indices);
	int baseVertex = 0, baseIndex = 0;
	for (int i = 0; i < Index; ++i)
	{
		baseVertex += m_Entries[i].NumVertex;
		baseIndex += m_Entries[i].NumIndices;
	}
	m_Entries[Index].BaseIndex = baseIndex;
	m_Entries[Index].BaseVertex = baseVertex;
}

void skinMeshNode::loadBone(const aiMesh* pMesh, std::vector<skinMeshNode::Vertex>& vertexs)
{
	if (pMesh->HasBones())
	{
		for (int i = 0; i < pMesh->mNumBones; ++i)
		{
			aiBone* bone = pMesh->mBones[i];
			int boneIndex = 0;
			std::string boneName(bone->mName.C_Str());
			if (m_bonesMapping.find(boneName) == m_bonesMapping.end())
			{
				BoneInfo b;
				b.name = boneName;
				m_bones.push_back(b);
				m_bonesMapping[boneName] = m_bones.size() - 1;
			}
			boneIndex = m_bonesMapping[boneName];
			aiMatrix4x4_Matrix4f(bone->mOffsetMatrix, m_bones[boneIndex].boneOffsetMat4);

			for (int j = 0; j < bone->mNumWeights; ++j)
			{
				unsigned int vertexId = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;
				if (vertexId > vertexs.size())
				{
					printf("mesh bone vertexId is greater than vertexs size");
					assert(0);
				}
				for (int k = 0; k < NUM_BONES_PER_VEREX; ++k)
				{
					if (vertexs[vertexId].vertexBone.IDs[k] == 0)
					{
						vertexs[vertexId].vertexBone.IDs[k] = boneIndex;
						vertexs[vertexId].vertexBone.Weights[k] = weight;
					}
				}				
			}
		}

		m_bonesTransforms.resize(m_bones.size());
		//get finalTransformationMat4
	}
}

bool skinMeshNode::InitMaterials(const aiScene* pScene, const char* filename)
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

skinMeshNode::Texture::Texture(aiTextureType tp, const std::string& path_)
{
	type = tp;
	path = path_;	
}

void skinMeshNode::Texture::load()
{

	
	if (std::string::npos != path.rfind(".bmp"))
	{
		id = TextureMgr->loadTextureBmp(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
	}
	else if (std::string::npos != path.rfind(".jpg"))
	{
		id = TextureMgr->loadTextureJpeg(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
	}	
	else if (std::string::npos != path.rfind(".tga"))
	{
		id = TextureMgr->loadTextureTga(path.c_str(), GL_RGBA, GL_RGBA, 0, 0);
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

void skinMeshNode::Texture::bind()
{
	// Set the filtering mode
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(id);
}

skinMeshNode::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
	BaseIndex = 0;
	BaseVertex = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

skinMeshNode::MeshEntry::~MeshEntry()
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


void skinMeshNode::MeshEntry::Init(unsigned int index, std::vector<skinMeshNode::Vertex>& Vertices, std::vector<unsigned int>& Indices)
{
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	std::vector<skinMeshNode::Vertex>::iterator verItr = Vertices.begin();
	size_t verSize = sizeof(skinMeshNode::Vertex)*Vertices.size();
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
	NumVertex = Vertices.size();
}

void skinMeshNode::BoneTransform(double TimeInSeconds, std::vector<Matrix4f>& Transforms)
{
	aiAnimation* aianim = m_aiScene->mAnimations[0];
	double durTime = aianim->mTicksPerSecond == 0 ? 25.f : aianim->mTicksPerSecond;
	double ticksTime = TimeInSeconds * durTime;
	float timeTicks = fmod(ticksTime, (float)aianim->mDuration);

	Matrix4f transformMat4;
	transformMat4.identity();
	ReadNodeHeirarchy(timeTicks, m_aiScene->mRootNode, transformMat4);

	
	for (int i = 0; i < m_bones.size(); ++i)
	{
		Transforms[i] = m_bones[i].finalTransformationMat4;
	}
}

void CalcInterpolatedScaling(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}
	int startn = 0;
	int endn = 0;
	float endTime = 0.f;
	for (int j = 0; j < pNodeAnim->mNumScalingKeys; ++j)
	{
		aiVectorKey& key = pNodeAnim->mScalingKeys[j];
		if (key.mTime >= AnimationTime)
		{
			endn = j;
			break;
		}
		startn = j;
	}

	aiVector3D deltaValue = pNodeAnim->mScalingKeys[endn].mValue - pNodeAnim->mScalingKeys[startn].mValue;
	float deltaTime = pNodeAnim->mScalingKeys[endn].mTime - AnimationTime;
	float ttime = pNodeAnim->mScalingKeys[endn].mTime - pNodeAnim->mScalingKeys[startn].mTime;
	Out=pNodeAnim->mScalingKeys[startn].mValue + deltaValue*(deltaTime / ttime);
}


unsigned int FindRotation(double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime <= pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime <= (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}
void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void skinMeshNode::ReadNodeHeirarchy(long long timeTicks, aiNode* nd, Matrix4f& parentTransform)
{
	std::string ndName(nd->mName.C_Str());
	unsigned int bIndex = m_bonesMapping[ndName];
	Matrix4f nodeTransformMt4;
	aiMatrix4x4_Matrix4f(nd->mTransformation, nodeTransformMt4);

	//更新animation里的关键帧里相关骨骼的转换矩阵
	aiNodeAnim* nodeAnim = nullptr;
	aiAnimation* aiAnim = m_aiScene->mAnimations[0];
	for (int i = 0; i < aiAnim->mNumChannels; ++i)
	{
		aiNodeAnim* anim = aiAnim->mChannels[i];
		std::string name = anim->mNodeName.C_Str();
		if (ndName == name)
		{
			nodeAnim = anim;
			break;
		}
	}

	if (nodeAnim != nullptr)
	{
		aiVector3D salValue;
		CalcInterpolatedScaling(salValue, timeTicks, nodeAnim);
		Matrix4f salMt4;
		salMt4.InitScaleTransform(salValue.x, salValue.y, salValue.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, timeTicks, nodeAnim);
		Matrix4f RotationM;
		aiMatrix3x3t<float> mat = RotationQ.GetMatrix();
		RotationM.m[0][0] = mat.a1; RotationM.m[0][1] = mat.a2; RotationM.m[0][2] = mat.a3;
		RotationM.m[1][0] = mat.b1; RotationM.m[1][1] = mat.b2; RotationM.m[1][2] = mat.b3;
		RotationM.m[2][0] = mat.c1; RotationM.m[2][1] = mat.c2; RotationM.m[2][2] = mat.c3;
		RotationM.m[3][3] = 1.0f;

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, timeTicks, nodeAnim);
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		nodeTransformMt4 = TranslationM*RotationM*salMt4;
	}
	nodeTransformMt4 = parentTransform*nodeTransformMt4;

	m_bones[bIndex].finalTransformationMat4 = nodeTransformMt4;

	for (int i = 0; i<nd->mNumChildren; ++i)
	{
		ReadNodeHeirarchy(timeTicks, nd->mChildren[i], nodeTransformMt4);
	}
}

void skinMeshNode::playAnimation(const char* anim, long long  time)
{
	m_playAnimName.assign(anim);
	m_animTime = time;
	m_playing = true;
}

void skinMeshNode::update(float ft)
{
	if (m_playing)
	{
		BoneTransform(ft, m_bonesTransforms);
	}
	
}
const std::vector<Matrix4f>& skinMeshNode::getBonesTransformMt4()
{
	return m_bonesTransforms;
	
}