#include "Math/Mathf.h"

float Mathf::Lerp(float startfloat, float endfloat, float t)
{
	float Lerp = (1.f - t) * startfloat + t * endfloat;
	return Lerp;
}