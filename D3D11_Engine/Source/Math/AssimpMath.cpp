#include "AssimpMath.h"

using namespace SimpleMath;

void AssimpMath::DecomposeTransform(const aiNode* node, Vector3& position, Quaternion& rotation, Vector3& scale)
{
    // Assimp�� aiMatrix4x4�� DirectX�� XMMATRIX�� ��ȯ
    aiMatrix4x4 transform = node->mTransformation;

    // ��ġ ����
    position.x = transform.a4; // 1�� 4��
    position.y = transform.b4; // 2�� 4��
    position.z = transform.c4; // 3�� 4��

    // ������ ����
    scale.x = sqrt(transform.a1 * transform.a1 + transform.a2 * transform.a2 + transform.a3 * transform.a3);
    scale.y = sqrt(transform.b1 * transform.b1 + transform.b2 * transform.b2 + transform.b3 * transform.b3);
    scale.z = sqrt(transform.c1 * transform.c1 + transform.c2 * transform.c2 + transform.c3 * transform.c3);

    // �������� ������ ����ȭ
    // �������� ������ ȸ�� ��� ���
    aiMatrix4x4 rotationMatrix;
    rotationMatrix.a1 = transform.a1 / scale.x;
    rotationMatrix.a2 = transform.a2 / scale.x;
    rotationMatrix.a3 = transform.a3 / scale.x;
    rotationMatrix.a4 = 0.0f;

    rotationMatrix.b1 = transform.b1 / scale.y;
    rotationMatrix.b2 = transform.b2 / scale.y;
    rotationMatrix.b3 = transform.b3 / scale.y;
    rotationMatrix.b4 = 0.0f;

    rotationMatrix.c1 = transform.c1 / scale.z;
    rotationMatrix.c2 = transform.c2 / scale.z;
    rotationMatrix.c3 = transform.c3 / scale.z;
    rotationMatrix.c4 = 0.0f;

    rotationMatrix.d1 = 0.0f;
    rotationMatrix.d2 = 0.0f;
    rotationMatrix.d3 = 0.0f;
    rotationMatrix.d4 = 1.0f;

    // ȸ���� ���ʹϾ����� ��ȯ
    float trace = rotationMatrix.a1 + rotationMatrix.b2 + rotationMatrix.c3;
    if (trace > 0)
    {
        float s = 0.5f / sqrt(trace + 1.0f);
        rotation.x = (rotationMatrix.b3 - rotationMatrix.c2) * s;
        rotation.y = (rotationMatrix.c1 - rotationMatrix.a3) * s;
        rotation.z = (rotationMatrix.a2 - rotationMatrix.b1) * s;
        rotation.w = 0.25f / s;
    }
    else
    {
        if (rotationMatrix.a1 > rotationMatrix.b2 && rotationMatrix.a1 > rotationMatrix.c3)
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.a1 - rotationMatrix.b2 - rotationMatrix.c3);
            rotation.x = 0.25f * s;
            rotation.y = (rotationMatrix.b1 + rotationMatrix.a2) / s;
            rotation.z = (rotationMatrix.c1 + rotationMatrix.a3) / s;
            rotation.w = (rotationMatrix.b3 - rotationMatrix.c2) / s;
        }
        else if (rotationMatrix.b2 > rotationMatrix.c3)
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.b2 - rotationMatrix.a1 - rotationMatrix.c3);
            rotation.x = (rotationMatrix.b1 + rotationMatrix.a2) / s;
            rotation.y = 0.25f * s;
            rotation.z = (rotationMatrix.c2 + rotationMatrix.b3) / s;
            rotation.w = (rotationMatrix.c1 - rotationMatrix.a3) / s;
        }
        else
        {
            float s = 2.0f * sqrt(1.0f + rotationMatrix.c3 - rotationMatrix.a1 - rotationMatrix.b2);
            rotation.x = (rotationMatrix.c1 + rotationMatrix.a3) / s;
            rotation.y = (rotationMatrix.c2 + rotationMatrix.b3) / s;
            rotation.z = 0.25f * s;
            rotation.w = (rotationMatrix.b1 - rotationMatrix.a2) / s;
        }
    }
}