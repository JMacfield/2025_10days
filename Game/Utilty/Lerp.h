#pragma once
#include <vector>

namespace Easings {
	float EaseInSine(float t);
	float EaseOutSine(float t);
	float EaseInOutSine(float t);

#pragma region Quint

	// だんだん減速(ぎりぎりまで速度が速い)
	float EaseOutQuint(float x);

	// だんだん早くなる(一気に早くなる)
	float EaseInQuint(float x);

	float EaseInOutQuint(float x);

#pragma endregion

#pragma region Circ

	float EaseInCirc(float x);

	float EaseOutCirc(float x);

	float EaseInOutCirc(float x);

#pragma endregion

#pragma region Cubic

	float EaseInCubic(float x);

	float EaseOutCubic(float x);

	float EaseInOutCubic(float x);

#pragma endregion

#pragma region Back

	float EaseInBack(float x);

	float EaseOutBack(float x);

	float EaseInOutBack(float x);

#pragma endregion

#pragma region Bounce

	float EaseOutBounce(float x);

	float EaseInBounce(float x);

	float EaseInOutBounce(float x);

#pragma endregion

#pragma region Elastic

	float EaseInElastic(float x);

	float EaseOutElastic(float x);

	float EaseInOutElastic(float x);

#pragma endregion

#pragma region Quart

	float EaseInQuart(float x);

	float EaseOutQuart(float x);

	float EaseInOutQuart(float x);

#pragma endregion

#pragma region Expo

	float EaseInExpo(float x);

	float EaseOutExpo(float x);

	float EaseInOutExpo(float x);

#pragma endregion
}