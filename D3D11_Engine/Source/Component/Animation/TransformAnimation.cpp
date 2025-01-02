#include "TransformAnimation.h"
#include <Core/TimeSystem.h>
#include <Utility\AssimpUtility.h>
#include <D3DCore/D3DRenderer.h>
#include <stack>

void TransformAnimation::CopyClips(TransformAnimation* dest, TransformAnimation* source)
{
	dest->clips = source->clips;
}

TransformAnimation::TransformAnimation()
{
}

TransformAnimation::~TransformAnimation()
{
}

bool TransformAnimation::PlayClip(const wchar_t* clipName, bool _isLoop)
{
	auto findIter = clips.find(clipName);
	if (findIter != clips.end())
	{
		currClip = &findIter->second;
		isPause = false;
		isLoop = _isLoop;
		
		//����� ã�´�
		for (auto& animation : currClip->nodeAnimations)
		{
			auto findTarget = targets.find(animation.targetName);
			if (findTarget == targets.end())
			{
				Debug_printf("warning: TransformAnimation::PlayClip target not found\n");
				currClip = nullptr;
				return false;
			}
			auto& [name, target] = *findTarget;
			animation.objTarget = target;
		}

		//ù�������� Matrix �ٷ� ����
		elapsedTime = currClip->TickTime * TimeSystem::Time.DeltaTime;
		for (auto& nodeAni : currClip->nodeAnimations)
		{
			nodeAni.Evaluate(elapsedTime);
		}
		transform.PushUpdateList();
		return true;
	}
	else
	{
		__debugbreak();
		Debug_printf("�������� �ʴ� Ŭ��.\n");
		return false;
	}
}

void TransformAnimation::StopClip()
{
	isPause = true;
	elapsedTime = 0;
	for (auto& nodeAni : currClip->nodeAnimations)
	{
		nodeAni.Evaluate(elapsedTime);
	}
}

void TransformAnimation::AddClip(const wchar_t* name, Clip& clip)
{
	clips[name] = clip;
}

void TransformAnimation::CopyClips(TransformAnimation* source)
{
	this->clips = source->clips;
}

void TransformAnimation::AddChildrenToTargets()
{
	targets.clear();
	std::stack<Transform*> transformStack;
	for (unsigned int i = 0; i < gameObject.transform.GetChildCount(); i++)
	{
		transformStack.push(gameObject.transform.GetChild(i));
	}
	while (!transformStack.empty())
	{
		Transform* currTransform = transformStack.top();
		transformStack.pop();
		targets[currTransform->gameObject.Name] = &currTransform->gameObject;
		for (int i = 0; i < currTransform->GetChildCount(); i++)
		{
			transformStack.push(currTransform->GetChild(i));
		}
	}
}

void TransformAnimation::Start()
{
}

void TransformAnimation::FixedUpdate()
{
}

void TransformAnimation::Update()
{

}

void TransformAnimation::LateUpdate()
{
	if (currClip && !isPause)
	{
		elapsedTime += currClip->TickTime * TimeSystem::Time.DeltaTime;

		if(d3dRenderer.CheckFrustumCulling(&gameObject))
			for (auto& nodeAni : currClip->nodeAnimations)
			{
				nodeAni.Evaluate(elapsedTime);
			}

		while (elapsedTime >= currClip->Duration)
		{
			if (isLoop)
			{
				elapsedTime -= currClip->Duration;
				currClip->ResetCashIndex();
			}
			else
			{ 
				elapsedTime = 0;
				isPause = true;
				break;
			}
		}
	}
}

void TransformAnimation::Clip::NodeAnimation::Evaluate(float elapsedTime)
{
	// ��ġ, ȸ��, �����Ͽ� ���� Ű�� ã��
	PositionKey* currPositionKey = nullptr;
	PositionKey* nextPositionKey = nullptr;
	for (int i = lastPosIndex; i < positionKeys->size(); i++)
	{
		if (positionKeys->data()[i].Time >= elapsedTime)
		{
			currPositionKey = &positionKeys->data()[i - 1];
			nextPositionKey = &positionKeys->data()[i];
			lastPosIndex = i; 
			break;
		}
	}

	RotationKey* currRotationKey = nullptr;
	RotationKey* nextRotationKey = nullptr;
	for (int i = lastRotIndex; i < rotationKeys->size(); i++)
	{
		if (rotationKeys->data()[i].Time >= elapsedTime)
		{
			currRotationKey = &rotationKeys->data()[i - 1];
			nextRotationKey = &rotationKeys->data()[i];
			lastRotIndex = i;
			break;
		}
	}

	ScaleKey* currScaleKey = nullptr;
	ScaleKey* nextScaleKey = nullptr;
	for (int i = lastScaleIndex; i < scaleKeys->size(); i++)
	{
		if (scaleKeys->data()[i].Time >= elapsedTime)
		{
			currScaleKey = &scaleKeys->data()[i - 1];
			nextScaleKey = &scaleKeys->data()[i];
			lastScaleIndex = i;
			break;
		}
	}

	float t = 0;
	// ��ġ ����
	if (currPositionKey && nextPositionKey) 
	{
		t = (elapsedTime - currPositionKey->Time) / (nextPositionKey->Time - currPositionKey->Time);
		objTarget->transform.localPosition = Vector3::Lerp(currPositionKey->position, nextPositionKey->position, t);
	}
	else if (currPositionKey) 
	{
		objTarget->transform.localPosition = currPositionKey->position;
	}

	// ȸ�� ����
	if (currRotationKey && nextRotationKey) 
	{
		t = (elapsedTime - currRotationKey->Time) / (nextRotationKey->Time - currRotationKey->Time);
		objTarget->transform.localRotation = Quaternion::Slerp(currRotationKey->rotation, nextRotationKey->rotation, t);
	}
	else if (currRotationKey)
	{
		objTarget->transform.localRotation = currRotationKey->rotation;
	}

	// ������ ����
	if (currScaleKey && nextScaleKey) 
	{
		t = (elapsedTime - currScaleKey->Time) / (nextScaleKey->Time - currScaleKey->Time);
		objTarget->transform.localScale = Vector3::Lerp(currScaleKey->scale, nextScaleKey->scale, t);
	}
	else if (currScaleKey)
	{
		objTarget->transform.localScale = currScaleKey->scale;
	}
}