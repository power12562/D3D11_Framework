#include "AssimpComponent.h"
#include <_Debug\Console.h>
#include <Framework/SceneManager.h>
#include <queue>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <Math/AssimpMath.h>

#include "../Source/SimpleMashComponent.h"

AssimpComponent::AssimpComponent()
{
}

AssimpComponent::~AssimpComponent()
{

}

void AssimpComponent::Start()
{
    LoadFBX("Resource/zeldaPosed001.fbx");
}

void AssimpComponent::FixedUpdate()
{
}

void AssimpComponent::Update()
{
}

void AssimpComponent::LateUpdate()
{
}

void AssimpComponent::Render()
{
}

void AssimpComponent::LoadFBX(const char* path)
{
    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate |    // vertex �ﰢ�� ���� ���
        aiProcess_GenNormals |        // Normal ���� ����  
        aiProcess_GenUVCoords |      // �ؽ�ó ��ǥ ����
        aiProcess_CalcTangentSpace |  // ź��Ʈ ���� ����
        aiProcess_ConvertToLeftHanded |  // DX�� �޼���ǥ�� ��ȯ
        aiProcess_PreTransformVertices;   // ����� ��ȯ����� ������ ���ؽ� �����Ѵ�.  *StaticMesh�� ó���Ҷ���

    std::string fileName(path);
    const aiScene* pScene = importer.ReadFile(fileName, importFlags);
    if (pScene == nullptr)
        return;

    this->directory = fileName.substr(0, fileName.find_last_of("/\\"));
   
    std::queue<aiNode*> nodeQue;
    nodeQue.push(pScene->mRootNode);
    const aiNode* currNode = nullptr;

    std::queue<GameObject*> objQue; 
    objQue.push(&gameObject);
    GameObject* currObj = nullptr;

    while (!nodeQue.empty())
    {
        currNode = nodeQue.front();
        nodeQue.pop();

        currObj = objQue.front();
        objQue.pop();

        int childNumber = 0;
        if (currNode)
        {    
            SimpleMashComponent& meshComponent = currObj->AddComponent<SimpleMashComponent>();
            
            Vector3 pos;
            Quaternion rot;
            Vector3 scale;
            AssimpMath::DecomposeTransform(currNode, pos, rot, scale);
            currObj->transform.position = pos;
            currObj->transform.rotation = rot;
            currObj->transform.scale = scale;

            for (unsigned int i = 0; i < currNode->mNumMeshes; i++)
            {
                unsigned int meshIndex = currNode->mMeshes[i];
                aiMesh* pMesh = pScene->mMeshes[meshIndex];
                Vertex vertex;
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
                    vertex.Tex.x = texCoord.x; // u ��ǥ
                    vertex.Tex.y = texCoord.y; // v ��ǥ

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
            }
            for (unsigned int i = 0; i < currNode->mNumChildren; i++)
            {       
                // ť�� �ڽ� ���� ������Ʈ�� ����
                nodeQue.push(currNode->mChildren[i]);

                std::wstring childName = currObj->Name + L"_Child_" + std::to_wstring(i);
                GameObject* childObj = NewGameObject<GameObject>(childName.c_str());
                childObj->transform.SetParent(currObj->transform, false);

                objQue.push(childObj);
            }  
            meshComponent.SetMesh();
        }
    }
}
