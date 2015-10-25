//=============================================================================
// Cloth.fx                                                                                                   
//
// Calculate and display Cloth using contraints to draw the lines
//=============================================================================

cbuffer cbPerFrame
{
	float3 g_eyePosW;
	float4x4 g_matView;
	float4x4 g_matProj;
};

cbuffer cbPerObject
{
	// Standard
	float4x4 g_matWorld;
};

struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
};

struct VS_OUT
{
	float4 positionH    : SV_POSITION;
	float3 position     : POSITION;
	float4 color		: COLOR;
};
 
VS_OUT VS_Standard(VS_IN _inputVS)
{
	VS_OUT outputVS;
	
	// Transform to world space space.
	outputVS.position = mul(float4(_inputVS.position, 1.0f), g_matWorld).xyz;
		
	// Transform to homogeneous clip space.
	outputVS.positionH = mul(float4(_inputVS.position, 1.0f), g_matWorld);
	outputVS.positionH = mul(outputVS.positionH, g_matView);
	outputVS.positionH = mul(outputVS.positionH, g_matProj);

	// Past the color along
	outputVS.color = _inputVS.color;
	
	return outputVS;
}

float4 PS_Standard(VS_OUT _inputPS) : SV_Target
{
	return _inputPS.color;
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