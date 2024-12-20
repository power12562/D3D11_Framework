#include "AssimpUtility.h"
#include <Light/SimpleDirectionalLight.h>
#include <Manager/SceneManager.h>
#include <GameObject\Base\GameObject.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Component/Render/SimpleBoneMeshRender.h>
#include <Component/Bone/BoneComponent.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <Math/AssimpMath.h>
#include <Utility\utfConvert.h>
#include <Material\BlingPhongMaterial.h>
#include <Component/Animation/TransformAnimation.h>
#include <Manager\ResourceManager.h>
#include <Manager/HLSLManager.h>

#include <Component/Render/PBRMeshRender.h>
#include <Component/Render/PBRBoneMeshRender.h>
#include <Component/Render/BlingPhongMeshRender.h>
#include <Component/Render/BlingPhongBoneMeshRender.h>

#include <Math/Mathf.h>
#include <filesystem>
#include <iostream>

namespace Utility
{
	static GameObject* NewMeshObject(
		SURFACE_TYPE surface, 
		const wchar_t* name)
	{
		switch (surface)
		{
		case SURFACE_TYPE::BlingPhong:
			return NewGameObject<BlingPhongMeshObject>(name);
		case SURFACE_TYPE::PBR:
			return NewGameObject<PBRMeshObject>(name);
		default:
			return NewGameObject(name);
		}
	}

	static SimpleMeshRender& AddMeshComponent(GameObject* obj, SURFACE_TYPE surface)
	{
		switch (surface)
		{
		case SURFACE_TYPE::BlingPhong:
			return obj->AddComponent<BlingPhongMeshRender>();
		case SURFACE_TYPE::PBR:
			return obj->AddComponent<PBRMeshRender>();
		default:
			return obj->AddComponent<SimpleMeshRender>();
		}

	}
	
	static SimpleBoneMeshRender& AddBoneMeshComponent(GameObject* obj, SURFACE_TYPE surface)
	{
		switch (surface)
		{
		case SURFACE_TYPE::BlingPhong:
			return obj->AddComponent<BlingPhongBoneMeshRender>();
		case SURFACE_TYPE::PBR:
			return obj->AddComponent<PBRBoneMeshRender>();
		default:
			return obj->AddComponent<SimpleBoneMeshRender>();
		}
	}

	static void SetBaseColor(GameObject& obj, Color& baseColor, SURFACE_TYPE surface)
	{
		switch (surface)
		{
		case SURFACE_TYPE::BlingPhong:
			static_cast<BlingPhongMeshObject&>(obj).Material.MaterialDiffuse = baseColor;
			return;
		case SURFACE_TYPE::PBR:
			static_cast<PBRMeshObject&>(obj).Material.Albedo = baseColor;
			return;
		default:
			return;
		}
	}

