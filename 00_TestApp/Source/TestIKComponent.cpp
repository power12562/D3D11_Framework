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
	if(endEffector)
	{ 		
        CCDMode();
	}
}

void TestIKComponent::LateUpdate()
{
}

void TestIKComponent::SetIK(GameObject* target, int maxDepth)
{
	endEffector = &target->transform;
	this->maxDepth = maxDepth;

    //체인 추적 (endEffector에서 maxDepth만큼 부모로)
    chain.clear();
    Transform* current = endEffector;
    for (int i = 0; i <= maxDepth; ++i)
    {
        chain.push_back(current);
        if (current->Parent != nullptr)
        {
            current = current->Parent;
        }
        else
        {
            break;
        }
    }
}

void TestIKComponent::CCDMode()
{

}
