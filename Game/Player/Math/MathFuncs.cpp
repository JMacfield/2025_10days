#include "MathFuncs.h"

namespace MathFuncs {
	Matrix4x4 MakeRotateMatrix(const Vector3& radian) {
		Matrix4x4 rotateX{};
		Matrix4x4 rotateY{};
		Matrix4x4 rotateZ{};
		rotateX = MakeRotateXMatrix(radian.x);
		rotateY = MakeRotateYMatrix(radian.y);
		rotateZ = MakeRotateZMatrix(radian.z);

		Matrix4x4 result{};
		result = Multiply(rotateX, Multiply(rotateZ, rotateY));

		return result;
	}

	Vector3 TargetOffset(const Vector3& offset, const Vector3& rot) {
		// 追従対象からのオフセット
		Vector3 result = offset;
		// 回転行列を合成
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rot);

		// オフセットをカメラの回転に合わせて回転
		result = TransformNormal(offset, rotateMatrix);

		return result;
	}

	Vector3 GetWorldPosition(const Matrix4x4& m) {
		// ワールド行列の平行移動成分を取得
		Vector3 worldPosition = {
			m.m[3][0],
			m.m[3][1],
			m.m[3][2]
		};
		return worldPosition;
	}

	Vector3 Multiply(const float& scalar, const Vector3& v) {
		Vector3 result;
		result.x = v.x * scalar;
		result.y = v.y * scalar;
		result.z = v.z * scalar;
		return result;
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return { v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x };
	}

	Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& target, float damping) {
		float factor = 1.0f - std::exp(-damping);
		return current + (target - current) * factor;
	}
	float ExponentialInterpolate(const float& current, const float& target, float damping) {
		float factor = 1.0f - std::exp(-damping);
		return current + (target - current) * factor;
	}
}