	static void LoadTexture(aiMaterial* ai_material, const wchar_t* directory, MeshRender* meshRender, SURFACE_TYPE surface)
	{
		aiString path;
		std::wstring basePath;
		{
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path) ||
				AI_SUCCESS == ai_material->GetTexture(aiTextureType_BASE_COLOR, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Albedo, basePath.c_str());
				}
			}
			else if (aiColor4D baseColor; ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
			{
				SimpleMeshRender* simpleMeshRender = reinterpret_cast<SimpleMeshRender*>(meshRender);
				
				simpleMeshRender->baseColor.x = baseColor.r;
				simpleMeshRender->baseColor.y = baseColor.g;
				simpleMeshRender->baseColor.z = baseColor.b;
				simpleMeshRender->baseColor.w = baseColor.a;

				SetBaseColor(simpleMeshRender->gameObject, simpleMeshRender->baseColor, surface);
			}
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_NORMALS, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Normal, basePath.c_str());
				}
			}
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_SPECULAR, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Specular, basePath.c_str());
				}
			}
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_EMISSIVE, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Emissive, basePath.c_str());
				}
			}
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_OPACITY, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Opacity, basePath.c_str());
					meshRender->isAlpha = true;
					
					if(meshRender->IsPSShader())
					{
						std::wstring path = HLSLManager::EngineShaderPath;
						switch (surface)
						{
						case SURFACE_TYPE::BlingPhong:
							path += L"BlingPhongOpacitiyPS.hlsl";
							meshRender->SetPixelShader(path.c_str());
							break;
						case SURFACE_TYPE::PBR:
							path += L"PBROpacitiyPS.hlsl";
							meshRender->SetPixelShader(path.c_str());
							break;
						default:
							break;
						}
					}
				}
			}

			//블랜더 기준 메탈릭 맵
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_METALNESS, 0, &path))
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Metalness, basePath.c_str());

					if (surface == SURFACE_TYPE::PBR)
					{
						static_cast<PBRMeshObject&>(meshRender->gameObject).Material.UseMetalnessMap = true;
					}
				}
			}
			else if (float scala; ai_material->Get(AI_MATKEY_METALLIC_FACTOR, scala) == AI_SUCCESS)
			{
				if (surface == SURFACE_TYPE::PBR)
				{
					static_cast<PBRMeshObject&>(meshRender->gameObject).Material.Roughness = scala;
				}
			}

			//블랜더 기준 러프니스 맵
			if (AI_SUCCESS == ai_material->GetTexture(aiTextureType_SHININESS, 0, &path)) 
			{
				if (Utility::ParseFileName(path))
				{
					basePath = directory;
					basePath += L"\\";
					basePath += utfConvert::utf8_to_wstring(path.C_Str());
					meshRender->textures.SetTexture2D(E_TEXTURE::Roughness, basePath.c_str());

					if (surface == SURFACE_TYPE::PBR)
					{
						static_cast<PBRMeshObject&>(meshRender->gameObject).Material.UseRoughnessMap = true;
					}
				}
			}
			else if (float scala; ai_material->Get(AI_MATKEY_ROUGHNESS_FACTOR, scala) == AI_SUCCESS)
			{
				if (surface == SURFACE_TYPE::PBR)
				{
					static_cast<PBRMeshObject&>(meshRender->gameObject).Material.Roughness = scala;
				}
			}
		}
	}

	static void SetNodeTransform(const aiNode* node, GameObject* objcet)
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

	static void SetTransformAnimation(const aiScene* pScene, GameObject& _gameObject, std::unordered_map<std::wstring, GameObject*>& addObjMap)
	{
		using Clip = TransformAnimation::Clip;
		using PositionKey = TransformAnimation::Clip::NodeAnimation::PositionKey;
		using RotationKey = TransformAnimation::Clip::NodeAnimation::RotationKey;
		using ScaleKey = TransformAnimation::Clip::NodeAnimation::ScaleKey;

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

				//make keyList
				if (currNodeAnim->mNumPositionKeys > 0 && !nodeAnime.positionKeys)
				{
					nodeAnime.positionKeys = std::make_shared<std::vector<PositionKey>>();
				}
				if (currNodeAnim->mNumRotationKeys > 0 && !nodeAnime.rotationKeys)
				{
					nodeAnime.rotationKeys = std::make_shared<std::vector<RotationKey>>();
				}
				if (currNodeAnim->mNumScalingKeys > 0 && !nodeAnime.scaleKeys)
				{
					nodeAnime.scaleKeys = std::make_shared<std::vector<ScaleKey>>();
				}

				for (unsigned int k = 0; k < currNodeAnim->mNumPositionKeys; k++)
				{
					NodeAnime::PositionKey key;
					key.position.x = currNodeAnim->mPositionKeys[k].mValue.x;
					key.position.y = currNodeAnim->mPositionKeys[k].mValue.y;
					key.position.z = currNodeAnim->mPositionKeys[k].mValue.z;
					key.Time = (float)currNodeAnim->mPositionKeys[k].mTime;
					nodeAnime.positionKeys->push_back(key);
				}
				for (unsigned int k = 0; k < currNodeAnim->mNumRotationKeys; k++)
				{
					NodeAnime::RotationKey key;
					key.rotation.x = currNodeAnim->mRotationKeys[k].mValue.x;
					key.rotation.y = currNodeAnim->mRotationKeys[k].mValue.y;
					key.rotation.z = currNodeAnim->mRotationKeys[k].mValue.z;
					key.rotation.w = currNodeAnim->mRotationKeys[k].mValue.w;

					key.Time = (float)currNodeAnim->mRotationKeys[k].mTime;
					nodeAnime.rotationKeys->push_back(key);
				}
				for (unsigned int k = 0; k < currNodeAnim->mNumScalingKeys; k++)
				{
					NodeAnime::ScaleKey key;
					key.scale.x = currNodeAnim->mScalingKeys[k].mValue.x;
					key.scale.y = currNodeAnim->mScalingKeys[k].mValue.y;
					key.scale.z = currNodeAnim->mScalingKeys[k].mValue.z;
					key.Time = (float)currNodeAnim->mScalingKeys[k].mTime;
					nodeAnime.scaleKeys->push_back(key);
				}
				clip.nodeAnimations.push_back(nodeAnime);
			}
			anime.AddClip(utfConvert::utf8_to_wstring(pScene->mAnimations[i]->mName.C_Str()).c_str(), clip);
		}
	}

	static GameObject* IsResource(const std::wstring& key)
	{
		std::shared_ptr<GameObject> rootObject = GetResourceManager<GameObject>().GetResource(key.c_str());
		return rootObject.get();
	}

	static bool IsBone(GameObject* obj)
	{
		std::queue<GameObject*> objQue;
		objQue.push(obj);
		GameObject* currObj = nullptr;
		while (!objQue.empty())
		{
			currObj = objQue.front();
			objQue.pop();
			if (BoneComponent* bone = currObj->IsComponent<BoneComponent>())
			{
				return true;
			}
			for (unsigned int i = 0; i < currObj->transform.GetChildCount(); i++)
			{
				objQue.push(&currObj->transform.GetChild(i)->gameObject);
			}
		}
		return false;
	}

	static void CopyFBX(GameObject* DestinationObj, GameObject* SourceObj,
		const wchar_t* key, 
		std::function<void(MeshRender*)> initMesh,
		SURFACE_TYPE surface
		)
	{
		if (DestinationObj == SourceObj)
		{
			__debugbreak(); //대상 오브젝트가 동일합니다.
			return;
		}

		std::queue<GameObject*> objSourceQue;
		objSourceQue.push(SourceObj);
		GameObject* currSourceObj = nullptr;

		std::queue<GameObject*> objDestQue;
		objDestQue.push(DestinationObj);
		GameObject* currDestObj = nullptr;

		bool isBone = IsBone(SourceObj);
		bool isRoot = true;

		std::vector<SimpleBoneMeshRender*> meshList;
		std::vector<BoneComponent*> boneList(128);
		std::vector<TransformAnimation::Clip*> clipList;
		std::unordered_map<std::wstring, GameObject*> destObjNameMap;

		while (!objSourceQue.empty())
		{
			currSourceObj = objSourceQue.front();
			objSourceQue.pop();

			currDestObj = objDestQue.front();
			objDestQue.pop();

			//copy Bounds
			currDestObj->BoundingBox = currSourceObj->BoundingBox;

			if (isRoot)
			{
				//copy Animation
				if (TransformAnimation* animation = currSourceObj->IsComponent<TransformAnimation>())
				{
					TransformAnimation* destAnime = &currDestObj->AddComponent<TransformAnimation>();
					destAnime->CopyClips(animation);
					clipList.reserve(destAnime->GetClipsCount());
					for (auto& clip : destAnime->GetClips())
					{
						clipList.push_back(const_cast<TransformAnimation::Clip*>(&clip.second));
					}
				}
			}
			else
			{
				//copy local transform
				currDestObj->transform = currSourceObj->transform;
			}

			if (isBone)
			{
				//copy bone
				if (BoneComponent* sourceBone = currSourceObj->IsComponent<BoneComponent>())
				{
					BoneComponent& destBone = currDestObj->AddComponent<BoneComponent>();
					destBone.myIndex = sourceBone->myIndex;
					boneList[destBone.myIndex] = &destBone;
				}

				//copy mesh
				for (int i = 0; i < currSourceObj->GetComponentCount(); i++)
				{
					if (SimpleBoneMeshRender* sourceMesh = currSourceObj->GetComponentAtIndex<SimpleBoneMeshRender>(i))
					{
						SimpleBoneMeshRender& destMesh = AddBoneMeshComponent(currDestObj, surface);
						destMesh.baseColor = sourceMesh->baseColor;
						SetBaseColor(destMesh.gameObject, destMesh.baseColor, surface);
						if (surface == SURFACE_TYPE::PBR && typeid(PBRMeshObject) == typeid(sourceMesh->gameObject))
						{
							static_cast<PBRMeshObject&>(destMesh.gameObject).Material = static_cast<PBRMeshObject&>(sourceMesh->gameObject).Material;
						}
						destMesh.isAlpha = sourceMesh->isAlpha;
						destMesh.textures = sourceMesh->textures;
						destMesh.samplerState = sourceMesh->samplerState;

						destMesh.MeshID = sourceMesh->MeshID;
						destMesh.offsetMatrices = sourceMesh->offsetMatrices;

						destMesh.CopyShader(*sourceMesh);
						destMesh.SetMeshResource(key);

						meshList.push_back(&destMesh);

						initMesh(&destMesh);
					}
				}
			}
			else
			{
				//copy mesh
				for (int i = 0; i < currSourceObj->GetComponentCount(); i++)
				{
					if (SimpleMeshRender* sourceMesh = currSourceObj->GetComponentAtIndex<SimpleMeshRender>(i))
					{
						SimpleMeshRender& destMesh = AddMeshComponent(currDestObj, surface);
						destMesh.baseColor = sourceMesh->baseColor;
						SetBaseColor(destMesh.gameObject, destMesh.baseColor, surface);
						if (surface == SURFACE_TYPE::PBR && typeid(PBRMeshObject) == typeid(sourceMesh->gameObject))
						{
							static_cast<PBRMeshObject&>(destMesh.gameObject).Material = static_cast<PBRMeshObject&>(sourceMesh->gameObject).Material;
						}
						destMesh.isAlpha = sourceMesh->isAlpha;
						destMesh.textures = sourceMesh->textures;
						destMesh.samplerState = sourceMesh->samplerState;

						destMesh.MeshID = sourceMesh->MeshID;
						destMesh.SetMeshResource(key);
						destMesh.CopyShader(*sourceMesh);

						initMesh(&destMesh);
					}
				}
			}

			//Create Child Objects
			for (unsigned int i = 0; i < currSourceObj->transform.GetChildCount(); i++)
			{
				GameObject* sourceChild = &currSourceObj->transform.GetChild(i)->gameObject;
				objSourceQue.push(sourceChild);

				GameObject* destChild = NewMeshObject(surface, sourceChild->Name.c_str());
				destChild->transform.SetParent(currDestObj->transform);
				objDestQue.push(destChild);
			}

			destObjNameMap[currDestObj->Name] = currDestObj;
			isRoot = false;
		}

		//set boneList
		for (int i = 0; i < boneList.size(); i++)
		{
			if (boneList[i] == nullptr)
			{
				boneList.resize(i);
				break;
			}
		}
		for (auto& mesh : meshList)
		{
			mesh->boneList = boneList;
		}

		//set clip target
		for (auto& clip : clipList)
		{
			for (auto& animation : clip->nodeAnimations)
			{
				animation.objTarget = destObjNameMap[animation.objTarget->Name];
			}
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

void Utility::LoadFBX(const wchar_t* path,
	GameObject& _gameObject,
	std::function<void(MeshRender*)> initMesh,
	bool isStatic,
	SURFACE_TYPE surface)
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

	std::wstring wstr_path = path;		
	std::string str_path = wstring_to_utf8(path);

	if (GameObject* rootObject = IsResource(wstr_path))
	{
		CopyFBX(&_gameObject, rootObject, wstr_path.c_str(), initMesh, surface);
		return;
	}

	const aiScene* pScene = importer.ReadFile(str_path, importFlags);
	if (pScene == nullptr)
		return;

	std::wstring directory = utfConvert::utf8_to_wstring(str_path.substr(0, str_path.find_last_of("/\\")));

	std::queue<aiNode*> nodeQue;
	nodeQue.push(pScene->mRootNode);
	const aiNode* currNode = nullptr;

	std::queue<GameObject*> objQue;
	objQue.push(&_gameObject);
	GameObject* currObj = nullptr;

	std::unordered_map<std::wstring, GameObject*> addObjMap;
	std::unordered_map<std::wstring, int> boneIndexMap(128);
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

	std::vector<BoneComponent*> boneList(boneCount);
	std::vector<SimpleBoneMeshRender*> meshList;
	std::weak_ptr<GameObject> rootObj = _gameObject.GetWeakPtr();
	GetResourceManager<GameObject>().SetResource(wstr_path.c_str(), rootObj);

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
					for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
					{
						SimpleBoneMeshRender& meshComponent = AddBoneMeshComponent(currObj, surface);
						meshList.push_back(&meshComponent);

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), &meshComponent, surface);

						//offsetMatrices
						meshComponent.offsetMatrices = std::make_shared<OffsetMatrices>();
						meshComponent.offsetMatrices->data.resize(boneCount);

						for (unsigned int i = 0; i < pMesh->mNumBones; i++)
						{
							aiMatrix4x4 ai_matrix = pMesh->mBones[i]->mOffsetMatrix;
							std::wstring name = utf8_to_wstring(pMesh->mBones[i]->mName.C_Str());
							meshComponent.offsetMatrices->data[getBoneIndex(name)] = Matrix(&ai_matrix.a1).Transpose();
						}

						//cash vertex bone info
						std::unordered_map<int, std::vector<float>>		 weightsMap;
						std::unordered_map<int, std::list<std::wstring>> nameMap;
						if (pMesh->mNumVertices > 0)
						{
							weightsMap.reserve(pMesh->mNumVertices);
							nameMap.reserve(pMesh->mNumVertices);
						}
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

							aiVector3D Tangents = pMesh->mTangents[vertexIndex];
							vertex.Tangent.x = Tangents.x;
							vertex.Tangent.y = Tangents.y;
							vertex.Tangent.z = Tangents.z;

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
			
						meshComponent.MeshID = meshIndex;
						meshComponent.SetMeshResource(wstr_path.c_str());
						meshComponent.CreateMesh();

						initMesh(&meshComponent);
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
						SimpleMeshRender& meshComponent = AddMeshComponent(currObj, surface);

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), &meshComponent, surface);
			
						for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++)
						{
							SimpleMeshRender::Vertex vertex;
							aiVector3D position = pMesh->mVertices[vertexIndex];
							vertex.position.x = position.x;
							vertex.position.y = position.y;
							vertex.position.z = position.z;
							vertex.position.w = 1.0f;

							aiVector3D normal = pMesh->mNormals[vertexIndex];
							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;

							aiVector3D Tangents = pMesh->mTangents[vertexIndex];
							vertex.Tangent.x = Tangents.x;
							vertex.Tangent.y = Tangents.y;
							vertex.Tangent.z = Tangents.z;

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

						meshComponent.MeshID = meshIndex;
						meshComponent.SetMeshResource(wstr_path.c_str());
						meshComponent.CreateMesh();

						initMesh(&meshComponent);
					}
				}
			}
			
			for (unsigned int i = 0; i < currNode->mNumChildren; i++)
			{
				nodeQue.push(currNode->mChildren[i]);
				std::wstring childName = utf8_to_wstring(currNode->mChildren[i]->mName.C_Str());
				GameObject* childObj = NewMeshObject(surface, childName.c_str());
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

	//리소스 등록
	sceneManager.SetResouceObj(path, &_gameObject);
}

