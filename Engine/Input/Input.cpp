#include "Input.h"
#include "WinAPI.h"
/**
* @file Input.cpp
* @brief 入力デバイスの管理を行うクラス
*/

void Input::Initialize() {
	WinAPI* sWinAPI = WinAPI::GetInstance();
	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		sWinAPI->GetWc().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		sWinAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
		}
		else
		{
			// Controller is not connected
		}
	}


}

void Input::Update() {
	// 前回のキー入力を保存
	memcpy(preKeys, keys, sizeof(keys));

	// キーボード情報の取得開始
	keyboard->Acquire();

	keyboard->GetDeviceState(sizeof(keys), keys);

	memcpy(&preState_, &state_, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &state_);

	if (dwResult != ERROR_SUCCESS) {
		ZeroMemory(&state_, sizeof(XINPUT_STATE));
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	if (keys[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::GetJoystickState(XINPUT_STATE& state)
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// コントローラーの状態を取得
	result = XInputGetState(0, &state);

	if (result == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (keys[keyNumber] && preKeys[keyNumber] == 0) {
		return true;
	}
	return false;
}
Input* Input::GetInstance() {
	if (!instance_) {
		instance_ = std::make_unique<Input>();
	}
	return instance_.get();
}

bool Input::PushButton(WORD button)
{
	if (state_.Gamepad.wButtons & button) {
		return true;
	}
	return false;
}

bool Input::TriggerButton(WORD button)
{
	// 現在押されていて、かつ前回は押されていないか
	if ((state_.Gamepad.wButtons & button) && !(preState_.Gamepad.wButtons & button)) {
		return true;
	}
	return false;
}