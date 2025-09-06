#pragma once
#include "IPostEffectState.h"
#include "PSOProperty.h"
#include "Vector3.h"
#include "Timer.h" // Timer.hをインクルード

/**
* @file Scanline.h
* @brief Scanlineエフェクト
*/

// シェーダーに渡す定数バッファ用の構造体
struct ScanlineInfo {
    float intensity; // 線の強さ (0.0 ~ 1.0)
    float density;   // 線の密度
    float speed;     // 線が動く速度
    float time;      // 時間
};

class PostProcess;

class Scanline : public IPostEffectState
{
public:
    void Init() override;
    PSOProperty CreatePipelineStateObject() override;
    void CommandRootParameter(PostProcess* postProcess) override;

    // ... (他のオーバーライド関数) ...
    std::vector<D3D12_DESCRIPTOR_RANGE> CreateDescriptorRange() override;
    std::vector<D3D12_ROOT_PARAMETER> CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange) override;
    std::vector<D3D12_STATIC_SAMPLER_DESC> CreateSampler() override;
    D3D12_ROOT_SIGNATURE_DESC CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers) override;
    D3D12_INPUT_LAYOUT_DESC SetInputLayout() override;
    D3D12_BLEND_DESC SetBlendState() override;
    D3D12_RASTERIZER_DESC SetRasterizerState() override;
    D3D12_DEPTH_STENCIL_DESC CreateDepth() override;
private:
    // 定数バッファ用のリソース
    Microsoft::WRL::ComPtr<ID3D12Resource> cbufferResource_;
    // 書き込み用ポインタ
    ScanlineInfo* cbufferData_ = nullptr;
    // Scanlineクラス専用のタイマーを追加
    Timer timer_;
};