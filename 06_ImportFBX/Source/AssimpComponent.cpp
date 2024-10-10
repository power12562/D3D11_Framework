#include "AssimpComponent.h"
#include <_Debug\Console.h>
#include <Framework\D3DRenderer.h>
#include <Framework\HLSLManager.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

AssimpComponent::AssimpComponent()
{
}

AssimpComponent::~AssimpComponent()
{
    using namespace Utility;
    SafeRelease(m_pTextureRV);
    SafeRelease(m_pNormalMap);
    SafeRelease(m_pSpecularMap);
    SafeRelease(m_pSamplerLinear);
}

void AssimpComponent::Start()
{
    hlslManager.CreateSharingShader(L"PixelShader.hlsl", "ps_4_0", &drawData.pPixelShader);
    hlslManager.CreateSharingShader(L"VertexShader.hlsl", "vs_4_0", &drawData.pVertexShader);

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
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    auto pDevice = d3dRenderer.GetDevice();
    ID3D10Blob* vertexShaderBuffer = nullptr;
    Utility::CheackHRESULT(Utility::CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    Utility::CheackHRESULT(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &drawData.pInputLayout));
    vertexShaderBuffer->Release();

    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate |    // vertex 삼각형 으로 출력
        aiProcess_GenNormals |        // Normal 정보 생성  
        aiProcess_GenUVCoords |      // 텍스처 좌표 생성
        aiProcess_CalcTangentSpace |  // 탄젠트 벡터 생성
        aiProcess_ConvertToLeftHanded |  // DX용 왼손좌표계 변환
        aiProcess_PreTransformVertices;   // 노드의 변환행렬을 적용한 버텍스 생성한다.  *StaticMesh로 처리할때만

    std::string fileName(path);
    const aiScene* pScene = importer.ReadFile(fileName, importFlags);
    if (pScene == nullptr)
        return;

    this->directory = fileName.substr(0, fileName.find_last_of("/\\"));
   
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector2 Tex;
    };
    drawData.vertexBufferStride = sizeof(Vertex);

    std::vector<Vertex> vertices;
    std::vector<aiMesh*> meshes;
    std::vector<UINT> indices;
    auto processMesh = [&](aiMesh* mesh)->aiMesh*
        {

        };

    auto processNode = [&](aiNode* node)->void
        {
            for (UINT i = 0; i < node->mNumMeshes; i++) 
            {
                aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh));
            }

            for (UINT i = 0; i < node->mNumChildren; i++) 
            {
                this->processNode(node->mChildren[i]);
            }
        };

    

  
}
