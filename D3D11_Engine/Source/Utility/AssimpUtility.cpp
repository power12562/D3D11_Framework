#include "AssimpUtility.h"
#include <Framework\SceneManager.h>
#include <GameObject\Base\GameObject.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Component/Render/SimpleBoneMeshRender.h>
#include <Component/BoneComponent.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <Math/AssimpMath.h>
#include <Utility\utfConvert.h>
#include <Material\SimpleMaterial.h>
#include <Component\TransformAnimation.h>
#include <Framework\MaterialManager.h>
#include <Math/Mathf.h>
#include <filesystem>
#include <iostream>

namespace Utility
{
	void LoadTexture(aiMaterial* ai_material, const wchar_t* directory, SimpleMaterial* material)
	{
		aiString path;
		std::wstring basePath;
		aiColor3D baseColor;

		if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
		{
			if (Utility::ParseFileName(path))
			{
				basePath = directory;
				basePath += L"\\";
				basePath += utfConvert::utf8_to_wstring(path.C_Str());
				material->SetDiffuse(basePath.c_str());
			}
		}
		else if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
		{
			Color& meshColor = material->cb_material.MaterialDiffuse;
			meshColor.x = baseColor.r;
			meshColor.y = baseColor.g;
			meshColor.z = baseColor.b;
			meshColor.w = 1.f;
		}
		if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_NORMALS, 0, &path))
		{
			if (Utility::ParseFileName(path))
			{
				basePath = directory;
				basePath += L"\\";
				basePath += utfConvert::utf8_to_wstring(path.C_Str());
				material->SetNormalMap(basePath.c_str());
			}
		}
		if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_SPECULAR, 0, &path))
		{

			if (Utility::ParseFileName(path))
			{
				basePath = directory;
				basePath += L"\\";
				basePath += utfConvert::utf8_to_wstring(path.C_Str());
				material->SetSpecularMap(basePath.c_str());
			}
		}
		if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_EMISSIVE, 0, &path))
		{
			if (Utility::ParseFileName(path))
			{
				basePath = directory;
				basePath += L"\\";
				basePath += utfConvert::utf8_to_wstring(path.C_Str());
				material->SetEmissiveMap(basePath.c_str());
			}
		}
		if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_OPACITY, 0, &path))
		{
			if (Utility::ParseFileName(path))
			{
				basePath = directory;
				basePath += L"\\";
				basePath += utfConvert::utf8_to_wstring(path.C_Str());
				material->SetOpacityMap(basePath.c_str());
			}
		}
	}

	void SetNodeTransform(const aiNode* node, GameObject* objcet)
	{
		Vector3 pos;
		Quaternion rot;
		Vector3 scale;
		AssimpMath::DecomposeTransform(node, pos, rot, scale);
		if (objcet->transform.Parent)
		{
			objcet->transform.localPosition = pos;
			objcet->transform.localRotation = rot;
			objcet->transform.localScale = scale;
		}
	}

	void SetTransformAnimation(const aiScene* pScene, GameObject& _gameObject, std::unordered_map<std::wstring, GameObject*>& addObjMap)
	{
		using Clip = TransformAnimation::Clip;

		TransformAnimation& anime = _gameObject.AddComponent<TransformAnimation>();
		for (unsigned int i = 0; i < pScene->mNumAnimations; i++)
		{
			aiAnimation* currAnimation = pScene->mAnimations[i];
			Clip clip;
			clip.Duration = (float)currAnimation->mDuration;
			clip.TickTime = (float)currAnimation->mTicksPerSecond;
			for (unsigned int j = 0; j < currAnimation->mNumChannels; j++)
			{
				aiNodeAnim* currNodeAnim = currAnimation->mChannels[j];
				using NodeAnime = Clip::NodeAnimation;
				NodeAnime nodeAnime;
				nodeAnime.objTarget = addObjMap[utfConvert::utf8_to_wstring(currNodeAnim->mNodeName.C_Str()).c_str()];
				for (unsigned int k = 0; k < currNodeAnim->mNumPositionKeys; k++)
				{
					NodeAnime::PositionKey key;
					key.position.x = currNodeAnim->mPositionKeys[k].mValue.x;
					key.position.y = currNodeAnim->mPositionKeys[k].mValue.y;
					key.position.z = currNodeAnim->mPositionKeys[k].mValue.z;
					key.Time = (float)currNodeAnim->mPositionKeys[k].mTime;
					nodeAnime.positionKeys.push_back(key);
				}
				for (unsigned int k = 0; k < currNodeAnim->mNumRotationKeys; k++)
				{
					NodeAnime::RotationKey key;
					key.rotation.x = currNodeAnim->mRotationKeys[k].mValue.x;
					key.rotation.y = currNodeAnim->mRotationKeys[k].mValue.y;
					key.rotation.z = currNodeAnim->mRotationKeys[k].mValue.z;
					key.rotation.w = currNodeAnim->mRotationKeys[k].mValue.w;

					key.Time = (float)currNodeAnim->mRotationKeys[k].mTime;
					nodeAnime.rotationKeys.push_back(key);
				}
				for (unsigned int k = 0; k < currNodeAnim->mNumScalingKeys; k++)
				{
					NodeAnime::ScaleKey key;
					key.scale.x = currNodeAnim->mScalingKeys[k].mValue.x;
					key.scale.y = currNodeAnim->mScalingKeys[k].mValue.y;
					key.scale.z = currNodeAnim->mScalingKeys[k].mValue.z;
					key.Time = (float)currNodeAnim->mScalingKeys[k].mTime;
					nodeAnime.scaleKeys.push_back(key);
				}
				clip.nodeAnimations.push_back(nodeAnime);
			}
			anime.AddClip(utfConvert::utf8_to_wstring(pScene->mAnimations[i]->mName.C_Str()).c_str(), clip);
		}
	}
}