void Utility::LoadFBX(const wchar_t* path, GameObject& _gameObject, bool isStatic, SURFACE_TYPE surface)
{
	LoadFBX(path, _gameObject,[](MeshRender* mesh)->void { return; }, isStatic, surface);
}

void Utility::LoadFBXResource(const wchar_t* path, SURFACE_TYPE surface)
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

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함

	GameObject& _gameObject = *NewGameObject<GameObject>(path);

	std::wstring wstr_path = path;
	std::string str_path = wstring_to_utf8(path);
	if (GameObject* rootObject = IsResource(wstr_path))
	{
		Debug_printf("Resource already loaded.\n");
		return;
	}

	const aiScene* pScene = importer.ReadFile(str_path, importFlags);
	if (pScene == nullptr)
	{
		__debugbreak(); //임포트 실패
		return;
	}
		
	std::wstring directory = utfConvert::utf8_to_wstring(str_path.substr(0, str_path.find_last_of("/\\")));

	std::queue<aiNode*> nodeQue;
	nodeQue.push(pScene->mRootNode);
	const aiNode* currNode = nullptr;

	std::queue<GameObject*> objQue;
	objQue.push(&_gameObject);
	GameObject* currObj = nullptr;

	std::unordered_map<std::wstring, GameObject*> addObjMap;
	std::unordered_map<std::wstring, int> boneIndexMap(128);
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
	std::shared_ptr<BoneWIT> rootBoneWIT = nullptr;
	if (boneCount > 0)
	{
		rootMatrixPallete = std::make_shared<MatrixPallete>();
		rootBoneWIT = std::make_shared<BoneWIT>();
	}
	std::vector<BoneComponent*> boneList(boneCount);
	std::vector<SimpleBoneMeshRender*> meshList;
	std::weak_ptr<GameObject> rootObj = _gameObject.GetWeakPtr();
	GetResourceManager<GameObject>().SetResource(wstr_path.c_str(), rootObj);

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
					for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
					{
						SimpleBoneMeshRender& meshComponent = AddBoneMeshComponent(currObj, surface);
						meshList.push_back(&meshComponent);

						wchar_t materialName[50]{};
						swprintf_s(materialName, L"%s (%d)", currObj->Name.c_str(), currObj->GetInstanceID());

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), &meshComponent, SURFACE_TYPE::NONE);
;
						//offsetMatrices
						meshComponent.offsetMatrices = std::make_shared<OffsetMatrices>();
						meshComponent.offsetMatrices->data.resize(boneCount);

						for (unsigned int i = 0; i < pMesh->mNumBones; i++)
						{
							aiMatrix4x4 ai_matrix = pMesh->mBones[i]->mOffsetMatrix;
							std::wstring name = utf8_to_wstring(pMesh->mBones[i]->mName.C_Str());
							meshComponent.offsetMatrices->data[getBoneIndex(name)] = Matrix(&ai_matrix.a1).Transpose();
						}

						//cash vertex bone info
						std::unordered_map<int, std::vector<float>>		 weightsMap;
						std::unordered_map<int, std::list<std::wstring>> nameMap;
						if (pMesh->mNumVertices > 0)
						{
							weightsMap.reserve(pMesh->mNumVertices);
							nameMap.reserve(pMesh->mNumVertices);
						}
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
							vertex.Tangent.x = BiTangents.x;
							vertex.Tangent.y = BiTangents.y;
							vertex.Tangent.z = BiTangents.z;

							aiVector3D texCoord = pMesh->mTextureCoords[0][vertexIndex];
							vertex.Tex.x = texCoord.x;
							vertex.Tex.y = texCoord.y;

							auto n = nameMap[vertexIndex].begin();
							for (int j = 0; j < weightsMap[vertexIndex].size(); j++)
							{
								if (vertex.BlendWeights[j] != 0)
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

						meshComponent.MeshID = meshIndex;
						meshComponent.SetMeshResource(wstr_path.c_str());
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
						SimpleMeshRender& meshComponent = AddMeshComponent(currObj, surface);

						unsigned int meshIndex = currNode->mMeshes[i];
						aiMesh* pMesh = pScene->mMeshes[meshIndex];

						//Load Texture
						aiMaterial* ai_material = pScene->mMaterials[pMesh->mMaterialIndex];
						LoadTexture(ai_material, directory.c_str(), &meshComponent, SURFACE_TYPE::NONE);

						for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++)
						{
							SimpleMeshRender::Vertex vertex;
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
							vertex.Tangent.x = BiTangents.x;
							vertex.Tangent.y = BiTangents.y;
							vertex.Tangent.z = BiTangents.z;

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

						meshComponent.MeshID = meshIndex;
						meshComponent.SetMeshResource(wstr_path.c_str());
						meshComponent.CreateMesh();
					}
				}
			}

			for (unsigned int i = 0; i < currNode->mNumChildren; i++)
			{
				nodeQue.push(currNode->mChildren[i]);
				std::wstring childName = utf8_to_wstring(currNode->mChildren[i]->mName.C_Str());
				GameObject* childObj = NewMeshObject(surface, childName.c_str());
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

	//Move ResouceObj
	sceneManager.SetResouceObj(path, &_gameObject);
	sceneManager.DestroyObject(_gameObject);
}
