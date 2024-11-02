#include "TransformAnimation.h"
#include <Framework\TimeSystem.h>
#include <Utility\AssimpUtility.h>

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
		return true;
	}
	else
	{
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
	// 위치, 회전, 스케일에 대한 키를 찾기
	std::shared_ptr<PositionKey> currPositionKey = nullptr;
	std::shared_ptr<PositionKey> nextPositionKey = nullptr;
	for (int i = lastPosIndex; i < positionKeys.size(); i++)
	{
		if (positionKeys[i]->Time >= elapsedTime)
		{
			currPositionKey = positionKeys[i - 1];
			nextPositionKey = positionKeys[i];
			lastPosIndex = i; 
			break;
		}
	}

	std::shared_ptr<RotationKey> currRotationKey = nullptr;
	std::shared_ptr<RotationKey> nextRotationKey = nullptr;
	for (int i = lastRotIndex; i < rotationKeys.size(); i++)
	{
		if (rotationKeys[i]->Time >= elapsedTime)
		{
			currRotationKey = rotationKeys[i - 1];
			nextRotationKey = rotationKeys[i];
			lastRotIndex = i;
			break;
		}
	}

	std::shared_ptr<ScaleKey> currScaleKey = nullptr;
	std::shared_ptr<ScaleKey> nextScaleKey = nullptr;
	for (int i = lastScaleIndex; i < scaleKeys.size(); i++)
	{
		if (scaleKeys[i]->Time >= elapsedTime)
		{
			currScaleKey = scaleKeys[i - 1];
			nextScaleKey = scaleKeys[i];
			lastScaleIndex = i;
			break;
		}
	}

	float t = 0;
	// 위치 보간
	if (currPositionKey && nextPositionKey) 
	{
		t = (elapsedTime - currPositionKey->Time) / (nextPositionKey->Time - currPositionKey->Time);
		objTarget->transform.localPosition = Vector3::Lerp(currPositionKey->position, nextPositionKey->position, t);
	}
	else if (currPositionKey) 
	{
		objTarget->transform.localPosition = currPositionKey->position;
	}

	// 회전 보간
	if (currRotationKey && nextRotationKey) 
	{
		t = (elapsedTime - currRotationKey->Time) / (nextRotationKey->Time - currRotationKey->Time);
		objTarget->transform.localRotation = Quaternion::Slerp(currRotationKey->rotation, nextRotationKey->rotation, t);
	}
	else if (currRotationKey)
	{
		objTarget->transform.localRotation = currRotationKey->rotation;
	}

	// 스케일 보간
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