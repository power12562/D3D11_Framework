#include "TestComponent.h"
#include <Framework/TimeSystem.h>

TestComponent::TestComponent()
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::Start()
{
}

void TestComponent::FixedUpdate()
{
}

void TestComponent::Update()
{
	elapsedTime += TimeSystem::Time.DeltaTime;
	if (elapsedTime >= 2.0f)
	{
		elapsedTime = 0;
	}
}

void TestComponent::LateUpdate()
{
}
