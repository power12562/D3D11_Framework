#pragma once
#include <GameObject/Base/GameObject.h>

class BoneObject : public GameObject
{
	SERIALIZED_OBJECT(BoneObject)
public:
	BoneObject() = default;
	/*�̸�, �ν��Ͻ� ���̵� �ο� �� ȣ��Ǵ� �Լ�.*/
	virtual ~BoneObject() = default;

	virtual void Serialized(std::ofstream& ofs);
	virtual void Deserialized(std::ifstream& ifs);

public:
	/*Get world matrix�� ����*/
	const Matrix& GetBoneMatrix();
	int myIndex = -1;
};