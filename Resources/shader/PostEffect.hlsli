/**
* @file PostEffect.hlsli
* @brief ポストエフェクト共通で使う定義
*/

// シーンが描画されたテクスチャ
Texture2D<float4> gTexture : register(t0); 
// 深度バッファのテクスチャ (必要であれば利用)
Texture2D<float> gDepthTexture : register(t1); 
// ノイズテクスチャ (グリッチなどで利用)
Texture2D<float4> gNoiseTexture : register(t2);

// サンプラー (t0, t1, t2 のテクスチャをサンプリングする際に使う)
SamplerState gSampler : register(s0); // リニア補間のサンプラー
SamplerState gPointSampler : register(s1); // ポイント補間のサンプラー

// 頂点シェーダーからピクセルシェーダーへ渡すデータ構造
struct VSOutput
{
    float4 svpos    : SV_POSITION;  // スクリーン座標 (DirectXが自動的に付与)
    float2 texcoord : TEXCOORD0;    // テクスチャ座標
};