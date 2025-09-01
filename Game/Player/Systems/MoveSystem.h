#pragma once
class MoveSystem {
public:
	// コンストラクタ
	MoveSystem() = default;
	// デストラクタ
	~MoveSystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:// Private method
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

private:
	// 稼働中か
	bool isActive_;
};