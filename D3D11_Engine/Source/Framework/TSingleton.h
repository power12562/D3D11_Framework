#pragma once
template <typename T>

class TSingleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};