//=============================================================================
// LitTex.fx                                                                                                   
//
// Creates shadow map for one light
//=============================================================================

cbuffer cbPerFrame
{
	float4x4 g_matProjLight;
};

cbuffer cbPerLight
{
	float4x4 g_matWorldObject;
	float4x4 g_matViewLight;
	float4x4 g_matWVP;
};

// Nonnumeric values cannot be added to a cbuffer
Texture2D g_diffuseMap;

SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
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
	float2 texCoord     : TEXCOORD;
};

VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;

	// Transform to homogeneous clip space.
	//outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWorldObject);
	//outputVS.positionH = mul(outputVS.positionH, g_matViewLight);
	//outputVS.positionH = mul(outputVS.positionH, g_matProjLight);

	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWVP);

	outputVS.texCoord = _inputVS.texCoord;

	return outputVS;
}

void PS_Standard(VS_OUT _inputPS)
{
	float4 diffuse = g_diffuseMap.Sample(g_triLinearSam, _inputPS.texCoord);

	// Clip all transparent pixels
	clip(diffuse.a - 0.15f);
}

technique10 CreateShadowMapTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));
	}
}
