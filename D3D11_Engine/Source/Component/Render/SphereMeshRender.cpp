#include "SphereMeshRender.h"
#include <Math/Mathf.h>

SphereMeshRender::SphereMeshRender()
{
   
}

void SphereMeshRender::CreateSphere(float radius, UINT stackCount, UINT sliceCount) 
{
    MeshID = 0;
    SetMeshResource(L"Sphere");

    for (UINT stack = 0; stack <= stackCount; ++stack) 
    {
        float phi = stack * Mathf::PI / stackCount; // 세로 방향 (0 ~ π)

        for (UINT slice = 0; slice <= sliceCount; ++slice) 
        {
            float theta = slice * 2.0f * Mathf::PI / sliceCount; // 가로 방향 (0 ~ 2π)

            // 구체 표면의 정점 좌표
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = -radius * std::sin(phi) * std::sin(theta);

            // 법선 벡터 (단위 벡터)
            Vector3 normal = { x, y, z };
            normal.Normalize();

            // 텍스처 좌표
            float u = static_cast<float>(slice) / sliceCount;
            float v = static_cast<float>(stack) / stackCount;

            // BiTangent 계산
            Vector3 tangent = {-std::sin(theta), 0, std::cos(theta)};
            tangent.Normalize();
            Vector3 biTangent = normal.Cross(tangent);

            vertices.emplace_back(
                Vector4{x, y, z, 1.0f}, // Position
                normal,                 // Normal
                biTangent,              // BiTangent
                Vector2{u, v}           // TexCoord
                );
        }
    }

    // 인덱스 생성 (삼각형)
    for (UINT stack = 0; stack < stackCount; ++stack) 
    {
        for (UINT slice = 0; slice < sliceCount; ++slice) 
        {
            UINT first = stack * (sliceCount + 1) + slice;
            UINT second = first + sliceCount + 1;

            // 삼각형 두 개 생성
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    CreateMesh();
}