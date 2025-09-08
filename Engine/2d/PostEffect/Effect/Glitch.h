#pragma once
#include "IPostEffectState.h"
#include "PSOProperty.h"
#include "Timer.h"

/**
* @file Glitch.h
* @brief Glitch effect
*/

// This struct must exactly match the cbuffer in the shader
struct GlitchInfo {
    float intensity;
    float time;
    float blockSize;  // Changed from block_size
    float noiseSpeed; // Changed from speed
};

class PostProcess;

class Glitch : public IPostEffectState
{
public:
    void Init() override;
    PSOProperty CreatePipelineStateObject() override;
    void CommandRootParameter(PostProcess* postProcess) override;

    std::vector<D3D12_DESCRIPTOR_RANGE> CreateDescriptorRange() override;
    std::vector<D3D12_ROOT_PARAMETER> CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange) override;
    std::vector<D3D12_STATIC_SAMPLER_DESC> CreateSampler() override;
    D3D12_ROOT_SIGNATURE_DESC CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers) override;
    D3D12_INPUT_LAYOUT_DESC SetInputLayout() override;
    D3D12_BLEND_DESC SetBlendState() override;
    D3D12_RASTERIZER_DESC SetRasterizerState() override;
    D3D12_DEPTH_STENCIL_DESC CreateDepth() override;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> cbufferResource_;
    GlitchInfo* cbufferData_ = nullptr;
    Timer timer_;
};