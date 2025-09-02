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
