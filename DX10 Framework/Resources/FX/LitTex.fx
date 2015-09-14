//=============================================================================
// LitTex.fx                                                                                                   
//
// Transforms, lights, and textures geometry.
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
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D g_mapDiffuse;
Texture2D g_mapSpec;

bool g_specularEnabled;

SamplerState g_triLinearSam
{
	Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

struct VS_OUT
{
	float4 positionH    : SV_POSITION;
    float3 position     : POSITION;
    float3 normal		: NORMAL;
    float2 texCoord     : TEXCOORD;
};
 
VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;
	
	// Transform to world space space.
	outputVS.position = mul(float4(_inputVS.position, 1.0f), g_matWorld).xyz;
	outputVS.normal = mul(float4(_inputVS.normal, 0.0f), g_matWorld).xyz;
		
	// Transform to homogeneous clip space.
	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWorld);
	outputVS.positionH = mul(outputVS.positionH, g_matView);
	outputVS.positionH = mul(outputVS.positionH, g_matProj);
	
	// Output vertex attributes for interpolation across triangle.
	outputVS.texCoord = mul(float4(_inputVS.texCoord, 0.0f, 1.0f), g_matTex).xy;
	
	return outputVS;
}

float4 PS_Standard(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;
	
	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);
    
	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };
	float3 litColor = ParallelLight(surface, g_light, g_eyePosW);
    
	return float4(litColor, diffuse.a);
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

technique10 AnimateWaterTech
{
	pass P0
	{
		// TO DO - Set Blend State
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));
	}
}
