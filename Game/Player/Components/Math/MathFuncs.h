#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "mathFunction.h"

namespace MathFuncs {
	/// <summary>
	/// 回転行列の作成(ラジアン)
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateMatrix(const Vector3& radian);

	/// <summary>
	/// オフセットを回転させる
	/// </summary>
	/// <param name="offset"></param>
	/// <param name="rot"></param>
	/// <returns></returns>
	Vector3 TargetOffset(const Vector3& offset, const Vector3& rot);
	/// <summary>
	/// アフィン行列から座標成分のみ抽出
	/// </summary>
	/// <param name="translationMatrix"></param>
	/// <returns></returns>
	Vector3 GetWorldPosition(const Matrix4x4& m);

	/// <summary>
	/// スカラー倍
	/// </summary>
	/// <param name="scalar"></param>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 Multiply(const float& scalar, const Vector3& v);

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& v1, const Vector3& v2);

	namespace Lerps {
		// 線形補間
		Vector4 Lerp(const Vector4& start, const Vector4& end, float t);
		Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
		Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
		float Lerp(const float& start, const float& end, float t);
		// 球面線形補間
		Vector3 Slerp(const Vector3& start, const Vector3& end, float t);

		/// <summary>
		/// 指数関数的に数値のイージングを行う
		/// </summary>
		/// <param name="current"></param>
		/// <param name="target"></param>
		/// <param name="damping"></param>
		/// <returns></returns>
		Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& target, float damping);
		float ExponentialInterpolate(const float& current, const float& target, float damping);
	}

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
}