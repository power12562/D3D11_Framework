#pragma once
template <typename T>

class TSingleton
{
protected:
	TSingleton() = default;
	~TSingleton() = default;
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};