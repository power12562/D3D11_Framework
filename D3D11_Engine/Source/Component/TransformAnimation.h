#pragma once
#include <Component\Base\Component.h>
#include <string>
#include <unordered_map>

struct aiAnimation;

class TransformAnimation : public Component
{
public:
	struct Clip
	{
		struct NodeAnimation
		{
			friend struct Clip;
			struct PositionKey
			{
				float Time;
				Vector3 position;
			};
			struct RotationKey
			{
				float Time;
				Quaternion rotation;
			};
			struct ScaleKey
			{
				float Time;
				Vector3 scale;
			};
			std::vector<PositionKey> positionKeys;
			std::vector<RotationKey> rotationKeys;
			std::vector<ScaleKey> scaleKeys;
			GameObject* objTarget = nullptr;
			void Evaluate(float elapsedTime);
		private:
			int lastPosIndex = 0;
			int lastRotIndex = 0;
			int lastScaleIndex = 0;
		};
		std::vector<NodeAnimation> nodeAnimations;
		float Duration;
		float TickTime;
		void ResetCashIndex() 
		{
			for (auto& i : nodeAnimations)
			{
				i.lastPosIndex = 0;
				i.lastRotIndex = 0;
				i.lastScaleIndex = 0;
			}
		}
	};
public:
	TransformAnimation();
	virtual ~TransformAnimation() override;
public:
	bool PlayClip(const wchar_t* clipName, bool isLoop = true);
	void StopClip();
	void AddClip(const wchar_t* name, Clip& clip);

public:
	virtual void Start() 		 override;
protected:						 
	virtual void FixedUpdate() 	 override;
	virtual void Update() 		 override;
	virtual void LateUpdate()	 override;

private:
	std::unordered_map<std::wstring, Clip> clips;
	
	Clip* currClip = nullptr;
	bool isPause = false;
	float elapsedTime = 0.f;
	bool isLoop = false;
};
