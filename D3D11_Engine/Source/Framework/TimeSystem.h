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
	float GetDeltatime_ms(bool isScale) const;
	float GetDeltatime_ms() const { return GetDeltatime_ms(true); }

	float GetDeltatime(bool isScale) const { return (GetDeltatime_ms(isScale) / 1000.f);}
	float GetDeltatime() const { return (GetDeltatime_ms(true) / 1000.f);}

	__declspec(property(get = GetDeltatime)) float DeltaTime;
	__declspec(property(get = GetDeltatime_ms)) float DeltaTime_ms;

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
