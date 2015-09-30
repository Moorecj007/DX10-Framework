//=============================================================================
// Water.fx                                                                                                   
//
// Transforms, lights, and textures geometry for Water Effects
//=============================================================================

// Local Includes
#include "LightStructures.fx"

cbuffer cbPerFrame
{
	Light g_light;
	float3 g_eyePosW;

	float4x4 g_matView;
	float4x4 g_matProj;
};

cbuffer cbPerObject
{
	float4x4 g_matWorld;
	float4x4 g_matTex;
	float4x4 g_matReflection;
	float g_reflectRefractScale;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D g_mapNormal;
Texture2D g_mapReflection;
Texture2D g_mapRefraction;

SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float4 position		: POSITION;
	float2 texCoord		: TEXCOORD0;
};

struct VS_OUT
{
	float4 position    : SV_POSITION;
    float2 texCoord     : TEXCOORD0;
	float4 reflectionPos     : TEXCOORD1;
	float4 refractionPos     : TEXCOORD2;
};
 
VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;
	matrix reflectProjectWorld;
	matrix viewProjectWorld;

	// Change the position vector to be 4 units for proper matrix calculations.
	_inputVS.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	outputVS.position = mul(_inputVS.position, g_matWorld);
	outputVS.position = mul(outputVS.position, g_matView);
	outputVS.position = mul(outputVS.position, g_matProj);

	// Store the texture coordinates for the pixel shader.
	outputVS.texCoord = _inputVS.texCoord; // mul(float4(_inputVS.texCoord, 0.0f, 1.0f), g_matTex).xy;

	// Create the reflection projection world matrix.
	reflectProjectWorld = mul(g_matReflection, g_matProj);
	reflectProjectWorld = mul(g_matWorld, reflectProjectWorld);

	// Calculate the input position against the reflectProjectWorld matrix.
	outputVS.reflectionPos = mul(_inputVS.position, reflectProjectWorld);

	// Create the view projection world matrix for refraction.
	viewProjectWorld = mul(g_matView, g_matProj);
	viewProjectWorld = mul(g_matWorld, viewProjectWorld);

	// Calculate the input position against the viewProjectWorld matrix.
	outputVS.refractionPos = mul(_inputVS.position, viewProjectWorld);

	return outputVS;
}

float4 PS_Standard(VS_OUT _inputPS) : SV_Target
{
	float2 reflectTexCoord;
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = _inputPS.reflectionPos.x / _inputPS.reflectionPos.w / 2.0f + 0.5f;
	reflectTexCoord.y = -_inputPS.reflectionPos.y / _inputPS.reflectionPos.w / 2.0f + 0.5f;

	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = _inputPS.refractionPos.x / _inputPS.refractionPos.w / 2.0f + 0.5f;
	refractTexCoord.y = -_inputPS.refractionPos.y / _inputPS.refractionPos.w / 2.0f + 0.5f;

	// Sample the normal from the normal map texture.
	normalMap = g_mapNormal.Sample(g_triLinearSam, _inputPS.texCoord);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalMap.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
	reflectTexCoord = reflectTexCoord + (normal.xy * g_reflectRefractScale);
	refractTexCoord = refractTexCoord + (normal.xy * g_reflectRefractScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	reflectionColor = g_mapReflection.Sample(g_triLinearSam, reflectTexCoord);
	refractionColor = g_mapRefraction.Sample(g_triLinearSam, refractTexCoord);

	// Combine the reflection and refraction results for the final color.
	color = lerp(reflectionColor, refractionColor, 0.9f);

	return color;
}

technique10 StandardTech
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
       	SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));	
    }
}