#pragma once
#include <Utility/AssimpUtility.h>
#include <Component\Base\Component.h>
#include <string>
#include <unordered_map>

struct aiAnimation;

class TransformAnimation : public Component
{
public:
	struct Clip
	{
		Clip() = default;
		~Clip() = default;
		Clip(const Clip& rhs)
		{
			if (this == &rhs)
				return;

			this->Duration = rhs.Duration;
			this->nodeAnimations = rhs.nodeAnimations;
			this->TickTime = rhs.TickTime;
			this->ResetCashIndex();
		}
		Clip& operator=(const Clip& rhs)
		{
			if (this == &rhs)
				return *this;

			this->Duration = rhs.Duration;
			this->nodeAnimations = rhs.nodeAnimations;
			this->TickTime = rhs.TickTime;
			this->ResetCashIndex();
			return *this;
		}
		struct NodeAnimation
		{
			friend struct Clip;
			struct PositionKey
			{
				float Time{};
				Vector3 position{};
			};
			struct RotationKey
			{
				float Time{};
				Quaternion rotation{};
			};
			struct ScaleKey
			{
				float Time{};
				Vector3 scale{};
			};
			std::shared_ptr<std::vector<PositionKey>> positionKeys;
			std::shared_ptr<std::vector<RotationKey>> rotationKeys;
			std::shared_ptr<std::vector<ScaleKey>> scaleKeys;
			void Evaluate(float elapsedTime);
		private:
			int lastPosIndex = 0;
			int lastRotIndex = 0;
			int lastScaleIndex = 0;
		public:
			GameObject* objTarget = nullptr;
		};
		std::vector<NodeAnimation> nodeAnimations;
		float Duration{};
		float TickTime{};
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
	static void CopyClips(TransformAnimation* dest, TransformAnimation* source);

public:
	TransformAnimation();
	virtual ~TransformAnimation() override;

public:
	bool PlayClip(const wchar_t* clipName, bool isLoop = true);
	void StopClip();
	void AddClip(const wchar_t* name, Clip& clip);
	const Clip* GetCurrClip() { return currClip; }
	const std::unordered_map<std::wstring, Clip>& GetClips() { return clips; }
	void CopyClips(TransformAnimation* source);
	size_t GetClipsCount() { return clips.size(); }

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
