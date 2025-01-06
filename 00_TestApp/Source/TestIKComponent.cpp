#include "TestIKComponent.h"
#include <framework.h>
#include <InverseKinematics/InverseKinematics.h>

TestIKComponent::TestIKComponent()
{
	
}

TestIKComponent::~TestIKComponent()
{

}

void TestIKComponent::Start()
{

}

void TestIKComponent::FixedUpdate()
{
}

void TestIKComponent::Update()
{
	if(!joints.empty())
	{ 		
        CCDMode();
	}
}

void TestIKComponent::LateUpdate()
{
}

void TestIKComponent::SetIK(GameObject* target, int maxDepth)
{
    //체인 추적 (endEffector에서 maxDepth만큼 부모로)
    this->maxDepth = maxDepth;
    joints.clear();
    Transform* current = &target->transform;
    for (int i = 0; i <= maxDepth; ++i)
    {
        joints.push_back(current);
        if (current->Parent == nullptr)
        {
            break;
        }
        current = current->Parent;
    }
    CalculateMaxReach();
}

void TestIKComponent::CCDMode()
{
    Vector3 tarPos = transform.GetPosition();
    Transform* cur = joints.front()->Parent;
    int depth = 1;
    while (cur != joints.back()->Parent)
    {
        // get 3 points
        Vector3 endPos = joints.front()->GetPosition();
        Vector3 curPos = cur->GetPosition();

        // compute cos theta and rotation axis r
        Vector3 e_c = (endPos - curPos);
        e_c.Normalize();
        Vector3 t_c = (tarPos - curPos);
        t_c.Normalize();

        float cos_theta = e_c.Dot(t_c);
        Vector3 r = e_c.Cross(t_c); //axis r

        // compute quaternion
        Quaternion q = Quaternion(r.x * 0.5f, r.y * 0.5f, r.z * 0.5f, (cos_theta + 1.0f) * 0.5f);
        cur->localRotation = cur->localRotation * q;
        cur = cur->Parent;
    }
}

void TestIKComponent::CalculateMaxReach()
{
    size_t numJoints = joints.size();
    if (numJoints > 0)
    {
        std::vector<Quaternion> tempRotation(numJoints);
        for (size_t i = 0; i < numJoints; i++)
        {
            tempRotation[i] = joints[i]->localRotation;
            joints[i]->localRotation = Quaternion();
        }
        for (size_t i = 1; i < numJoints; i++)
        {
            Vector3 diff = joints[i]->position - joints[i - 1]->position;
            maxReach += diff.Length();
        }
        for (size_t i = 0; i < numJoints; i++)
        {
            joints[i]->localRotation = tempRotation[i];
        }
    }
}
