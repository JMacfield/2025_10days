#pragma once

/// <summary>
/// 自機機能の基底クラス
/// </summary>
class ISystem {
public:
	// コンストラクタ
	ISystem() = default;
	// デストラクタ
	virtual ~ISystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

public:// Accessor method
#pragma region Getter
	/// <summary>
	/// 機能が稼働しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 機能が稼働しているかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsActive(const bool& isActive) { isActive_ = isActive; }
#pragma endregion

protected:
	// 機能が稼働中か
	bool isActive_;
};