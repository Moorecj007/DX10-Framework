//=============================================================================
// LitTex.fx                                                                                                   
//
// Transforms, lights, and textures geometry.
//=============================================================================

// Local Includes
#include "LightStructures.fx"

cbuffer cbPerFrame
{
	
	float3 g_eyePosW;
	float4x4 g_matView;
	float4x4 g_matProj;

	// Can change if Reflections is used in the frame
	Light g_light;

	// Reflection 
	float4 g_plane;
};

cbuffer cbPerObject
{
	// Standard
	float4x4 g_matWorld;
	float4x4 g_matTex;

	// Fade + BlendTex
	float	 g_reduceAlpha;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D g_mapDiffuse;
Texture2D g_mapDiffuse2;
Texture2D g_mapSpec;

bool g_specularEnabled;

// Sampler
SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Structure for the Vertex Shader Input
struct VS_IN
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

// Structure for the Vertex Shader Output and Pixel Shader Input
struct VS_OUT
{
	float4 positionH    : SV_POSITION;
	float3 position     : POSITION;
	float3 normal		: NORMAL;
    float2 texCoord     : TEXCOORD;
};

// Blend state for Transparency Values
BlendState SrcAlphaBlendingAdd
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};
 
//--------------------------------------------------------------
// Standard Shading
//--------------------------------------------------------------

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

//--------------------------------------------------------------
// Fade
//--------------------------------------------------------------

float4 PS_Fade(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	diffuse.a = diffuse.a - g_reduceAlpha;

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);

	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };
	float3 litColor = ParallelLight(surface, g_light, g_eyePosW);

	return float4(litColor, diffuse.a);
}

technique10 FadeTech
{
	pass P0
	{
		// For transparency values
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Fade()));
	}
}

//--------------------------------------------------------------
// Blend 2 Textures + Fades
//--------------------------------------------------------------

float4 PS_BlendTex2(VS_OUT _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	//float4 diffuse2 = g_mapDiffuse2.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	//diffuse = diffuse + diffuse2;

	diffuse.a = diffuse.a - g_reduceAlpha;

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normal = normalize(_inputPS.normal);

	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, normal, diffuse, spec };
	float3 litColor = ParallelLight(surface, g_light, g_eyePosW);

	return float4(litColor, diffuse.a);
}

technique10 BlendTex2Tech
{
	pass P0
	{
		// For transparency values
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_BlendTex2()));
	}
}

//--------------------------------------------------------------
// Reflection on a plane
//--------------------------------------------------------------

float4 PS_Reflection(VS_OUT _inputPS) : SV_Target
{
	float numerator = g_plane.x * _inputPS.position.x + g_plane.y * _inputPS.position.y + g_plane.z * _inputPS.position.z + g_plane.w;
	float denom = sqrt(g_plane.x * g_plane.x + g_plane.y * g_plane.y + g_plane.z * g_plane.z);

	float distance = numerator / denom;

	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);

	if (distance > 0.0f)
	{
		// Guaranteed Clip
		clip(diffuse.a - 255);
	}
	
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

technique10 ReflectionTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Reflection()));
	}
}

//--------------------------------------------------------------
// Refraction
//--------------------------------------------------------------

// Structure for the Vertex Shader Output and Pixel Shader Input
struct VS_OUT_REFRACTION
{
	float4 positionH    : SV_POSITION;
	float3 position     : POSITION;
	float3 normal		: NORMAL;
	float2 texCoord     : TEXCOORD;
	float  clip			: SV_ClipDistance0;
};

VS_OUT_REFRACTION VS_Refraction(VS_IN _inputVS)
{
	VS_OUT_REFRACTION outputVS;

	// Transform to world space space.
	outputVS.position = mul(float4(_inputVS.position, 1.0f), g_matWorld).xyz;
	outputVS.normal = mul(float4(_inputVS.normal, 0.0f), g_matWorld).xyz;
	outputVS.normal = normalize(outputVS.normal);

	// Transform to homogeneous clip space.
	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWorld);
	outputVS.positionH = mul(outputVS.positionH, g_matView);
	outputVS.positionH = mul(outputVS.positionH, g_matProj);

	// Output vertex attributes for interpolation across triangle.
	outputVS.texCoord = mul(float4(_inputVS.texCoord, 0.0f, 1.0f), g_matTex).xy;

	// TO DO CAL
	outputVS.clip = dot(mul(float4(_inputVS.position, 1.0f), g_matWorld), g_plane);

	return outputVS;
}

float4 PS_Refraction(VS_OUT_REFRACTION _inputPS) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);
	float4 spec = g_mapSpec.Sample(g_triLinearSam, _inputPS.texCoord);

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Compute the lit color for this pixel.
	SurfaceInfo surface = { _inputPS.position, _inputPS.normal, diffuse, spec };
	float3 litColor = ParallelLight(surface, g_light, g_eyePosW);

	return float4(litColor, diffuse.a);
}

technique10 RefractionTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Refraction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Refraction()));
	}
}