bool Utility::ParseFileName(aiString& str)
{
	const char* findBackSlash = strrchr(str.C_Str(), '\\');
	const char* findSlash = strrchr(str.C_Str(), '/');
	const char* findDot = strrchr(str.C_Str(), '.');
	if (findBackSlash)
	{
		str.Set(findBackSlash + 1);
		return true;
	}
	else if (findSlash)
	{
		str.Set(findSlash + 1);
		return true;
	}
	else if (findDot)
	{
		return true;
	}
	else
		return false;
}

void Utility::LoadFBX(const char* path,
	GameObject& _gameObject,
	SimpleMaterial* material,
	std::function<void(SimpleMaterial*)> initMaterial,
	bool isStatic)
{
	using namespace utfConvert;

	Assimp::Importer importer;
	unsigned int importFlags =
		aiProcess_Triangulate |         // vertex 삼각형 으로 출력
		aiProcess_GenNormals |          // Normal 정보 생성  
		aiProcess_GenUVCoords |         // 텍스처 좌표 생성
		aiProcess_CalcTangentSpace |    // 탄젠트 벡터 생성
		aiProcess_PopulateArmatureData |// 본 데이터 생성
		aiProcess_LimitBoneWeights |    // 본 영향 정점 개수 제한
		aiProcess_ConvertToLeftHanded;  // DX용 왼손좌표계 변환
	if (isStatic)
		importFlags |= aiProcess_PreTransformVertices;   // 노드의 변환행렬을 적용한 버텍스 생성한다.  *사용하면 모든 메쉬가 합쳐져서 애니메이션 사용이 불가능하다.

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함

	std::string fileName(path);
	const aiScene* pScene = importer.ReadFile(fileName, importFlags);
	if (pScene == nullptr)
		return;

	std::wstring directory = utfConvert::utf8_to_wstring(fileName.substr(0, fileName.find_last_of("/\\")));

	std::queue<aiNode*> nodeQue;
	nodeQue.push(pScene->mRootNode);
	const aiNode* currNode = nullptr;

	std::queue<GameObject*> objQue;
	objQue.push(&_gameObject);
	GameObject* currObj = nullptr;

	std::unordered_map<std::wstring, GameObject*> addObjMap;
	std::unordered_map<std::wstring, int> boneIndexMap;
	int boneIndexManager = 0;
	auto getBoneIndex = [&boneIndexMap, &boneIndexManager](const std::wstring& name)->int
		{
			auto findIter = boneIndexMap.find(name);
			if (findIter != boneIndexMap.end())
				return findIter->second;
			else
			{
				boneIndexMap[name] = boneIndexManager;
				return boneIndexManager++;
			}
		};

	//count bone
	int boneCount = 0;
	for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumBones; ++j)
		{
			getBoneIndex(utf8_to_wstring(mesh->mBones[j]->mName.C_Str()));
		}
	}
	boneCount = boneIndexMap.size();
	std::shared_ptr<MatrixPallete> rootMatrixPallete = nullptr;
	if (boneCount > 0)
		rootMatrixPallete = std::make_shared<MatrixPallete>();

	std::vector<BoneComponent*> boneList(boneCount);
	std::vector<SimpleBoneMeshRender*> meshList;

	while (!nodeQue.empty())
	{
		currNode = nodeQue.front();
		nodeQue.pop();

		currObj = objQue.front();
		objQue.pop();

		addObjMap[currObj->Name] = currObj;

		if (currNode)
		{			
			if (boneCount > 0)
			{					
				SetNodeTransform(currNode, currObj);

				auto findIndex = boneIndexMap.find(utf8_to_wstring(currNode->mName.C_Str()));
				if (findIndex != boneIndexMap.end())
				{
					int index = findIndex->second;
					boneList[index] = &currObj->AddComponent<BoneComponent>();
					boneList[index]->myIndex = index;
				}

				if (currNode->mNumMeshes > 0) 
				{			
					//Create Vertex
					for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
					{
						SimpleBoneMeshRender& meshComponent = currObj->AddComponent<SimpleBoneMeshRender>();
						meshList.push_back(&meshComponent);
						
						if (material)
						{
							meshComponent.Material = material;
						}
						else
						{
							meshComponent.Material = materialManager.GetMaterial((currObj->Name + L" (" + std::to_wstring(currObj->GetInstanceID())).c_str());
						}
						initMaterial(meshComponent.Material);
						meshComponent.matrixPallete = rootMatrixPallete;
						meshComponent.Material->cbuffer.CreateVSConstantBuffers<MatrixPallete>();
						meshComponent.Material->cbuffer.BindUpdateEvent(*meshComponent.matrixPallete);

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];

						//cash vertex bone info
						std::unordered_map<int, std::vector<float>> weightsMap;
						std::unordered_map<int, std::list<std::wstring>> nameMap;
						for (unsigned int j = 0; j < pMesh->mNumBones; j++)
						{
							aiBone* pBone = pMesh->mBones[j];
							for (unsigned int k = 0; k < pBone->mNumWeights; k++)
							{
								aiVertexWeight weight = pBone->mWeights[k];
								weightsMap[weight.mVertexId].push_back(weight.mWeight);
								nameMap[weight.mVertexId].emplace_back(utfConvert::utf8_to_wstring(pBone->mName.C_Str()));				
							}
						}

						for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++)
						{
							SimpleBoneMeshRender::Vertex vertex;

							aiVector3D position = pMesh->mVertices[vertexIndex];
							vertex.position.x = position.x;
							vertex.position.y = position.y;
							vertex.position.z = position.z;
							vertex.position.w = 1.0f;

							aiVector3D normal = pMesh->mNormals[vertexIndex];
							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;

							aiVector3D BiTangents = pMesh->mBitangents[vertexIndex];
							vertex.biTangent.x = BiTangents.x;
							vertex.biTangent.y = BiTangents.y;
							vertex.biTangent.z = BiTangents.z;

							aiVector3D texCoord = pMesh->mTextureCoords[0][vertexIndex];
							vertex.Tex.x = texCoord.x;
							vertex.Tex.y = texCoord.y;

							auto n = nameMap[vertexIndex].begin();
							for (int j = 0; j < weightsMap[vertexIndex].size(); j++)
							{
								if(vertex.BlendWeights[j] != 0 )
									__debugbreak();

								vertex.BlendWeights[j] = weightsMap[vertexIndex][j];
								vertex.BlendIndecses[j] = getBoneIndex(*n);
								n++;
							}

							meshComponent.vertices.push_back(vertex);	 							
						}

						//Create Index
						for (unsigned int faceIndex = 0; faceIndex < pMesh->mNumFaces; faceIndex++)
						{
							const aiFace& face = pMesh->mFaces[faceIndex];
							for (unsigned int numIndex = 0; numIndex < face.mNumIndices; numIndex++)
							{
								meshComponent.indices.push_back(face.mIndices[numIndex]);
							}
						}

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), meshComponent.Material);

						//offsetMatrices
						meshComponent.offsetMatrices.resize(boneCount);
						for (unsigned int i = 0; i < pMesh->mNumBones; i++)
						{
							aiMatrix4x4 ai_matrix = pMesh->mBones[i]->mOffsetMatrix;
							std::wstring name = utf8_to_wstring(pMesh->mBones[i]->mName.C_Str());
							meshComponent.offsetMatrices[getBoneIndex(name)] = Matrix(&ai_matrix.a1).Transpose();
						}
						
						//CreateBuffer
						meshComponent.CreateMesh();
					}
				}
			}
			else
			{
				SetNodeTransform(currNode, currObj);

				if (currNode->mNumMeshes > 0)
				{
					//Create Vertex
					for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
					{
						SimpleMeshRender& meshComponent = currObj->AddComponent<SimpleMeshRender>();

						if (material)
						{
							meshComponent.Material = material;
						}
						else
						{
							meshComponent.Material = materialManager.GetMaterial((currObj->Name + L" (" + std::to_wstring(currObj->GetInstanceID())).c_str());
						}
						initMaterial(meshComponent.Material);

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];
						SimpleMeshRender::Vertex vertex;
						for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++)
						{
							aiVector3D position = pMesh->mVertices[vertexIndex];
							vertex.position.x = position.x;
							vertex.position.y = position.y;
							vertex.position.z = position.z;
							vertex.position.w = 1.0f;

							aiVector3D normal = pMesh->mNormals[vertexIndex];
							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;

							aiVector3D BiTangents = pMesh->mBitangents[vertexIndex];
							vertex.biTangent.x = BiTangents.x;
							vertex.biTangent.y = BiTangents.y;
							vertex.biTangent.z = BiTangents.z;

							aiVector3D texCoord = pMesh->mTextureCoords[0][vertexIndex];
							vertex.Tex.x = texCoord.x;
							vertex.Tex.y = texCoord.y;

							meshComponent.vertices.push_back(vertex);
						}

						//Create Index
						for (unsigned int faceIndex = 0; faceIndex < pMesh->mNumFaces; faceIndex++)
						{
							const aiFace& face = pMesh->mFaces[faceIndex];
							for (unsigned int numIndex = 0; numIndex < face.mNumIndices; numIndex++)
							{
								meshComponent.indices.push_back(face.mIndices[numIndex]);
							}
						}

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), meshComponent.Material);

						meshComponent.CreateMesh();
					}
				}
			}
			
			for (unsigned int i = 0; i < currNode->mNumChildren; i++)
			{
				nodeQue.push(currNode->mChildren[i]);
				std::wstring childName = utf8_to_wstring(currNode->mChildren[i]->mName.C_Str());
				GameObject* childObj = NewGameObject<GameObject>(childName.c_str());
				childObj->transform.SetParent(currObj->transform, false);

				objQue.push(childObj);
			}
		}
	}
	//set bone info
	for (auto& mesh : meshList)
	{
		mesh->boneList = boneList;
	}


	if (pScene->mAnimations)
	{
		SetTransformAnimation(pScene, _gameObject, addObjMap);
	}
}

void Utility::LoadFBX(const char* path, GameObject& _gameObject, SimpleMaterial* material, bool isStatic)
{
	LoadFBX(path, _gameObject, material, [](SimpleMaterial* material)->void { return; }, isStatic);
}