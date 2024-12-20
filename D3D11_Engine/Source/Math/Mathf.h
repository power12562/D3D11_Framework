#pragma once
#undef min
#undef max

#include <iostream>
#include <algorithm>
#include <type_traits>

namespace Mathf
{
	constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
	constexpr float FLOAT_MIN = std::numeric_limits<float>::min();
	constexpr float PI = 3.14159265f;
	constexpr float Deg2Rad = 0.01745329f;
	constexpr float Rad2Deg = 57.29578f;
	constexpr float Epsilon = std::numeric_limits<float>::epsilon();

	float Lerp(float startfloat, float endfloat, float t); //선형 보간
	
	/* 전달받은 배열중 가장 작은 값을 찾습니다.*/
	template<typename T, std::size_t N>
	T FindMinValue(const T(&arr)[N])
	{
		return *std::min_element(arr, arr + N);
	};

	/* 전달받은 배열중 가장 높은 값을 찾습니다.*/
	template<typename T, std::size_t N>
	T FindMaxValue(const T(&arr)[N])
	{
		return *std::max_element(arr, arr + N);
	};

	template<typename T>
	T FindMinValue(T value) 
	{
		return value;
	};
	/** 전달받은 인자들중 가장 낮은 값을 찾습니다.*/
	template<typename T, typename... Args>
	T FindMinValue(T first, Args... args)
	{
		return std::min(first, FindMinValue(args...));
	};

	template<typename T>
	T FindMaxValue(T value)
	{
		return value;
	};
	/** 전달받은 인자들중 가장 높은 값을 찾습니다.*/
	template<typename T, typename... Args>
	T FindMaxValue(T first, Args... args)
	{
		return std::max(first, FindMaxValue(args...));
	};
}