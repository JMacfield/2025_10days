/**
* @file Glitch.PS.hlsl
* @brief Glitch effect pixel shader
*/

#include "PostEffect.hlsli" // Include common definitions

// This constant buffer must match the GlitchInfo struct in Glitch.h
cbuffer GlitchParams : register(b0)
{
    float intensity;
    float time;
    float blockSize;
    float noiseSpeed;
};

// A simple function to generate a pseudo-random number
float random(float2 uv)
{
    // Uses the UV coordinates and time to create a seemingly random value
    return frac(sin(dot(uv, float2(12.9898f, 78.233f)) + time * noiseSpeed) * 43758.5453123f);
}

// Pixel shader main function
float4 main(VSOutput input) : SV_TARGET
{
    // Get the original pixel color from the scene texture
    float4 color = gTexture.Sample(gSampler, input.texcoord);
    
    // Quantize the Y-coordinate into blocks. This creates horizontal bands for the glitch.
    float blockyY = floor(input.texcoord.y * blockSize) / blockSize;
    
    // Generate a random number based on the blocky Y-coordinate.
    // This makes all pixels in the same horizontal band get the same random value.
    float glitchRandom = random(float2(0.0f, blockyY));
    
    // Set a threshold to determine if the glitch should occur.
    // A higher intensity makes the threshold lower, causing glitches to happen more often.
    float threshold = 1.0f - (intensity * 0.9f);

    // If the random number exceeds the threshold, apply the glitch effect
    if (glitchRandom > threshold)
    {
        // Calculate a horizontal offset based on how much the random number exceeded the threshold
        float offset = (glitchRandom - threshold) * 0.2f * intensity;
        float2 glitchUV = input.texcoord;
        glitchUV.x += offset; // Shift the texture coordinate horizontally

        // Create a chromatic aberration (color fringing) effect
        // Sample the Red, Green, and Blue channels from slightly different horizontal positions
        float r = gTexture.Sample(gSampler, glitchUV + float2(offset * 0.5f, 0.0f)).r;
        float g = gTexture.Sample(gSampler, glitchUV).g;
        float b = gTexture.Sample(gSampler, glitchUV - float2(offset * 0.5f, 0.0f)).b;
        
        // Combine the shifted color channels to create the final glitched color
        color = float4(r, g, b, 1.0f);
    }
    
    return color;
}