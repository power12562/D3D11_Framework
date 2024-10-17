#include "AssimpMath.h"

using namespace SimpleMath;

void AssimpMath::DecomposeTransform(const aiNode* node, Vector3& position, Quaternion& rotation, Vector3& scale)
{
    // Assimp의 aiMatrix4x4를 DirectX의 XMMATRIX로 변환
    aiMatrix4x4 transform = node->mTransformation;

    aiVector3D ai_position;
    aiQuaternion ai_rotation;
    aiVector3D ai_scale;
    transform.Decompose(ai_scale, ai_rotation, ai_position);

    position.x = ai_position.x;
    position.y = ai_position.y;
    position.z = ai_position.z;

    scale.x = ai_scale.x;
    scale.y = ai_scale.y;
    scale.z = ai_scale.z;

    rotation.x = ai_rotation.x;
    rotation.y = ai_rotation.y;
    rotation.z = ai_rotation.z;
    rotation.w = ai_rotation.w;
}