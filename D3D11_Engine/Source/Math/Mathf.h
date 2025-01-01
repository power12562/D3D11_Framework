#pragma once
#include <directxtk/SimpleMath.h>
#include <iostream>
#include <algorithm>
#include <type_traits>
#undef min
#undef max

namespace Mathf
{
	constexpr float FLOAT_MAX = FLT_MAX;
	constexpr float FLOAT_MIN = FLT_MIN;
	constexpr float PI = 3.14159265f;
	constexpr float Deg2Rad = 0.01745329f;
	constexpr float Rad2Deg = 57.29578f;
	constexpr float Epsilon = std::numeric_limits<float>::epsilon();
	/*radian angle ���� Epsilon*/
	constexpr float AngleEpsilon = 0.001f;

	//���� ����
	float Lerp(float startfloat, float endfloat, float t);

	//�ΰ��� ���ʹϾ� ���� ���̸� ��ȯ���ݴϴ�. ���� : radian
	float GetAngleDifference(const DirectX::SimpleMath::Quaternion& q1, const DirectX::SimpleMath::Quaternion& q2);
	
	/* ���޹��� �迭�� ���� ���� ���� ã���ϴ�.*/
	template<typename T, std::size_t N>
	T FindMinValue(const T(&arr)[N])
	{
		return *std::min_element(arr, arr + N);
	};

	/* ���޹��� �迭�� ���� ���� ���� ã���ϴ�.*/
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
	/** ���޹��� ���ڵ��� ���� ���� ���� ã���ϴ�.*/
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
	/** ���޹��� ���ڵ��� ���� ���� ���� ã���ϴ�.*/
	template<typename T, typename... Args>
	T FindMaxValue(T first, Args... args)
	{
		return std::max(first, FindMaxValue(args...));
	};
}