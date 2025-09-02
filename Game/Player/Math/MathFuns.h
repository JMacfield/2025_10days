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
	/// 指数関数的に数値のイージングを行う
	/// </summary>
	/// <param name="current"></param>
	/// <param name="target"></param>
	/// <param name="damping"></param>
	/// <returns></returns>
	Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& target, float damping);
	float ExponentialInterpolate(const float& current, const float& target, float damping);
}
