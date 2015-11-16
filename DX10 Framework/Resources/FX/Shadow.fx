//=============================================================================
// shadow.fx by Frank Luna (C) 2008 All Rights Reserved.
// TO DO CAL
// Demonstrates shadow mapping.
//=============================================================================

#include "LightStructures.fx"
 
 
cbuffer cbPerFrame
{
};

cbuffer cbPerObject
{
	float4x4 gLightWVP; 
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gSpecMap;
Texture2D gShadowMap;

// Global
float3 lightPosition;

SamplerState gShadowSam
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};	

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 posL     : POSITION;
	float3 normalL  : NORMAL;
	float2 texC     : TEXCOORD;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ShadowVertexShader(VS_IN input)
{
	PixelInputType output;
	float4 worldPosition;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(float4(input.posL, 1.0f), gWVP);

	// Calculate the position of the vertice as viewed by the light source.
	output.lightViewPosition = mul(float4(input.posL, 1.0f), gLightWVP);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.texC;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normalL, (float3x3)gWorld);
	output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
	worldPosition = mul(float4(input.posL, 1.0f), gWorld);

	// Determine the light position based on the position of the light and the position of the vertex in the world.
	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ShadowPixelShader(PixelInputType input) : SV_Target
{
	float bias;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;

	// Set the bias value for fixing the floating point precision issues.
	bias = 0.003f;

	// Set the default output color to be black (shadow).
	color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = gShadowMap.Sample(gShadowSam, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if (lightDepthValue < depthValue)
		{
			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos));

			// If this pixel is illuminated then set it to pure white (non-shadow).
			if (lightIntensity > 0.0f)
			{
				color = float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 BlackWhiteShadowTech
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, ShadowVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, ShadowPixelShader()));
		SetGeometryShader(NULL);
	}
}