#include "Scanline.h"
#include <function.h>
#include <DirectXCommon.h>
#include "PostProcess.h"
#include "SRVManager.h"
#include "Mesh.h"
#include <d3dx12.h>

void Scanline::Init()
{
    // 定数バッファリソースを作成
    cbufferResource_ = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(ScanlineInfo));
    // 書き込むためのアドレスを取得
    cbufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&cbufferData_));

    // パラメータの初期値を設定
    cbufferData_->intensity = 1.0f;
    cbufferData_->density = 30000.0f;
    cbufferData_->speed = 1.0f;
    cbufferData_->time = 0.0f;

    // 内部タイマーを開始
    timer_.start();
}

PSOProperty Scanline::CreatePipelineStateObject()
{
    PSOProperty property;
    DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

    // ルートシグネチャとその他の設定
    std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange = CreateDescriptorRange();
    std::vector<D3D12_ROOT_PARAMETER> rootParameters = CreateRootParamerter(descriptorRange);
    std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = CreateSampler();
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature = CreateRootSignature(property, rootParameters, staticSamplers);
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = SetInputLayout();
    D3D12_BLEND_DESC blendDesc = SetBlendState();
    D3D12_RASTERIZER_DESC rasterizerDesc = SetRasterizerState();
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = CreateDepth();

    // シェーダーをコンパイル
    property.vertexShaderBlob = CompileShader(L"Resources/shader/Fullscreen.VS.hlsl",
        L"vs_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
    assert(property.vertexShaderBlob != nullptr);

    // Scanline用のピクセルシェーダーを指定
    property.pixelShaderBlob = CompileShader(L"Resources/shader/Scanline.PS.hlsl",
        L"ps_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
    assert(property.pixelShaderBlob != nullptr);

    // PSOを構築
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = property.rootSignature.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
    graphicsPipelineStateDesc.VS = { property.vertexShaderBlob->GetBufferPointer(), property.vertexShaderBlob->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { property.pixelShaderBlob->GetBufferPointer(), property.pixelShaderBlob->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // PSOを生成
    HRESULT hr = sDirectXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&property.graphicsPipelineState));
    assert(SUCCEEDED(hr));
    return property;
}


void Scanline::CommandRootParameter(PostProcess* postProcess)
{
    DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

    // 内部タイマーから経過時間を取得し、シェーダーに渡す
    cbufferData_->time = static_cast<float>(timer_.elapsedSeconds());

    // テクスチャなどの設定
    sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(0, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetRenderIndex()));
    sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetDepthIndex()));
    sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetGPUDescriptorHandle(postProcess->GetNoisetex()));

    // 定数バッファを設定
    sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbufferResource_->GetGPUVirtualAddress());
}

// ↓↓↓【エラー修正】不足していた関数群を追加 ↓↓↓
// ----------------------------------------------------------------------------------
// 以下の関数群は多くのエフェクトで共通のため、他の.cppからコピーしてきたものです
// ----------------------------------------------------------------------------------

std::vector<D3D12_DESCRIPTOR_RANGE> Scanline::CreateDescriptorRange() {
    std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange(3);
    descriptorRange[0] = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
    descriptorRange[1] = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
    descriptorRange[2] = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
    return descriptorRange;
}

std::vector<D3D12_ROOT_PARAMETER> Scanline::CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange) {
    std::vector<D3D12_ROOT_PARAMETER> rootParameters(4);
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[0].DescriptorTable = { 1, &descriptorRange[0] };
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[1].DescriptorTable = { 1, &descriptorRange[1] };
    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[2].DescriptorTable = { 1, &descriptorRange[2] };
    rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[3].Descriptor = { 0, 0 };
    return rootParameters;
}

std::vector<D3D12_STATIC_SAMPLER_DESC> Scanline::CreateSampler() {
    std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers(2);
    staticSamplers[0] = { D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK, 0, D3D12_FLOAT32_MAX, 0, 0, D3D12_SHADER_VISIBILITY_PIXEL };
    staticSamplers[1] = { D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0, 16, D3D12_COMPARISON_FUNC_NEVER, D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK, 0, D3D12_FLOAT32_MAX, 1, 0, D3D12_SHADER_VISIBILITY_PIXEL };
    return staticSamplers;
}

D3D12_ROOT_SIGNATURE_DESC Scanline::CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers) {
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    descriptionRootSignature.pParameters = rootParameters.data();
    descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size());
    descriptionRootSignature.pStaticSamplers = samplers.data();
    descriptionRootSignature.NumStaticSamplers = static_cast<UINT>(samplers.size());

    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &pso.signatureBlob, &pso.errorBlob);
    if (FAILED(hr)) {
        ConsoleLog(reinterpret_cast<char*>(pso.errorBlob->GetBufferPointer()));
        assert(false);
    }
    hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, pso.signatureBlob->GetBufferPointer(), pso.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pso.rootSignature));
    assert(SUCCEEDED(hr));
    return descriptionRootSignature;
}

D3D12_INPUT_LAYOUT_DESC Scanline::SetInputLayout() {
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
    inputLayoutDesc.pInputElementDescs = nullptr;
    inputLayoutDesc.NumElements = 0;
    return inputLayoutDesc;
}

D3D12_BLEND_DESC Scanline::SetBlendState() {
    D3D12_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    return blendDesc;
}

D3D12_RASTERIZER_DESC Scanline::SetRasterizerState() {
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    return rasterizerDesc;
}

D3D12_DEPTH_STENCIL_DESC Scanline::CreateDepth() {
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = false;
    return depthStencilDesc;
}