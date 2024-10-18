#include "AssimpUtility.h"
#include <Framework\SceneManager.h>
#include <GameObject\Base\GameObject.h>
#include <Component\Render\SimpleMeshRender.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <Math/AssimpMath.h>
#include <Utility\utfConvert.h>
#include <Material\SimpleMaterial.h>
#include <Component\TransformAnimation.h>
#include <Framework\MaterialManager.h>
#include <filesystem>
#include <iostream>

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

void Utility::LoadFBX(const char* path, GameObject& _gameObject, const wchar_t* material_name)
{
    Assimp::Importer importer;
    unsigned int importFlags =
        aiProcess_Triangulate |         // vertex �ﰢ�� ���� ���
        aiProcess_GenNormals |          // Normal ���� ����  
        aiProcess_GenUVCoords |         // �ؽ�ó ��ǥ ����
        aiProcess_CalcTangentSpace |    // ź��Ʈ ���� ����
        aiProcess_ConvertToLeftHanded;  // DX�� �޼���ǥ�� ��ȯ
    //|  aiProcess_PreTransformVertices;   // ����� ��ȯ����� ������ ���ؽ� �����Ѵ�.  *����ϸ� ��� �޽��� �������� �ִϸ��̼� ����� �Ұ����ϴ�.

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

    std::unordered_map<std::wstring, GameObject*> addMap;

    while (!nodeQue.empty())
    {
        currNode = nodeQue.front();
        nodeQue.pop();

        currObj = objQue.front();
        objQue.pop();

        addMap[currObj->Name] = currObj;

        int childNumber = 0;
        if (currNode)
        {
            SimpleMeshRender& meshComponent = currObj->AddComponent<SimpleMeshRender>();
            meshComponent.Material = materialManager.GetMaterial(material_name);

            Vector3 pos;
            Quaternion rot;
            Vector3 scale;
            AssimpMath::DecomposeTransform(currNode, pos, rot, scale);
            if (currObj->transform.Parent)
            {
                currObj->transform.localPosition = pos;
                currObj->transform.localRotation = rot;
                currObj->transform.localScale = scale;
            }

            for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
            {
                unsigned int meshIndex = currNode->mMeshes[i];
                aiMesh* pMesh = pScene->mMeshes[meshIndex];
                SimpleMeshRender::Vertex vertex;
                for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++)
                {
                    aiVector3D position = pMesh->mVertices[vertexIndex];
                    vertex.position.x = position.x;
                    vertex.position.y = position.y;
                    vertex.position.z = position.z;

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

                for (unsigned int faceIndex = 0; faceIndex < pMesh->mNumFaces; faceIndex++)
                {
                    const aiFace& face = pMesh->mFaces[faceIndex];
                    for (unsigned int numIndex = 0; numIndex < face.mNumIndices; numIndex++)
                    {
                        meshComponent.indices.push_back(face.mIndices[numIndex]);
                    }
                }

                using namespace utfConvert;
                //Load Texture
                aiMaterial* materials = pScene->mMaterials[pMesh->mMaterialIndex];
                aiString path;
                std::wstring basePath;
                if (AI_SUCCESS == materials->GetTexture(aiTextureType_DIFFUSE, 0, &path))
                {
                    if (Utility::ParseFileName(path))
                    {
                        basePath = directory;
                        basePath += L"\\";
                        basePath += utfConvert::utf8_to_wstring(path.C_Str());
                        meshComponent.Material->SetDiffuse(basePath.c_str());
                    }
                }
                if (AI_SUCCESS == materials->GetTexture(aiTextureType_NORMALS, 0, &path))
                {
                    if (Utility::ParseFileName(path))
                    {
                        basePath = directory;
                        basePath += L"\\";
                        basePath += utfConvert::utf8_to_wstring(path.C_Str());
                        meshComponent.Material->SetNormalMap(basePath.c_str());
                    }
                }
                if (AI_SUCCESS == materials->GetTexture(aiTextureType_SPECULAR, 0, &path))
                {

                    if (Utility::ParseFileName(path))
                    {
                        basePath = directory;
                        basePath += L"\\";
                        basePath += utfConvert::utf8_to_wstring(path.C_Str());
                        meshComponent.Material->SetSpecularMap(basePath.c_str());
                    }
                }
                if (AI_SUCCESS == materials->GetTexture(aiTextureType_EMISSIVE, 0, &path))
                {
                    if (Utility::ParseFileName(path))
                    {
                        basePath = directory;
                        basePath += L"\\";
                        basePath += utfConvert::utf8_to_wstring(path.C_Str());
                        meshComponent.Material->SetEmissiveMap(basePath.c_str());
                    }
                }
                if (AI_SUCCESS == materials->GetTexture(aiTextureType_OPACITY, 0, &path))
                {
                    if (Utility::ParseFileName(path))
                    {
                        basePath = directory;
                        basePath += L"\\";
                        basePath += utfConvert::utf8_to_wstring(path.C_Str());
                        meshComponent.Material->SetOpacityMap(basePath.c_str());
                    }
                }
            }
            for (unsigned int i = 0; i < currNode->mNumChildren; i++)
            {
                nodeQue.push(currNode->mChildren[i]);
                std::wstring childName = utfConvert::utf8_to_wstring(currNode->mChildren[i]->mName.C_Str());
                GameObject* childObj = NewGameObject<GameObject>(childName.c_str());
                childObj->transform.SetParent(currObj->transform, false);

                objQue.push(childObj);
            }
            meshComponent.CreateMesh();
        }
    }

    if (pScene->mAnimations)
    {
        using Clip = TransformAnimation::Clip;

        TransformAnimation& anime = _gameObject.AddComponent<TransformAnimation>();
        for (unsigned int i = 0; i < pScene->mNumAnimations; i++)
        {
            aiAnimation* currAnimation = pScene->mAnimations[i];
            Clip clip;
            clip.Duration = currAnimation->mDuration;
            clip.TickTime = currAnimation->mTicksPerSecond;
            for (unsigned int j = 0; j < currAnimation->mNumChannels; j++)
            {
                aiNodeAnim* currNodeAnim = currAnimation->mChannels[j];
                using NodeAnime = Clip::NodeAnimation;
                NodeAnime nodeAnime;
                nodeAnime.objTarget = addMap[utfConvert::utf8_to_wstring(currNodeAnim->mNodeName.C_Str()).c_str()];
                for (unsigned int k = 0; k < currNodeAnim->mNumPositionKeys; k++)
                {
                    NodeAnime::PositionKey key;
                    key.position = Vector3(&currNodeAnim->mPositionKeys[k].mValue.x);
                    key.Time = currNodeAnim->mPositionKeys[k].mTime;
                    nodeAnime.positionKeys.push_back(key);
                }
                for (unsigned int k = 0; k < currNodeAnim->mNumRotationKeys; k++)
                {
                    NodeAnime::RotationKey key;
                    key.rotation.x = currNodeAnim->mRotationKeys[k].mValue.x;
                    key.rotation.y = currNodeAnim->mRotationKeys[k].mValue.y;
                    key.rotation.z = currNodeAnim->mRotationKeys[k].mValue.z;
                    key.rotation.w = currNodeAnim->mRotationKeys[k].mValue.w;

                    key.Time = currNodeAnim->mRotationKeys[k].mTime;
                    nodeAnime.rotationKeys.push_back(key);
                }
                for (unsigned int k = 0; k < currNodeAnim->mNumScalingKeys; k++)
                {
                    NodeAnime::ScaleKey key;
                    key.scale = Vector3(&currNodeAnim->mScalingKeys[k].mValue.x);
                    key.Time = currNodeAnim->mScalingKeys[k].mTime;
                    nodeAnime.scaleKeys.push_back(key);
                }
                clip.nodeAnimations.push_back(nodeAnime);
            }
            anime.AddClip(utfConvert::utf8_to_wstring(pScene->mAnimations[i]->mName.C_Str()).c_str(), clip);
        }
    }
}
