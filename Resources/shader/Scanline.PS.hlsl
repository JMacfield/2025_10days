#include "PostEffect.hlsli"

cbuffer ScanlineParams : register(b0)
{
    float intensity;
    float density;
    float speed;
    float time;
};

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = gTexture.Sample(gSampler, input.texcoord);
    
    float scanlineWave = sin((input.texcoord.y * density) + (time * speed));
    
    // この数値を小さくすると線が太くなります (例: 2.0f)
    float scanline = pow(abs(scanlineWave), 1500.0f);
    
    color.rgb *= 1.0f - (scanline * intensity);
    
    return color;
}