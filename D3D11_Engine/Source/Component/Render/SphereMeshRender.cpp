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
        float phi = stack * Mathf::PI / stackCount; // ���� ���� (0 ~ ��)

        for (UINT slice = 0; slice <= sliceCount; ++slice) 
        {
            float theta = slice * 2.0f * Mathf::PI / sliceCount; // ���� ���� (0 ~ 2��)

            // ��ü ǥ���� ���� ��ǥ
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = -radius * std::sin(phi) * std::sin(theta);

            // ���� ���� (���� ����)
            Vector3 normal = { x, y, z };
            normal.Normalize();

            // �ؽ�ó ��ǥ
            float u = static_cast<float>(slice) / sliceCount;
            float v = static_cast<float>(stack) / stackCount;

            // Tangent ���
            Vector3 tangent = {-std::sin(theta), 0, std::cos(theta)};

            vertices.emplace_back(
                Vector4{x, y, z, 1.0f}, // Position
                normal,                 // Normal
                tangent,              // Tangent
                Vector2{u, v}           // TexCoord
                );
        }
    }

    // �ε��� ���� (�ﰢ��)
    for (UINT stack = 0; stack < stackCount; ++stack) 
    {
        for (UINT slice = 0; slice < sliceCount; ++slice) 
        {
            UINT first = stack * (sliceCount + 1) + slice;
            UINT second = first + sliceCount + 1;

            // �ﰢ�� �� �� ����
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