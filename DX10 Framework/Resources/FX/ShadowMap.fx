//=============================================================================
// ShadowMap.fx                                                                                                   
//
// Creates a Shadow Map
//=============================================================================

cbuffer cbPerFrame
{
	float4x4 g_matLightWVP;
};

// Nonnumeric values cannot be added to a cbuffer
Texture2D g_mapDiffuse;

SamplerState g_triLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 position     : POSITION;
	float3 normal		: NORMAL;
	float2 texCoord     : TEXCOORD;
};

struct VS_OUT
{
	float4 positionH	: SV_POSITION;
	float2 texCoord		: TEXCOORD;
};

VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;

	// Transform into homogenous space from the lights perpective
	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matLightWVP);
	outputVS.texCoord = _inputVS.texCoord;

	return outputVS;
}

void PS_Standard(VS_OUT _inputPS)
{
	float4 diffuse = g_mapDiffuse.Sample(g_triLinearSam, _inputPS.texCoord);

	// Clip all the transparent or mostly transparent pixels
	clip(diffuse.a - 0.15f);
}
RasterizerState RS_ShadowCulling
{
	CullMode = front;
};

technique10 CreateShadowMapTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Standard()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Standard()));
	}
}
