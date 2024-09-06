#pragma once
#include <windows.h>
#include <cmath>

class QPCTime
{

public:
	QPCTime();
	~QPCTime() = default; 

	void UpdateTime();
	const float GetFrameRate()
	{
		if (deltaTime_ms == 0) return 0;

		return ceil(((1000.0f / deltaTime_ms) * 1000.f) / 1000.f);
	}

	float GetDeltaTime_ms(bool isScale = true) const;
	float GetDeltaTime(bool isScale = true) const { return (GetDeltaTime_ms(isScale) / 1000.f);}

	float GetPropertyDeltaTime() { return GetDeltaTime(); }
	__declspec(property(get = GetPropertyDeltaTime)) float DeltaTime;

	float GetPropertyDeltaTime_ms() { return GetDeltaTime_ms(); }
	__declspec(property(get = GetPropertyDeltaTime_ms)) float DeltaTime_ms;

	int FixedUpdatePerSec = 50;
	float GetFixedDelta() const { return 1.f / (float)FixedUpdatePerSec; }

	float timeScale;

private:
	LARGE_INTEGER previousTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER frequency;

	float deltaTime_ms = 0;
};

namespace TimeSystem
{
	extern QPCTime Time;
}
