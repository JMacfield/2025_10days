#pragma once
class JumpSystem {
public:
	// コンストラクタ
	JumpSystem() = default;
	// デストラクタ
	~JumpSystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:
	// 稼働中か
	bool isActive_;
};